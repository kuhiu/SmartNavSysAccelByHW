import tensorflow as tf
from keras import optimizers
from load_dict import load_dict
from model import SqueezeDet
import sys
from read_model import read_model
import cv2
import numpy as np
from keras.models import Model
from post_cnn import slice_predictions_np, softmax, sigmoid, safe_exp_np, bbox_transform, bbox_transform_inv, boxes_from_deltas_np

#np.set_printoptions(threshold=sys.maxsize)
filename = "./files_fromTrain/model.50-3.32.hdf5"
CONFIG = "./files_fromTrain/squeeze.config"
sess = tf.InteractiveSession()

#create config object
cfg = load_dict(CONFIG)

#create sqdet model
SqueezeDet = SqueezeDet(cfg)

#dummy optimizer for compilation
sgd = optimizers.SGD(lr=0.01, decay=0, momentum=0.9, nesterov=False, clipnorm=1.0)

#model compiler 
SqueezeDet.model.compile(optimizer=sgd, loss=[SqueezeDet.loss])

#read parameters
[kernel_conv1, bias_conv1, kernel_conv12, bias_conv12, 
 kernel_fire2_s11,   kernel_fire3_s11,   kernel_fire4_s11,   kernel_fire5_s11,   kernel_fire6_s11,   kernel_fire7_s11,   kernel_fire8_s11,   kernel_fire9_s11,   kernel_fire10_s11,   kernel_fire11_s11, 
 kernel_fire2_e11,   kernel_fire3_e11,   kernel_fire4_e11,   kernel_fire5_e11,   kernel_fire6_e11,   kernel_fire7_e11,   kernel_fire8_e11,   kernel_fire9_e11,   kernel_fire10_e11,   kernel_fire11_e11, 
 kernel_fire2_e33,   kernel_fire3_e33,   kernel_fire4_e33,   kernel_fire5_e33,   kernel_fire6_e33,   kernel_fire7_e33,   kernel_fire8_e33,   kernel_fire9_e33,   kernel_fire10_e33,   kernel_fire11_e33, 
    bias_fire2_s11,     bias_fire3_s11,     bias_fire4_s11,     bias_fire5_s11,     bias_fire6_s11,     bias_fire7_s11,     bias_fire8_s11,     bias_fire9_s11,     bias_fire10_s11,     bias_fire11_s11, 
    bias_fire2_e11,     bias_fire3_e11,     bias_fire4_e11,     bias_fire5_e11,     bias_fire6_e11,     bias_fire7_e11,     bias_fire8_e11,     bias_fire9_e11,     bias_fire10_e11,     bias_fire11_e11, 
    bias_fire2_e33,     bias_fire3_e33,     bias_fire4_e33,     bias_fire5_e33,     bias_fire6_e33,     bias_fire7_e33,     bias_fire8_e33,     bias_fire9_e33,     bias_fire10_e33,     bias_fire11_e33
                                                                                                                                                                                                                ]   = read_model(filename)

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
# Input
######################################################################################################################
in_image = cv2.imread("./testing/test5.png",flags=cv2.IMREAD_UNCHANGED).astype(np.float32, copy=False)
in_image = cv2.cvtColor(in_image, cv2.COLOR_BGRA2RGB)
print("in_image shape  = \n", in_image.shape)
in_image = np.reshape(in_image, [1, 240, 320, 3])

######################################################################################################################
# Output
######################################################################################################################

intermediate_layer_model = Model(inputs=SqueezeDet.model.input, outputs=SqueezeDet.model.get_layer("reshape_1").output)
intermediate_output = intermediate_layer_model.predict( in_image )

intermediate_layer_model2 = Model(inputs=SqueezeDet.model.input, outputs=SqueezeDet.model.get_layer("conv1").output)
intermediate_output2 = intermediate_layer_model2.predict( in_image )

print("intermediate_output2\n", intermediate_output2)
#r = open("./intermediate_output2" + ".txt", "w")
#r.write( str( intermediate_output2 ) )
#r.close()



[pred_class_probs, pred_conf, pred_box_delta] = slice_predictions_np(intermediate_output)

#print( " pred_class_probs = \n", pred_class_probs )

det_boxes = boxes_from_deltas_np(pred_box_delta, cfg)

##compute class probabilities
#print( " pred_conf output = \n", pred_conf )
probs = pred_class_probs * np.reshape(pred_conf, [1, 2700, 1])

#print( " probs output = \n", probs )
r = open("./probs" + ".txt", "w")
r.write( str(  probs ) )
r.close()

det_probs = np.max(probs, 1)
det_class = np.argmax(probs, 1)

print( " det_probs output = \n", det_probs[0][0] )
print( " det_class output = \n", det_class[0][0] )

more_probable = det_class[0][0]

print( " det_boxes output = \n", det_boxes[0] )

box_predicted = det_boxes[0][more_probable].astype(int)

print("box_predicted: \n", box_predicted[0], box_predicted[1], box_predicted[2], box_predicted[3])

in_image = np.reshape(in_image, [240, 320, 3])
in_image = cv2.cvtColor(in_image, cv2.COLOR_RGB2BGR)
cv2.rectangle(in_image, (box_predicted[0], box_predicted[1]) ,(box_predicted[2], box_predicted[3]), (0,0,255), 3)
cv2.imwrite("./testing/test5MODIF.png", in_image)