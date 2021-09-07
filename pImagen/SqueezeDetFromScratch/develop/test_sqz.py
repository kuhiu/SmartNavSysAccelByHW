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
import h5py
import sys

#np.set_printoptions(threshold=sys.maxsize)
#np.set_printoptions(threshold=10)

filename = "model.50-3.32.hdf5"
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


        # input_layer = Input(shape=( 240, 320, 3), name="input")

        # conv1 = Conv2D(filters=64, kernel_size=(3, 3), strides=(2, 2), padding="SAME", activation='relu',
        #                 use_bias=True,  name="conv1")(input_layer)


        # pool1 = MaxPool2D(pool_size=(3,3), strides=(2, 2), padding='SAME', name="pool1")(conv1)

        # fire2 = self._fire_layer(name="fire2", input = pool1, s1x1=16, e1x1=64, e3x3=64)
        # fire3 = self._fire_layer(name='fire3', input = fire2, s1x1=16, e1x1=64, e3x3=64)

        # pool3 = MaxPool2D(pool_size=(3, 3), strides=(2, 2), padding='SAME', name='pool3')(fire3)

        # fire4 = self._fire_layer('fire4', pool3, s1x1=32, e1x1=128, e3x3=128)
        # fire5 = self._fire_layer('fire5', fire4, s1x1=32, e1x1=128, e3x3=128)

        # pool5 = MaxPool2D(pool_size=(3, 3), strides=(2, 2), padding='SAME', name="pool5")(fire5)

        # fire6 = self._fire_layer('fire6', pool5, s1x1=48, e1x1=192, e3x3=192)
        # fire7 = self._fire_layer('fire7', fire6, s1x1=48, e1x1=192, e3x3=192)
        # fire8 = self._fire_layer('fire8', fire7, s1x1=64, e1x1=256, e3x3=256)
        # fire9 = self._fire_layer('fire9', fire8, s1x1=64, e1x1=256, e3x3=256)

        # # Two extra fire modules that are not trained before
        # fire10 = self._fire_layer('fire10', fire9, s1x1=96, e1x1=384, e3x3=384)
        # fire11 = self._fire_layer('fire11', fire10, s1x1=96, e1x1=384, e3x3=384)

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

    def _create_model(self):
        """
        #builds the Keras model from config
        #return: squeezeDet in Keras
        """
        input_layer = Input(shape=( self.config.IMAGE_HEIGHT, self.config.IMAGE_WIDTH, self.config.N_CHANNELS), name="input")

        conv1 = Conv2D(filters=64, kernel_size=(3, 3), strides=(2, 2), padding="SAME", activation='relu',
                       name="conv1",use_bias=True, kernel_initializer=TruncatedNormal(stddev=0.001),
                       kernel_regularizer=l2(self.config.WEIGHT_DECAY))(input_layer)


        pool1 = MaxPool2D(pool_size=(3,3), strides=(2, 2), padding='SAME', name="pool1")(conv1)


        fire2 = self._fire_layer(name="fire2", input = pool1, s1x1=16, e1x1=64, e3x3=64)


        fire3 = self._fire_layer(
            'fire3', fire2, s1x1=16, e1x1=64, e3x3=64)
        pool3 = MaxPool2D(
            pool_size=(3, 3), strides=(2, 2), padding='SAME', name='pool3')(fire3)

        fire4 = self._fire_layer(
            'fire4', pool3, s1x1=32, e1x1=128, e3x3=128)
        fire5 = self._fire_layer(
            'fire5', fire4, s1x1=32, e1x1=128, e3x3=128)

        pool5 = MaxPool2D(pool_size=(3, 3), strides=(2, 2), padding='SAME', name="pool5")(fire5)

        fire6 = self._fire_layer(
            'fire6', pool5, s1x1=48, e1x1=192, e3x3=192)
        fire7 = self._fire_layer(
            'fire7', fire6, s1x1=48, e1x1=192, e3x3=192)
        fire8 = self._fire_layer(
            'fire8', fire7, s1x1=64, e1x1=256, e3x3=256)
        fire9 = self._fire_layer(
            'fire9', fire8, s1x1=64, e1x1=256, e3x3=256)

        # Two extra fire modules that are not trained before
        fire10 = self._fire_layer(
            'fire10', fire9, s1x1=96, e1x1=384, e3x3=384)
        fire11 = self._fire_layer(
            'fire11', fire10, s1x1=96, e1x1=384, e3x3=384)


        dropout11 = Dropout( rate=self.config.KEEP_PROB, name='drop11')(fire11)


        #compute the number of output nodes from number of anchors, classes, confidence score and bounding box corners
        num_output = self.config.ANCHOR_PER_GRID * (self.config.CLASSES + 1 + 4)

        preds = Conv2D(
            name='conv12', filters=num_output, kernel_size=(3, 3), strides=(1, 1), activation=None, padding="SAME",
            use_bias=True, kernel_initializer=TruncatedNormal(stddev=0.001),
            kernel_regularizer=l2(self.config.WEIGHT_DECAY))(dropout11)


        
        #reshape
        pred_reshaped = Reshape((self.config.ANCHORS, -1))(preds)

        #pad for loss function so y_pred and y_true have the same dimensions, wrap in lambda layer
        pred_padded = Lambda(self._pad)( pred_reshaped)

        model = Model(inputs=input_layer, outputs=pred_padded)

        print(model.summary())

        return model

    def _fire_layer(self, name, input, s1x1, e1x1, e3x3, stdd=0.01):
            """
            wrapper for fire layer constructions
            :param name: name for layer
            :param input: previous layer
            :param s1x1: number of filters for squeezing
            :param e1x1: number of filter for expand 1x1
            :param e3x3: number of filter for expand 3x3
            :param stdd: standard deviation used for intialization
            :return: a keras fire layer
            """

            sq1x1 = Conv2D(
                name = name + '/squeeze1x1', filters=s1x1, kernel_size=(1, 1), strides=(1, 1), use_bias=True,
                padding='SAME', kernel_initializer=TruncatedNormal(stddev=stdd), activation="relu",
                kernel_regularizer=l2(self.config.WEIGHT_DECAY))(input)

            ex1x1 = Conv2D(
                name = name + '/expand1x1', filters=e1x1, kernel_size=(1, 1), strides=(1, 1), use_bias=True,
                padding='SAME',  kernel_initializer=TruncatedNormal(stddev=stdd), activation="relu",
                kernel_regularizer=l2(self.config.WEIGHT_DECAY))(sq1x1)

            ex3x3 = Conv2D(
                name = name + '/expand3x3',  filters=e3x3, kernel_size=(3, 3), strides=(1, 1), use_bias=True,
                padding='SAME', kernel_initializer=TruncatedNormal(stddev=stdd), activation="relu",
                kernel_regularizer=l2(self.config.WEIGHT_DECAY))(sq1x1)

            return concatenate([ex1x1, ex3x3], axis=3)

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

