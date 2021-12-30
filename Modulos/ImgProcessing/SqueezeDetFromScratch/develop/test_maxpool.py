import numpy as np
from keras.layers import Input, MaxPool2D,  Conv2D, Dropout, concatenate, Reshape, Lambda, AveragePooling2D
import tensorflow as tf

sess = tf.InteractiveSession()

in_image  = tf.constant ([      [ 1, 1, 1, 1, 1, 1, 1 ],
                                [ 2, 2, 2, 2, 2, 2, 2 ],
                                [ 3, 3, 3, 3, 3, 3, 3 ],
                                [ 4, 4, 4, 4, 4, 4, 4 ],
                                [ 5, 5, 5, 5, 5, 5, 5 ],
                                [ 6, 6, 6, 6, 6, 6, 6 ],
                                [ 7, 7, 7, 7, 7, 7, 7 ],    ])

in_image = tf.reshape(in_image, [1, 7, 7, 1])

in_image = tf.convert_to_tensor(in_image)

max_pool_2d = tf.keras.layers.MaxPooling2D(pool_size=(3, 3), strides=(2, 2), padding='valid')

print (sess.run( max_pool_2d(in_image) ))