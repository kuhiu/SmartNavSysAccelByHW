import numpy as np

def softmax(x, axis=-1):
    """Compute softmax values for each sets of scores in x."""

    e_x = np.exp(x - np.max(x))
    #print("e_x \n", e_x )
    #print("wtf is this\n", np.sum(e_x,axis=axis) )

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
    y_pred = y_pred[:, :, 0:n_outputs]      # Esto quita el padding
    y_pred = np.reshape(y_pred, (1, 15, 20, -1))    # Esto es la salida del conv12 1x15x20x54
    #print("y_prednout \n",y_pred.shape)

    # number of class probabilities, n classes for each anchor

    num_class_probs = 9 * 1

    #print("a que le aplico softmax\n", np.reshape( y_pred[:, :, 4], [-1, 1]) )    # 1,2700,1
    #print("a que le aplico softmax\n", np.reshape( y_pred[:, :, 5], [-1, 1]) )    # 1,2700,1
    #print("y_pred \n", y_pred[:, :, :, :num_class_probs].shape)
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

    #r = open("./softmax" + ".txt", "w")
    #r.write( str( pred_class_probs ) )
    #r.close()

    # number of confidence scores, one for each anchor + class probs
    num_confidence_scores = 9 + num_class_probs

    # slice the confidence scores and put them trough a sigmoid for probabilities
    #print("a que le aplico sigmoid \n",y_pred[:, :, :, num_class_probs:num_confidence_scores])
    #r = open("./sigmoid" + ".txt", "w")
    #r.write( str( np.reshape( y_pred[:, :, :, num_class_probs:num_confidence_scores], [1, 2700] ) ) )
    #r.close()

    pred_conf = sigmoid(
        np.reshape(
            y_pred[:, :, :, num_class_probs:num_confidence_scores],
            [1, 2700]
        )
    )

    #r = open("./sigmoid" + ".txt", "w")
    #r.write( str(  pred_conf ) )
    #r.close()

    # slice remaining bounding box_deltas
    #r = open("./pred_box_delta" + ".txt", "w")
    #r.write( str( np.reshape( y_pred[:, :, :, num_confidence_scores:], [1, 2700, 4] ) ) )
    #r.close()

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
    #print(" delta:  \n", delta_x[0][2114] )
    #print(" delta:  \n", delta_y[0][2114] )
    #print(" delta:  \n", delta_w[0][2114] )
    #print(" delta:  \n", delta_h[0][2114] )

    # get the coordinates and sizes of the anchor boxes from config
    #print("config.ANCHOR_BOX \n", config.ANCHOR_BOX.shape) # 2700, 4
    
    #r = open("./files_fromTrain/ANCHOR_BOX" + ".txt", "w")
    #l = config.ANCHOR_BOX[:].flatten()
    #r.write(str( config.ANCHOR_BOX.shape ) + '\n')
    #for item in l:
    #    r.write(str('{:f}'.format(item)) + '\n')
    #r.close()

    anchor_x = config.ANCHOR_BOX[:, 0]
    anchor_y = config.ANCHOR_BOX[:, 1]
    anchor_w = config.ANCHOR_BOX[:, 2]
    anchor_h = config.ANCHOR_BOX[:, 3]

    #print(" anchor_x:  \n", anchor_x[2114] )
    # as we only predict the deltas, we need to transform the anchor box values before computing the loss

    box_center_x = anchor_x + delta_x * anchor_w
    box_center_y = anchor_y + delta_y * anchor_h
    box_width = anchor_w * safe_exp_np(delta_w, config.EXP_THRESH)
    box_height = anchor_h * safe_exp_np(delta_h, config.EXP_THRESH)
    #print("box \n", box_center_x[0][2114], box_center_y[0][2114], box_width[0][2114], box_height[0][2114])
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