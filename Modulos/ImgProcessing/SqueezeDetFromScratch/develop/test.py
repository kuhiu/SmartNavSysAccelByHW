import numpy as np
from keras.layers import Reshape
import tensorflow as tf

sess = tf.Session() 

a = np.array([ [ [[1,2,3],[4,5,6],[7,8,9]], [[10,10,10],[10,10,10],[10,10,10]] ] ])
a = tf.convert_to_tensor(a)
b = Reshape((9, 2))(a)

print("Vector: \n", a)
print("Vector: \n", b)

print(sess.run(a))
print(sess.run(b))