######################################################################################################################

f = h5py.File(filename, "r")

bias = []
weights = []

try:
    if len(f.items())==0:
        print("ARCHIVO VACIO\n") 

    for layer, g in f.items():
        #for key, value in g.attrs.items():
        #    print("      {}: {}".format(key, value))

        for p_name in g.keys():
            param = g[p_name]
            subkeys = param.keys()
            for k_name in param.keys():
                param2 = param[k_name]
                if ( (k_name == "fire2") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire2_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire2_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire2_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire2_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire2_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire2_e33 = param3.get(key2)[:]
                                r.close()
                        #print("         {}/{}: ".format(fire_name, param2.get(fire_name).keys() ))
                if ( (k_name == "fire3") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire3_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire3_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire3_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire3_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire3_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire3_e33 = param3.get(key2)[:]
                                r.close()
                        #print("         {}/{}: ".format(fire_name, param2.get(fire_name).keys() ))
                if ( (k_name == "fire4") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire4_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire4_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire4_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire4_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire4_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire4_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire5") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire5_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire5_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire5_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire5_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire5_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire5_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire6") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire6_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire6_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire6_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire6_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire6_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire6_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire7") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire7_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire7_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire7_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire7_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire7_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire7_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire8") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire8_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire8_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire8_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire8_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire8_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire8_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire9") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire9_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire9_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire9_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire9_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire9_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire9_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire10") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire10_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire10_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire10_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire10_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire10_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire10_e33 = param3.get(key2)[:]
                                r.close()
                if ( (k_name == "fire11") ):
                    for fire_name in param2.keys():
                        param3 = param2[fire_name]
                        if ( fire_name == "squeeze1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire11_s11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire11_s11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand1x1"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire11_e11 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire11_e11 = param3.get(key2)[:]
                                r.close()
                        elif ( fire_name == "expand3x3"):
                            for key2 in param3.keys():
                                r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                if ( key2 == "kernel:0" ):
                                    kernel_fire11_e33 = param3.get(key2)[:] 
                                if ( key2 == "bias:0" ):
                                    bias_fire11_e33 = param3.get(key2)[:]
                                r.close()

                elif ( p_name == "conv2d_1" ):
                    r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + p_name + ":" + k_name + ".txt", "r")
                    if ( k_name == "kernel:0" ):
                        kernel_conv1 = param.get(k_name)[:] 
                    if ( k_name == "bias:0" ):
                        bias_conv1 = param.get(k_name)[:]
                    r.close()

                elif ( p_name == "conv12" ):
                    r = open("../../SqueezeNet/squeezedet-keras-master/main/model/parametros/" + p_name + ":" + k_name + ".txt", "r")
                    if ( k_name == "kernel:0" ):
                        kernel_conv12 = param.get(k_name)[:] 
                    if ( k_name == "bias:0" ):
                        bias_conv12 = param.get(k_name)[:]
                    r.close()
