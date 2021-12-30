import h5py


def read_model(filename):

    f = h5py.File(filename, "r")

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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire2_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire2_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire2_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire2_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire3_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire3_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire3_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire3_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire4_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire4_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire4_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire4_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire5_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire5_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire5_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire5_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire6_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire6_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire6_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire6_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire7_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire7_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire7_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire7_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire8_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire8_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire8_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire8_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire9_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire9_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire9_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire9_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire10_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire10_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire10_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire10_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
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
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire11_s11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire11_s11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand1x1"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire11_e11 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire11_e11 = param3.get(key2)[:]
                                    r.close()
                            elif ( fire_name == "expand3x3"):
                                for key2 in param3.keys():
                                    r = open("./files_fromTrain/parametros/" + k_name + ":" + fire_name + ":" + key2 + ".txt", "r")
                                    if ( key2 == "kernel:0" ):
                                        kernel_fire11_e33 = param3.get(key2)[:] 
                                    if ( key2 == "bias:0" ):
                                        bias_fire11_e33 = param3.get(key2)[:]
                                    r.close()

                    elif ( p_name == "conv2d_1" ):
                        r = open("./files_fromTrain/parametros/" + p_name + ":" + k_name + ".txt", "r")
                        if ( k_name == "kernel:0" ):
                            kernel_conv1 = param.get(k_name)[:] 
                        if ( k_name == "bias:0" ):
                            bias_conv1 = param.get(k_name)[:]
                        r.close()

                    elif ( p_name == "conv12" ):
                        r = open("./files_fromTrain/parametros/" + p_name + ":" + k_name + ".txt", "r")
                        if ( k_name == "kernel:0" ):
                            kernel_conv12 = param.get(k_name)[:] 
                        if ( k_name == "bias:0" ):
                            bias_conv12 = param.get(k_name)[:]
                        r.close()
    finally:
        f.close()

    return [kernel_conv1, bias_conv1, kernel_conv12, bias_conv12, 
            kernel_fire2_s11,   kernel_fire3_s11,   kernel_fire4_s11,   kernel_fire5_s11,   kernel_fire6_s11,   kernel_fire7_s11,   kernel_fire8_s11,   kernel_fire9_s11,   kernel_fire10_s11,   kernel_fire11_s11, 
            kernel_fire2_e11,   kernel_fire3_e11,   kernel_fire4_e11,   kernel_fire5_e11,   kernel_fire6_e11,   kernel_fire7_e11,   kernel_fire8_e11,   kernel_fire9_e11,   kernel_fire10_e11,   kernel_fire11_e11, 
            kernel_fire2_e33,   kernel_fire3_e33,   kernel_fire4_e33,   kernel_fire5_e33,   kernel_fire6_e33,   kernel_fire7_e33,   kernel_fire8_e33,   kernel_fire9_e33,   kernel_fire10_e33,   kernel_fire11_e33, 
              bias_fire2_s11,     bias_fire3_s11,     bias_fire4_s11,     bias_fire5_s11,     bias_fire6_s11,     bias_fire7_s11,     bias_fire8_s11,     bias_fire9_s11,     bias_fire10_s11,     bias_fire11_s11, 
              bias_fire2_e11,     bias_fire3_e11,     bias_fire4_e11,     bias_fire5_e11,     bias_fire6_e11,     bias_fire7_e11,     bias_fire8_e11,     bias_fire9_e11,     bias_fire10_e11,     bias_fire11_e11, 
              bias_fire2_e33,     bias_fire3_e33,     bias_fire4_e33,     bias_fire5_e33,     bias_fire6_e33,     bias_fire7_e33,     bias_fire8_e33,     bias_fire9_e33,     bias_fire10_e33,     bias_fire11_e33
                                                                                                                                                                                                                        ]