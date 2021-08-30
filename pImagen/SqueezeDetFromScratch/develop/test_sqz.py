import numpy as np
from keras.layers import Input, MaxPool2D,  Conv2D, Dropout, concatenate, Reshape, Lambda, AveragePooling2D
import tensorflow as tf
from keras.initializers import TruncatedNormal
import utils.utils as utils
from keras.regularizers import l2
from keras import backend as K
from keras.models import Model
from keras import optimizers
from create_config import load_dict
import cv2


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
        input_layer = Input(shape=( 70, 70, 3), name="input")

        conv1 = Conv2D(filters=2, kernel_size=(3, 3), strides=(2, 2), padding="valid", activation='relu',
                        use_bias=True, kernel_initializer=TruncatedNormal(stddev=0.001),
                        kernel_regularizer=l2(0.001),  name="conv1")(input_layer)


        pool1 = MaxPool2D(pool_size=(3,3), strides=(2, 2), padding='SAME', name="pool1")(conv1)

        model = Model(inputs=input_layer, outputs=pool1)
        
        print(model.summary())

        return model

        # input_layer = Input(shape=( self.config.IMAGE_HEIGHT, self.config.IMAGE_WIDTH, self.config.N_CHANNELS), name="input")

        # conv1 = Conv2D(filters=64, kernel_size=(3, 3), strides=(2, 2), padding="SAME", activation='relu',
        #                use_bias=True, kernel_initializer=TruncatedNormal(stddev=0.001),
        #                kernel_regularizer=l2(self.config.WEIGHT_DECAY))(input_layer)


        # pool1 = MaxPool2D(pool_size=(3,3), strides=(2, 2), padding='SAME', name="pool1")(conv1)


        # fire2 = self._fire_layer(name="fire2", input = pool1, s1x1=16, e1x1=64, e3x3=64)


        # fire3 = self._fire_layer(
        #     'fire3', fire2, s1x1=16, e1x1=64, e3x3=64)
        # pool3 = MaxPool2D(
        #     pool_size=(3, 3), strides=(2, 2), padding='SAME', name='pool3')(fire3)

        # fire4 = self._fire_layer(
        #     'fire4', pool3, s1x1=32, e1x1=128, e3x3=128)
        # fire5 = self._fire_layer(
        #     'fire5', fire4, s1x1=32, e1x1=128, e3x3=128)

        # pool5 = MaxPool2D(pool_size=(3, 3), strides=(2, 2), padding='SAME', name="pool5")(fire5)

        # fire6 = self._fire_layer(
        #     'fire6', pool5, s1x1=48, e1x1=192, e3x3=192)
        # fire7 = self._fire_layer(
        #     'fire7', fire6, s1x1=48, e1x1=192, e3x3=192)
        # fire8 = self._fire_layer(
        #     'fire8', fire7, s1x1=64, e1x1=256, e3x3=256)
        # fire9 = self._fire_layer(
        #     'fire9', fire8, s1x1=64, e1x1=256, e3x3=256)

        # # Two extra fire modules that are not trained before
        # fire10 = self._fire_layer(
        #     'fire10', fire9, s1x1=96, e1x1=384, e3x3=384)
        # fire11 = self._fire_layer(
        #     'fire11', fire10, s1x1=96, e1x1=384, e3x3=384)


        # dropout11 = Dropout( rate=self.config.KEEP_PROB, name='drop11')(fire11)


        # #compute the number of output nodes from number of anchors, classes, confidence score and bounding box corners
        # num_output = self.config.ANCHOR_PER_GRID * (self.config.CLASSES + 1 + 4)

        # preds = Conv2D(
        #     name='conv12', filters=num_output, kernel_size=(3, 3), strides=(1, 1), activation=None, padding="SAME",
        #     use_bias=True, kernel_initializer=TruncatedNormal(stddev=0.001),
        #     kernel_regularizer=l2(self.config.WEIGHT_DECAY))(dropout11)


        
        # #reshape
        # pred_reshaped = Reshape((self.config.ANCHORS, -1))(preds)

        # #pad for loss function so y_pred and y_true have the same dimensions, wrap in lambda layer
        # pred_padded = Lambda(self._pad)( pred_reshaped)

        # model = Model(inputs=input_layer, outputs=pred_padded)

        # print(model.summary())

        # return model


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








CONFIG = "squeeze.config"

#create config object
cfg = load_dict(CONFIG)

SqueezeDet = SqueezeDet(cfg)

#dummy optimizer for compilation
sgd = optimizers.SGD(lr=0.01, decay=0, momentum=0.9, nesterov=False, clipnorm=1.0)

SqueezeDet.model.compile(optimizer=sgd, loss=[SqueezeDet.loss])

#print("Los pesos son: \n", ( SqueezeDet.model.get_weights()[0].shape ) )
#print("Los pesos son: \n", ( SqueezeDet.model.get_weights()[0] ) )
#print("Las bias son: \n", ( SqueezeDet.model.get_weights()[1] ) )

#for i, weights in enumerate( SqueezeDet.model.get_weights() ):
#    print("i y pesos son: \n", i,weights)
#    print("pesos forma: \n", weights.shape)

kernel_conv1_load=[]

kernel_conv1 =  np.array ([     [ [[1, 1], [1, 1], [1, 1]], [[1, 1], [1, 1], [1, 1]], [[1, 1], [1, 1], [1, 1]]  ],
                                [ [[2, 2], [2, 2], [2, 2]], [[2, 2], [2, 2], [2, 2]], [[2, 2], [2, 2], [2, 2]]  ],
                                [ [[3, 3], [3, 3], [3, 3]], [[3, 3], [3, 3], [3, 3]], [[3, 3], [3, 3], [3, 3]]  ]
                                                                                                                    ]) 

bias_conv1 = np.array ([2, 3])

#kernel_conv1 = np.reshape(kernel_conv1, [3, 3, 1, 2])
print("kernel_conv1: \n", kernel_conv1.shape)


kernel_conv1_load.append(kernel_conv1)
kernel_conv1_load.append(bias_conv1)

#kernel_conv1_load = np.reshape(kernel_conv1_load, [3, 3, 1, 2])

SqueezeDet.model.layers[1].set_weights(kernel_conv1_load)

####################################################################
# Input
####################################################################
#in_image  = np.array ([ [ 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 ],
#                        [ 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0 ],
#                        [ 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0 ],
#                        [ 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0 ],
#                        [ 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0 ],
#                        [ 6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0 ],
#                        [ 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0 ],  ])

in_image = cv2.imread("test.png",flags=cv2.IMREAD_UNCHANGED).astype(np.float32, copy=False)
#in_image = cv2.cvtColor(in_image, cv2.COLOR_BGRA2RGBA)
#in_image = cv2.imread("test.png").astype(np.float32, copy=False)
in_image = cv2.cvtColor(in_image, cv2.COLOR_BGRA2RGB)
#print("in_image shape = \n", in_image[0][0])
#print("in_image shape = \n", in_image[0][1])
#print("in_image shape = \n", in_image[0][2])
#print("in_image shape = \n", in_image.shape)
print("in_image  = \n", in_image)
in_image = np.reshape(in_image, [1, 70, 70, 3])

intermediate_layer_model = Model(inputs=SqueezeDet.model.input, outputs=SqueezeDet.model.get_layer("pool1").output)
intermediate_output = intermediate_layer_model.predict( in_image )
print( " shape = \n", intermediate_output.shape )
print( " output = \n", intermediate_output )
#cv2.imwrite('cv2_conv1_python.png',intermediate_output)










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