finally:
    f.close()


######################################################################################################################
kernel_conv1_load=[]
kernel_conv12_load=[]

kernel_fire2_load_s11=[]
kernel_fire2_load_e11=[]
kernel_fire2_load_e33=[]

kernel_fire3_load_s11=[]
kernel_fire3_load_e11=[]
kernel_fire3_load_e33=[]

kernel_fire4_load_s11=[]
kernel_fire4_load_e11=[]
kernel_fire4_load_e33=[]

kernel_fire5_load_s11=[]
kernel_fire5_load_e11=[]
kernel_fire5_load_e33=[]

kernel_fire6_load_s11=[]
kernel_fire6_load_e11=[]
kernel_fire6_load_e33=[]

kernel_fire7_load_s11=[]
kernel_fire7_load_e11=[]
kernel_fire7_load_e33=[]

kernel_fire8_load_s11=[]
kernel_fire8_load_e11=[]
kernel_fire8_load_e33=[]

kernel_fire9_load_s11=[]
kernel_fire9_load_e11=[]
kernel_fire9_load_e33=[]

kernel_fire10_load_s11=[]
kernel_fire10_load_e11=[]
kernel_fire10_load_e33=[]

kernel_fire11_load_s11=[]
kernel_fire11_load_e11=[]
kernel_fire11_load_e33=[]
######################################################################################################################
kernel_conv1_load.append(kernel_conv1)              #print("kernel_conv1  \n", kernel_conv1)
kernel_conv1_load.append(bias_conv1)                #print("bias_conv1 \n", bias_conv1)
######################################################################################################################
kernel_fire2_load_s11.append(kernel_fire2_s11)      #print("kernel_fire2_s11  \n", kernel_fire2_s11)
kernel_fire2_load_s11.append(bias_fire2_s11)        #print("bias_fire2_s11 \n", bias_fire2_s11)
kernel_fire2_load_e11.append(kernel_fire2_e11)      #print("kernel_fire2_e11  \n", kernel_fire2_e11)
kernel_fire2_load_e11.append(bias_fire2_e11)        #print("bias_fire2_e11 \n", bias_fire2_e11)
kernel_fire2_load_e33.append(kernel_fire2_e33)      #print("kernel_fire2_e33  \n", kernel_fire2_e33)
kernel_fire2_load_e33.append(bias_fire2_e33)        #print("bias_fire2_e33 \n", bias_fire2_e33)
######################################################################################################################
kernel_fire3_load_s11.append(kernel_fire3_s11)
kernel_fire3_load_s11.append(bias_fire3_s11)
kernel_fire3_load_e11.append(kernel_fire3_e11)
kernel_fire3_load_e11.append(bias_fire3_e11)
kernel_fire3_load_e33.append(kernel_fire3_e33)
kernel_fire3_load_e33.append(bias_fire3_e33)
######################################################################################################################
kernel_fire4_load_s11.append(kernel_fire4_s11)
kernel_fire4_load_s11.append(bias_fire4_s11)
kernel_fire4_load_e11.append(kernel_fire4_e11)
kernel_fire4_load_e11.append(bias_fire4_e11)
kernel_fire4_load_e33.append(kernel_fire4_e33)
kernel_fire4_load_e33.append(bias_fire4_e33)
######################################################################################################################
kernel_fire5_load_s11.append(kernel_fire5_s11)
kernel_fire5_load_s11.append(bias_fire5_s11)
kernel_fire5_load_e11.append(kernel_fire5_e11)
kernel_fire5_load_e11.append(bias_fire5_e11)
kernel_fire5_load_e33.append(kernel_fire5_e33)
kernel_fire5_load_e33.append(bias_fire5_e33)
######################################################################################################################
kernel_fire6_load_s11.append(kernel_fire6_s11)
kernel_fire6_load_s11.append(bias_fire6_s11)
kernel_fire6_load_e11.append(kernel_fire6_e11)
kernel_fire6_load_e11.append(bias_fire6_e11)
kernel_fire6_load_e33.append(kernel_fire6_e33)
kernel_fire6_load_e33.append(bias_fire6_e33)
######################################################################################################################
kernel_fire7_load_s11.append(kernel_fire7_s11)
kernel_fire7_load_s11.append(bias_fire7_s11)
kernel_fire7_load_e11.append(kernel_fire7_e11)
kernel_fire7_load_e11.append(bias_fire7_e11)
kernel_fire7_load_e33.append(kernel_fire7_e33)
kernel_fire7_load_e33.append(bias_fire7_e33)
######################################################################################################################
kernel_fire8_load_s11.append(kernel_fire8_s11)
kernel_fire8_load_s11.append(bias_fire8_s11)
kernel_fire8_load_e11.append(kernel_fire8_e11)
kernel_fire8_load_e11.append(bias_fire8_e11)
kernel_fire8_load_e33.append(kernel_fire8_e33)
kernel_fire8_load_e33.append(bias_fire8_e33)
######################################################################################################################
kernel_fire9_load_s11.append(kernel_fire9_s11)
kernel_fire9_load_s11.append(bias_fire9_s11)
kernel_fire9_load_e11.append(kernel_fire9_e11)
kernel_fire9_load_e11.append(bias_fire9_e11)
kernel_fire9_load_e33.append(kernel_fire9_e33)
kernel_fire9_load_e33.append(bias_fire9_e33)
######################################################################################################################
kernel_fire10_load_s11.append(kernel_fire10_s11)
kernel_fire10_load_s11.append(bias_fire10_s11)
kernel_fire10_load_e11.append(kernel_fire10_e11)
kernel_fire10_load_e11.append(bias_fire10_e11)
kernel_fire10_load_e33.append(kernel_fire10_e33)
kernel_fire10_load_e33.append(bias_fire10_e33)
######################################################################################################################
kernel_fire11_load_s11.append(kernel_fire11_s11)
kernel_fire11_load_s11.append(bias_fire11_s11)
kernel_fire11_load_e11.append(kernel_fire11_e11)
kernel_fire11_load_e11.append(bias_fire11_e11)
kernel_fire11_load_e33.append(kernel_fire11_e33)
kernel_fire11_load_e33.append(bias_fire11_e33)
######################################################################################################################
kernel_conv12_load.append(kernel_conv12)              #print("kernel_conv1  \n", kernel_conv1)
kernel_conv12_load.append(bias_conv12)                #print("bias_conv1 \n", bias_conv1)
######################################################################################################################
SqueezeDet.model.get_layer("conv1").set_weights(kernel_conv1_load)

