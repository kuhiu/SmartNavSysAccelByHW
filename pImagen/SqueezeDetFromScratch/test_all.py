import numpy as np
from keras.layers import Input, MaxPool2D,  Conv2D, Dropout, concatenate, Reshape, Lambda, AveragePooling2D
import tensorflow as tf
from keras.initializers import TruncatedNormal
from keras.regularizers import l2
from keras import backend as K
import utils.utils as utils
from keras.models import Model
from keras import optimizers
from create_config import load_dict

sess = tf.InteractiveSession()

#class that wraps config and model
class SqueezeDet():
    #initialize model from config file
    def __init__(self, config):
        """Init of SqueezeDet Class
        
        Arguments:
            config {[type]} -- dict containing hyperparameters for network building
        """

        #hyperparameter config file
        self.config = config
        #create Keras model
        self.model = self._create_model()

    def _create_model(self):
        """
        #builds the Keras model from config
        #return: squeezeDet in Keras
        """
        input_layer = Input(shape=( 7, 7, 1), name="input")

        conv1 = Conv2D(filters=1, kernel_size=(3, 3), strides=(2, 2), padding="valid", activation='relu',
                        use_bias=False, kernel_initializer=TruncatedNormal(stddev=0.001),
                        kernel_regularizer=l2(0.001),  name="conv1")(input_layer)


        #pool1 = MaxPool2D(pool_size=(3,3), strides=(2, 2), padding='SAME', name="pool1")(conv1)

        model = Model(inputs=input_layer, outputs=conv1)
        
        print(model.summary())

        return model

    #wrapper for padding, written in tensorflow. If you want to change to theano you need to rewrite this!
    def _pad(self, input):
        """
        pads the network output so y_pred and y_true have the same dimensions
        :param input: previous layer
        :return: layer, last dimensions padded for 4
        """

        #pad = K.placeholder( (None,self.config.ANCHORS, 4))


        #pad = np.zeros ((self.config.BATCH_SIZE,self.config.ANCHORS, 4))
        #return K.concatenate( [input, pad], axis=-1)


        padding = np.zeros((3,2))
        padding[2,1] = 4
        return tf.pad(input, padding ,"CONSTANT")



    #loss function to optimize
    def loss(self, y_true, y_pred):
        """
        squeezeDet loss function for object detection and classification
        :param y_true: ground truth with shape [batchsize, #anchors, classes+8+labels]
        :param y_pred:
        :return: a tensor of the total loss
        """

        #handle for config
        mc = self.config

        #slice y_true
        input_mask = y_true[:, :, 0]
        input_mask = K.expand_dims(input_mask, axis=-1)
        box_input = y_true[:, :, 1:5]
        box_delta_input = y_true[:, :, 5:9]
        labels = y_true[:, :, 9:]

        #number of objects. Used to normalize bbox and classification loss
        num_objects = K.sum(input_mask)


        #before computing the losses we need to slice the network outputs
        pred_class_probs, pred_conf, pred_box_delta = utils.slice_predictions(y_pred, mc)

        #compute boxes
        det_boxes = utils.boxes_from_deltas(pred_box_delta, mc)

        #again unstack is not avaible in pure keras backend
        unstacked_boxes_pred = []
        unstacked_boxes_input = []

        for i in range(4):
            unstacked_boxes_pred.append(det_boxes[:, :, i])
            unstacked_boxes_input.append(box_input[:, :, i])



        #compute the ious
        ious = utils.tensor_iou(utils.bbox_transform(unstacked_boxes_pred),
                                utils.bbox_transform(unstacked_boxes_input),
                                input_mask,
                                mc
                                )


        
        #compute class loss,add a small value into log to prevent blowing up
        class_loss = K.sum(labels * (-K.log(pred_class_probs + mc.EPSILON))
                 + (1 - labels) * (-K.log(1 - pred_class_probs + mc.EPSILON))
                * input_mask * mc.LOSS_COEF_CLASS) / num_objects

        #bounding box loss
        bbox_loss = (K.sum(mc.LOSS_COEF_BBOX * K.square(input_mask * (pred_box_delta - box_delta_input))) / num_objects)

        #reshape input for correct broadcasting
        input_mask = K.reshape(input_mask, [mc.BATCH_SIZE, mc.ANCHORS])

        #confidence score loss
        conf_loss = K.mean(
            K.sum(
                K.square((ious - pred_conf))
                * (input_mask * mc.LOSS_COEF_CONF_POS / num_objects
                   + (1 - input_mask) * mc.LOSS_COEF_CONF_NEG / (mc.ANCHORS - num_objects)),
                axis=[1]
            ),
        )

        # add above losses
        total_loss = class_loss + conf_loss + bbox_loss

        return total_loss


    #the sublosses, to be used as metrics during training

    def bbox_loss(self, y_true, y_pred):
        """
        squeezeDet loss function for object detection and classification
        :param y_true: ground truth with shape [batchsize, #anchors, classes+8+labels]
        :param y_pred:
        :return: a tensor of the bbox loss
        """

        #handle for config
        mc = self.config

        #calculate non padded entries
        n_outputs = mc.CLASSES + 1 + 4

        #slice and reshape network output
        y_pred = y_pred[:, :, 0:n_outputs]
        y_pred = K.reshape(y_pred, (mc.BATCH_SIZE, mc.N_ANCHORS_HEIGHT, mc.N_ANCHORS_WIDTH, -1))


        #slice y_true
        input_mask = y_true[:, :, 0]
        input_mask = K.expand_dims(input_mask, axis=-1)
        box_delta_input = y_true[:, :, 5:9]

        #number of objects. Used to normalize bbox and classification loss
        num_objects = K.sum(input_mask)


        #before computing the losses we need to slice the network outputs

        #number of class probabilities, n classes for each anchor
        num_class_probs = mc.ANCHOR_PER_GRID * mc.CLASSES

        #number of confidence scores, one for each anchor + class probs
        num_confidence_scores = mc.ANCHOR_PER_GRID+num_class_probs

        #slice the confidence scores and put them trough a sigmoid for probabilities
        pred_conf = K.sigmoid(
            K.reshape(
                  y_pred[:, :, :, num_class_probs:num_confidence_scores],
                  [mc.BATCH_SIZE, mc.ANCHORS]
              )
          )

        #slice remaining bounding box_deltas
        pred_box_delta = K.reshape(
              y_pred[:, :, :, num_confidence_scores:],
              [mc.BATCH_SIZE, mc.ANCHORS, 4]
          )


        # cross-entropy: q * -log(p) + (1-q) * -log(1-p)
        # add a small value into log to prevent blowing up


        #bounding box loss
        bbox_loss = (K.sum(mc.LOSS_COEF_BBOX * K.square(input_mask * (pred_box_delta - box_delta_input))) / num_objects)




        return bbox_loss


    def conf_loss(self, y_true, y_pred):
        """
        squeezeDet loss function for object detection and classification
        :param y_true: ground truth with shape [batchsize, #anchors, classes+8+labels]
        :param y_pred:
        :return: a tensor of the conf loss
        """

        #handle for config
        mc = self.config

        #calculate non padded entries
        n_outputs = mc.CLASSES + 1 + 4

        #slice and reshape network output
        y_pred = y_pred[:, :, 0:n_outputs]
        y_pred = K.reshape(y_pred, (mc.BATCH_SIZE, mc.N_ANCHORS_HEIGHT, mc.N_ANCHORS_WIDTH, -1))


        #slice y_true
        input_mask = y_true[:, :, 0]
        input_mask = K.expand_dims(input_mask, axis=-1)
        box_input = y_true[:, :, 1:5]

        #number of objects. Used to normalize bbox and classification loss
        num_objects = K.sum(input_mask)


        #before computing the losses we need to slice the network outputs

        #number of class probabilities, n classes for each anchor
        num_class_probs = mc.ANCHOR_PER_GRID * mc.CLASSES



        #number of confidence scores, one for each anchor + class probs
        num_confidence_scores = mc.ANCHOR_PER_GRID+num_class_probs

        #slice the confidence scores and put them trough a sigmoid for probabilities
        pred_conf = K.sigmoid(
            K.reshape(
                  y_pred[:, :, :, num_class_probs:num_confidence_scores],
                  [mc.BATCH_SIZE, mc.ANCHORS]
              )
          )

        #slice remaining bounding box_deltas
        pred_box_delta = K.reshape(
              y_pred[:, :, :, num_confidence_scores:],
              [mc.BATCH_SIZE, mc.ANCHORS, 4]
          )

        #compute boxes
        det_boxes = utils.boxes_from_deltas(pred_box_delta, mc)


        #again unstack is not avaible in pure keras backend
        unstacked_boxes_pred = []
        unstacked_boxes_input = []

        for i in range(4):
            unstacked_boxes_pred.append(det_boxes[:, :, i])
            unstacked_boxes_input.append(box_input[:, :, i])



        #compute the ious
        ious = utils.tensor_iou(utils.bbox_transform(unstacked_boxes_pred),
                                utils.bbox_transform(unstacked_boxes_input),
                                input_mask,
                                mc
                                )



        #reshape input for correct broadcasting
        input_mask = K.reshape(input_mask, [mc.BATCH_SIZE, mc.ANCHORS])

        #confidence score loss
        conf_loss = K.mean(
            K.sum(
                K.square((ious - pred_conf))
                * (input_mask * mc.LOSS_COEF_CONF_POS / num_objects
                   + (1 - input_mask) * mc.LOSS_COEF_CONF_NEG / (mc.ANCHORS - num_objects)),
                axis=[1]
            ),
        )



        return conf_loss


    def class_loss(self, y_true, y_pred):
        """
        squeezeDet loss function for object detection and classification
        :param y_true: ground truth with shape [batchsize, #anchors, classes+8+labels]
        :param y_pred:
        :return: a tensor of the class loss
        """

        #handle for config
        mc = self.config

        #calculate non padded entries
        n_outputs = mc.CLASSES + 1 + 4

        #slice and reshape network output
        y_pred = y_pred[:, :, 0:n_outputs]
        y_pred = K.reshape(y_pred, (mc.BATCH_SIZE, mc.N_ANCHORS_HEIGHT, mc.N_ANCHORS_WIDTH, -1))


        #slice y_true
        input_mask = y_true[:, :, 0]
        input_mask = K.expand_dims(input_mask, axis=-1)
        labels = y_true[:, :, 9:]

        #number of objects. Used to normalize bbox and classification loss
        num_objects = K.sum(input_mask)


        #before computing the losses we need to slice the network outputs

        #number of class probabilities, n classes for each anchor
        num_class_probs = mc.ANCHOR_PER_GRID * mc.CLASSES

        #slice pred tensor to extract class pred scores and then normalize them
        pred_class_probs = K.reshape(
            K.softmax(
                K.reshape(
                    y_pred[:, :, :, :num_class_probs],
                    [-1, mc.CLASSES]
                )
            ),
            [mc.BATCH_SIZE, mc.ANCHORS, mc.CLASSES],
        )



        # cross-entropy: q * -log(p) + (1-q) * -log(1-p)
        # add a small value into log to prevent blowing up


        #compute class loss
        class_loss = K.sum((labels * (-K.log(pred_class_probs + mc.EPSILON))
                 + (1 - labels) * (-K.log(1 - pred_class_probs + mc.EPSILON)))
                * input_mask * mc.LOSS_COEF_CLASS) / num_objects




        return class_loss


    #loss function again, used for metrics to show loss without regularization cost, just of copy of the original loss
    def loss_without_regularization(self, y_true, y_pred):
        """
        squeezeDet loss function for object detection and classification
        :param y_true: ground truth with shape [batchsize, #anchors, classes+8+labels]
        :param y_pred:
        :return: a tensor of the total loss
        """

        #handle for config
        mc = self.config

        #slice y_true
        input_mask = y_true[:, :, 0]
        input_mask = K.expand_dims(input_mask, axis=-1)
        box_input = y_true[:, :, 1:5]
        box_delta_input = y_true[:, :, 5:9]
        labels = y_true[:, :, 9:]

        #number of objects. Used to normalize bbox and classification loss
        num_objects = K.sum(input_mask)


        #before computing the losses we need to slice the network outputs

        pred_class_probs, pred_conf, pred_box_delta = utils.slice_predictions(y_pred, mc)

        #compute boxes
        det_boxes = utils.boxes_from_deltas(pred_box_delta, mc)

        #again unstack is not avaible in pure keras backend
        unstacked_boxes_pred = []
        unstacked_boxes_input = []

        for i in range(4):
            unstacked_boxes_pred.append(det_boxes[:, :, i])
            unstacked_boxes_input.append(box_input[:, :, i])



        #compute the ious
        ious = utils.tensor_iou(utils.bbox_transform(unstacked_boxes_pred),
                                utils.bbox_transform(unstacked_boxes_input),
                                input_mask,
                                mc)


        # cross-entropy: q * -log(p) + (1-q) * -log(1-p)
        # add a small value into log to prevent blowing up


        #compute class loss
        class_loss = K.sum(labels * (-K.log(pred_class_probs + mc.EPSILON))
                 + (1 - labels) * (-K.log(1 - pred_class_probs + mc.EPSILON))
                * input_mask * mc.LOSS_COEF_CLASS) / num_objects



        #bounding box loss
        bbox_loss = (K.sum(mc.LOSS_COEF_BBOX * K.square(input_mask * (pred_box_delta - box_delta_input))) / num_objects)

        #reshape input for correct broadcasting
        input_mask = K.reshape(input_mask, [mc.BATCH_SIZE, mc.ANCHORS])

        #confidence score loss
        conf_loss = K.mean(
            K.sum(
                K.square((ious - pred_conf))
                * (input_mask * mc.LOSS_COEF_CONF_POS / num_objects
                   + (1 - input_mask) * mc.LOSS_COEF_CONF_NEG / (mc.ANCHORS - num_objects)),
                axis=[1]
            ),
        )

        # add above losses 
        total_loss = class_loss + conf_loss + bbox_loss

        return total_loss






