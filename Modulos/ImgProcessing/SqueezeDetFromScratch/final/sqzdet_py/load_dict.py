import json
import numpy as np
from easydict import EasyDict as edict

#compute the anchors for the grid from the seed
def set_anchors(cfg):
    H, W, B = cfg.ANCHORS_HEIGHT, cfg.ANCHORS_WIDTH, cfg.ANCHOR_PER_GRID

    #print("cfg.ANCHOR_SEED: \n", cfg.ANCHOR_SEED.shape)
    anchor_shapes = np.reshape( [cfg.ANCHOR_SEED] * H * W, (H, W, B, 2) )

    #print("anchor_shapes: \n", anchor_shapes.shape)

    center_x = np.reshape(
        np.transpose(
            np.reshape(
                np.array([np.arange(1, W+1)*float(cfg.IMAGE_WIDTH)/(W)]  *H*B),
                (B, H, W)
            ),
            (1, 2, 0)
        ),
        (H, W, B, 1)
    )
    #print("arr: \n", np.array([np.arange(1, W+1)*float(cfg.IMAGE_WIDTH)/(W)]  *H*B).shape )
    #print("arr2 \n", float(cfg.IMAGE_WIDTH)/(W+1))
    #print("center_x: \n", center_x.shape)

    center_y = np.reshape(
        np.transpose(
            np.reshape(
                np.array([np.arange(1, H+1)*float(cfg.IMAGE_HEIGHT)/(H)]*W*B),
                (B, W, H)
            ),
            (2, 1, 0)
        ),
        (H, W, B, 1)
    )

    #print("center_y: \n", center_y.shape)
    anchors = np.reshape(
        np.concatenate((center_x, center_y, anchor_shapes), axis=3),
        (-1, 4)
    )
    #print("anchors: \n", anchors.shape)
    return anchors, H, W


def load_dict(path):
    """Loads a dictionary from a given path name
    
    Arguments:
        path {[type]} -- string of path
    
    Returns:
        [type] -- [description]
    """

    with open(path, "r") as f:
        cfg = json.load(f)  ### this loads the array from .json format


    #changes lists back
    for key, val, in cfg.items():

        if type(val) is list:
            cfg[key] = np.array(val)

    #cast do easydict
    cfg = edict(cfg)

    #create full anchors from seed
    cfg.ANCHOR_BOX, cfg.N_ANCHORS_HEIGHT, cfg.N_ANCHORS_WIDTH = set_anchors(cfg)
    cfg.ANCHORS = len(cfg.ANCHOR_BOX)

    #if you added a class in the config manually, but were to lazy to update
    cfg.CLASSES = len(cfg.CLASS_NAMES)
    cfg.CLASS_TO_IDX = dict(zip(cfg.CLASS_NAMES, range(cfg.CLASSES)))



    return cfg