SqueezeDet.model.get_layer("fire2/squeeze1x1").set_weights(kernel_fire2_load_s11)
SqueezeDet.model.get_layer("fire2/expand1x1").set_weights(kernel_fire2_load_e11)
SqueezeDet.model.get_layer("fire2/expand3x3").set_weights(kernel_fire2_load_e33)

SqueezeDet.model.get_layer("fire3/squeeze1x1").set_weights(kernel_fire3_load_s11)
SqueezeDet.model.get_layer("fire3/expand1x1").set_weights(kernel_fire3_load_e11)
SqueezeDet.model.get_layer("fire3/expand3x3").set_weights(kernel_fire3_load_e33)

SqueezeDet.model.get_layer("fire4/squeeze1x1").set_weights(kernel_fire4_load_s11)
SqueezeDet.model.get_layer("fire4/expand1x1").set_weights(kernel_fire4_load_e11)
SqueezeDet.model.get_layer("fire4/expand3x3").set_weights(kernel_fire4_load_e33)

SqueezeDet.model.get_layer("fire5/squeeze1x1").set_weights(kernel_fire5_load_s11)
SqueezeDet.model.get_layer("fire5/expand1x1").set_weights(kernel_fire5_load_e11)
SqueezeDet.model.get_layer("fire5/expand3x3").set_weights(kernel_fire5_load_e33)