CONFIG = "squeeze.config"

#create config object
cfg = load_dict(CONFIG)

SqueezeDet = SqueezeDet(cfg)

#dummy optimizer for compilation
sgd = optimizers.SGD(lr=0.01, decay=0, momentum=0.9, nesterov=False, clipnorm=1.0)

SqueezeDet.model.compile(optimizer=sgd, loss=[SqueezeDet.loss])

#print("Los pesos son: \n", ( SqueezeDet.model.get_weights()[0] ) )

#for i, weights in enumerate( SqueezeDet.model.get_weights() ):
#    print("i y pesos son: \n", i,weights)
#    print("pesos forma: \n", weights.shape)

kernel_conv1_load=[]

kernel_conv1 =  np.array ( [    [ 1, 1, 1 ],
                                [ 2, 2, 2 ],
                                [ 3, 3, 3 ],   ])

#print("load forma: \n", load.shape)
kernel_conv1 = np.reshape(kernel_conv1, [3, 3, 1, 1])

kernel_conv1_load.append(kernel_conv1)

SqueezeDet.model.layers[1].set_weights(kernel_conv1_load)

####################################################################
# Input
####################################################################
in_image  = np.array ([ [ 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ],
                        [ 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0 ],
                        [ 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 ],
                        [ 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 ],
                        [ 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0 ],
                        [ 6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0 ],
                        [ 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0 ],  ])

in_image = np.reshape(in_image, [1, 7, 7, 1])

intermediate_layer_model = Model(inputs=SqueezeDet.model.input, outputs=SqueezeDet.model.get_layer("conv1").output)
intermediate_output = intermediate_layer_model.predict( in_image )
print( intermediate_output )











""" ####################################################################
# Conv1
####################################################################
input_layer = Input(shape=( 7, 7, 1), name="input")
conv1 = Conv2D(filters=64, kernel_size=(3, 3), strides=(2, 2), padding="SAME", activation='relu',
                use_bias=True, kernel_initializer=TruncatedNormal(stddev=0.001),
                kernel_regularizer=l2(0.001))(input_layer)
#conv1 = tf.keras.layers.Conv2D( filters=1, kernel_size=(3, 3), strides=(2, 2), padding="valid", use_bias=False) (in_image)
print( "Conv2D \n", ( conv1(in_image) ) )
####################################################################
# MaxPool1
####################################################################
max_pool_2d = tf.keras.layers.MaxPooling2D(pool_size=(3, 3), strides=(2, 2), padding='valid') (in_image)

print ( "max_pool_2d \n", sess.run( max_pool_2d )) """