SqueezeDet.model.get_layer("fire6/squeeze1x1").set_weights(kernel_fire6_load_s11)
SqueezeDet.model.get_layer("fire6/expand1x1").set_weights(kernel_fire6_load_e11)
SqueezeDet.model.get_layer("fire6/expand3x3").set_weights(kernel_fire6_load_e33)

SqueezeDet.model.get_layer("fire7/squeeze1x1").set_weights(kernel_fire7_load_s11)
SqueezeDet.model.get_layer("fire7/expand1x1").set_weights(kernel_fire7_load_e11)
SqueezeDet.model.get_layer("fire7/expand3x3").set_weights(kernel_fire7_load_e33)

SqueezeDet.model.get_layer("fire8/squeeze1x1").set_weights(kernel_fire8_load_s11)
SqueezeDet.model.get_layer("fire8/expand1x1").set_weights(kernel_fire8_load_e11)
SqueezeDet.model.get_layer("fire8/expand3x3").set_weights(kernel_fire8_load_e33)

SqueezeDet.model.get_layer("fire9/squeeze1x1").set_weights(kernel_fire9_load_s11)
SqueezeDet.model.get_layer("fire9/expand1x1").set_weights(kernel_fire9_load_e11)
SqueezeDet.model.get_layer("fire9/expand3x3").set_weights(kernel_fire9_load_e33)

SqueezeDet.model.get_layer("fire10/squeeze1x1").set_weights(kernel_fire10_load_s11)
SqueezeDet.model.get_layer("fire10/expand1x1").set_weights(kernel_fire10_load_e11)
SqueezeDet.model.get_layer("fire10/expand3x3").set_weights(kernel_fire10_load_e33)

SqueezeDet.model.get_layer("fire11/squeeze1x1").set_weights(kernel_fire11_load_s11)
SqueezeDet.model.get_layer("fire11/expand1x1").set_weights(kernel_fire11_load_e11)
SqueezeDet.model.get_layer("fire11/expand3x3").set_weights(kernel_fire11_load_e33)

SqueezeDet.model.get_layer("conv12").set_weights(kernel_conv12_load)
######################################################################################################################



def softmax(x, axis=-1):
    """Compute softmax values for each sets of scores in x."""

    e_x = np.exp(x - np.max(x))
    return e_x / np.expand_dims(np.sum(e_x,axis=axis), axis=axis)


def sigmoid(x):
    """Sigmoid function
    
    Arguments:
        x {[type]} -- input
    
    Returns:
        [type] -- sigmoid(x)
    """


    return 1/(1+np.exp(-x))


def slice_predictions_np(y_pred):
    """
    does the same as above, only uses numpy
    :param y_pred: network output
    :param config: config file
    :return: unpadded and sliced predictions
    """

    # calculate non padded entries
    n_outputs = 1 + 1 + 4
    # slice and reshape network output
    y_pred = y_pred[:, :, 0:n_outputs]
    y_pred = np.reshape(y_pred, (1, 15, 20, -1))

    # number of class probabilities, n classes for each anchor

    num_class_probs = 9 * 1

    # slice pred tensor to extract class pred scores and then normalize them
    pred_class_probs = np.reshape(
        softmax(
            np.reshape(
                y_pred[:, :, :, :num_class_probs],
                [-1, 1]
            )
        ),
        [1, 2700, 1],
    )

    # number of confidence scores, one for each anchor + class probs
    num_confidence_scores = 9 + num_class_probs

    # slice the confidence scores and put them trough a sigmoid for probabilities
    pred_conf = sigmoid(
        np.reshape(
            y_pred[:, :, :, num_class_probs:num_confidence_scores],
            [1, 2700]
        )
    )

    # slice remaining bounding box_deltas
    pred_box_delta = np.reshape(
        y_pred[:, :, :, num_confidence_scores:],
        [1, 2700, 4]
    )

    return [pred_class_probs, pred_conf, pred_box_delta]

def safe_exp_np(w, thresh):
  """Safe exponential function for numpy tensors."""

  slope = np.exp(thresh)
  lin_bool = w > thresh

  lin_region = lin_bool.astype(float)

  lin_out = slope*(w - thresh + 1.)

  exp_out = np.exp(np.where(lin_bool, np.zeros_like(w), w))

  out = lin_region*lin_out + (1.-lin_region)*exp_out

  return out

def bbox_transform(bbox):
    """convert a bbox of form [cx, cy, w, h] to [xmin, ymin, xmax, ymax]. Works
    for numpy array or list of tensors.
    """
    cx, cy, w, h = bbox
    out_box = [[]]*4
    out_box[0] = cx-w/2
    out_box[1] = cy-h/2
    out_box[2] = cx+w/2
    out_box[3] = cy+h/2

    return out_box

def bbox_transform_inv(bbox):
    """convert a bbox of form [xmin, ymin, xmax, ymax] to [cx, cy, w, h]. Works
    for numpy array or list of tensors.
    """
    xmin, ymin, xmax, ymax = bbox
    out_box = [[]]*4

    width       = xmax - xmin + 1.0
    height      = ymax - ymin + 1.0
    out_box[0]  = xmin + 0.5*width
    out_box[1]  = ymin + 0.5*height
    out_box[2]  = width
    out_box[3]  = height

    return out_box

def boxes_from_deltas_np(pred_box_delta, config):

    """
    Converts prediction deltas to bounding boxes, but in numpy
    
    Arguments:
        pred_box_delta {[type]} -- tensor of deltas
        config {[type]} -- hyperparameter dict
    
    Returns:
        [type] -- tensor of bounding boxes
    """


    # Keras backend allows no unstacking

    delta_x = pred_box_delta[:, :, 0]
    delta_y = pred_box_delta[:, :, 1]
    delta_w = pred_box_delta[:, :, 2]
    delta_h = pred_box_delta[:, :, 3]

    # get the coordinates and sizes of the anchor boxes from config

    anchor_x = config.ANCHOR_BOX[:, 0]
    anchor_y = config.ANCHOR_BOX[:, 1]
    anchor_w = config.ANCHOR_BOX[:, 2]
    anchor_h = config.ANCHOR_BOX[:, 3]

    # as we only predict the deltas, we need to transform the anchor box values before computing the loss

    box_center_x = anchor_x + delta_x * anchor_w
    box_center_y = anchor_y + delta_y * anchor_h
    box_width = anchor_w * safe_exp_np(delta_w, config.EXP_THRESH)
    box_height = anchor_h * safe_exp_np(delta_h, config.EXP_THRESH)

    # tranform into a real box with four coordinates

    xmins, ymins, xmaxs, ymaxs = bbox_transform([box_center_x, box_center_y, box_width, box_height])

    # trim boxes if predicted outside

    xmins = np.minimum(
        np.maximum(0.0, xmins), config.IMAGE_WIDTH - 1.0)
    ymins = np.minimum(
        np.maximum(0.0, ymins), config.IMAGE_HEIGHT - 1.0)
    xmaxs = np.maximum(
        np.minimum(config.IMAGE_WIDTH - 1.0, xmaxs), 0.0)
    ymaxs = np.maximum(
        np.minimum(config.IMAGE_HEIGHT - 1.0, ymaxs), 0.0)

    det_boxes = np.transpose(
        np.stack(([xmins, ymins, xmaxs, ymaxs])),
        (1, 2, 0)
    )

    return (det_boxes)
    
######################################################################################################################
# Input
######################################################################################################################
in_image = cv2.imread("./sqzdet/test5.png",flags=cv2.IMREAD_UNCHANGED).astype(np.float32, copy=False)
in_image = cv2.cvtColor(in_image, cv2.COLOR_BGRA2RGB)

print("in_image shape  = \n", in_image.shape)
in_image = np.reshape(in_image, [1, 240, 320, 3])
#print("in_image  = \n", in_image)
######################################################################################################################
# Output
######################################################################################################################

intermediate_layer_model = Model(inputs=SqueezeDet.model.input, outputs=SqueezeDet.model.get_layer("reshape_1").output)
intermediate_output = intermediate_layer_model.predict( in_image )
#print( " reshape_1 shape = \n", intermediate_output.shape )
#print( " reshape_1 output = \n", intermediate_output )
#
#r = open("./reshape" + ".txt", "w")
#r.write(str(intermediate_output))
#r.close()
#
#intermediate_layer_model = Model(inputs=SqueezeDet.model.input, outputs=SqueezeDet.model.get_layer("lambda_1").output)
#intermediate_output = intermediate_layer_model.predict( in_image )

#r = open("./lambda_1" + ".txt", "w")
#r.write(str(intermediate_output))
#r.close()
#y_pred = intermediate_output
#y_pred_proba = intermediate_output[0][...,5]
##print("tesy_predt: ", y_pred )
#
##print( sess.run(sess.run(y_pred)) )
#
#r = open("./y_pred" + ".txt", "w")
#r.write( str( y_pred ))
#r.close()
#
#r = open("./y_pred_proba" + ".txt", "w")
#r.write( str( y_pred_proba ))
#r.close()
#
#r = open("./soft" + ".txt", "w")
#r.write( str( sess.run(K.softmax(y_pred_proba)) ))
#r.close()
#
#print("Testing softmax: ", (softmax( [1.0, 0.0, 0.0, 0.0] )))


[pred_class_probs, pred_conf, pred_box_delta] = slice_predictions_np(intermediate_output)

det_boxes = boxes_from_deltas_np(pred_box_delta, cfg)

##compute class probabilities
probs = pred_class_probs * np.reshape(pred_conf, [1, 2700, 1])

print( " probs output = \n", probs.shape )

det_probs = np.max(probs, 1)
det_class = np.argmax(probs, 1)

print( " det_probs output = \n", det_probs[0][0] )
print( " det_class output = \n", det_class[0][0] )

more_probable = det_class[0][0]

print( " det_boxes output = \n", det_boxes[0][more_probable] )

box_predicted = det_boxes[0][more_probable].astype(int)

in_image = np.reshape(in_image, [240, 320, 3])
in_image = cv2.cvtColor(in_image, cv2.COLOR_RGB2BGR)
cv2.rectangle(in_image, (box_predicted[0], box_predicted[1]) ,(box_predicted[2], box_predicted[3]), (0,0,255), 3)
cv2.imwrite("./sqzdet/test5MODIF.png", in_image)







   
    
#print( " reshape_1 output = \n", intermediate_output[0][219] )
#print( " salida output = \n", intermediate_output[0][48][:, :, 0:6] )

#in_image = np.reshape(in_image, [240, 320, 3])
#in_image = cv2.cvtColor(in_image, cv2.COLOR_RGB2BGR)
#cv2.rectangle(in_image, (136, 153) ,(257, 240), (0,0,255), 3)
#cv2.imwrite("./sqzdet/test5MODIF.png", in_image)
#
#xmax = 257
#xmin = 136
#ymax = 240
#ymin = 153
#
#out_box = [[]]*4
#width       = xmax - xmin + 1.0
#height      = ymax - ymin + 1.0
#out_box[0]  = xmin + 0.5*width
#out_box[1]  = ymin + 0.5*height
#out_box[2]  = width
#out_box[3]  = height
#
#print("outbox \n", out_box[0], out_box[1], out_box[2], out_box[3])
#cv2.rectangle(img, (det_box[0], det_box[1]), (det_box[2], det_box[3]), (0,0,255), 1)
#cv2.putText(img, config.CLASS_NAMES[all_filtered_classes[i][j]] + " " + str(all_filtered_scores[i][j]) , (det_box[0], det_box[1]), font, 0.5, (0,0,255), 1, cv2.LINE_AA)