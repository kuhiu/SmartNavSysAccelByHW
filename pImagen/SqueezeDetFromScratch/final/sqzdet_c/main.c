#include "main.h"

// Imgage size
int img_width, img_height;

// Read png lib
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

int main(void)
{    
    double time_spent;
    clock_t begin;

    // Imagen de entrada
    float *img              = NULL;
    float *img_padded       = NULL;

    // Conv1                                         // Conv12  
    float *conv1_kernel     = NULL;                  float *conv12_kernel    = NULL;                                     
    float *conv1_bias       = NULL;                  float *conv12_bias      = NULL;                                                        
    float *conv1_output     = NULL;                  float *conv12_output    = NULL;                                        
    int output_conv1_width, output_conv1_height;     int output_conv12_width, output_conv12_height;                                    
    
    // maxpool
    float *max_pool_1       = NULL;
    int pool_width_1, pool_height_1;
    float *max_pool_3       = NULL;
    int pool_width_3, pool_height_3;
    float *max_pool_5       = NULL;
    int pool_width_5, pool_height_5;

    // fire 2 layer                        // fire 3 layer                        // fire 4 layer       
    float *fire_2               = NULL;    float *fire_3 = NULL;                  float *fire_4               = NULL;
    float *fire_2_kernel_s1x1   = NULL;    float *fire_3_kernel_s1x1   = NULL;    float *fire_4_kernel_s1x1   = NULL;
    float *fire_2_bias_s1x1     = NULL;    float *fire_3_bias_s1x1     = NULL;    float *fire_4_bias_s1x1     = NULL;
    float *fire_2_kernel_e1x1   = NULL;    float *fire_3_kernel_e1x1   = NULL;    float *fire_4_kernel_e1x1   = NULL;
    float *fire_2_bias_e1x1     = NULL;    float *fire_3_bias_e1x1     = NULL;    float *fire_4_bias_e1x1     = NULL;
    float *fire_2_kernel_e3x3   = NULL;    float *fire_3_kernel_e3x3   = NULL;    float *fire_4_kernel_e3x3   = NULL;
    float *fire_2_bias_e3x3     = NULL;    float *fire_3_bias_e3x3     = NULL;    float *fire_4_bias_e3x3     = NULL;

    // fire 5 layer                        // fire 6 layer                        // fire 7 layer                  
    float *fire_5               = NULL;    float *fire_6               = NULL;    float *fire_7               = NULL;
    float *fire_5_kernel_s1x1   = NULL;    float *fire_6_kernel_s1x1   = NULL;    float *fire_7_kernel_s1x1   = NULL;
    float *fire_5_bias_s1x1     = NULL;    float *fire_6_bias_s1x1     = NULL;    float *fire_7_bias_s1x1     = NULL;
    float *fire_5_kernel_e1x1   = NULL;    float *fire_6_kernel_e1x1   = NULL;    float *fire_7_kernel_e1x1   = NULL;
    float *fire_5_bias_e1x1     = NULL;    float *fire_6_bias_e1x1     = NULL;    float *fire_7_bias_e1x1     = NULL;
    float *fire_5_kernel_e3x3   = NULL;    float *fire_6_kernel_e3x3   = NULL;    float *fire_7_kernel_e3x3   = NULL;
    float *fire_5_bias_e3x3     = NULL;    float *fire_6_bias_e3x3     = NULL;    float *fire_7_bias_e3x3     = NULL;

    // fire 8 layer                        // fire 9 layer                        // fire 10 layer  
    float *fire_8               = NULL;    float *fire_9               = NULL;    float *fire_10              = NULL;
    float *fire_8_kernel_s1x1   = NULL;    float *fire_9_kernel_s1x1   = NULL;    float *fire_10_kernel_s1x1  = NULL;
    float *fire_8_bias_s1x1     = NULL;    float *fire_9_bias_s1x1     = NULL;    float *fire_10_bias_s1x1    = NULL;
    float *fire_8_kernel_e1x1   = NULL;    float *fire_9_kernel_e1x1   = NULL;    float *fire_10_kernel_e1x1  = NULL;
    float *fire_8_bias_e1x1     = NULL;    float *fire_9_bias_e1x1     = NULL;    float *fire_10_bias_e1x1    = NULL;
    float *fire_8_kernel_e3x3   = NULL;    float *fire_9_kernel_e3x3   = NULL;    float *fire_10_kernel_e3x3  = NULL;
    float *fire_8_bias_e3x3     = NULL;    float *fire_9_bias_e3x3     = NULL;    float *fire_10_bias_e3x3    = NULL;

    // fire 11 layer                    
    float *fire_11               = NULL;
    float *fire_11_kernel_s1x1   = NULL;
    float *fire_11_bias_s1x1     = NULL;
    float *fire_11_kernel_e1x1   = NULL;
    float *fire_11_bias_e1x1     = NULL;
    float *fire_11_kernel_e3x3   = NULL;
    float *fire_11_bias_e3x3     = NULL;

    // Post-processing cnn
    float *pred_class           = NULL;
    float *pred_conf            = NULL;
    float *pred_class_softmax   = NULL;
    float *pred_conf_sigmod     = NULL;
    float *prob                 = NULL;
    float *box_predicted        = NULL;
    float *anchors              = NULL; 

    float xmax, xmin, ymax, ymin;
    float delta_x, delta_y, delta_w, delta_h, max=0;
    float box_center_x, box_center_y, box_width, box_height;  

    int index_Ofmax=0;            
    int i,j,k;

////////////////////////////////        Leo el header       ////////////////////////////////////////////////////////
    read_png_file("./test5.png");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////       Memoria dinamica     ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    img = (float*) malloc( img_width*img_height*3*sizeof(float) );
////////////////////////////////        Leo la imagen       ////////////////////////////////////////////////////////
    get_png_file(img);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    Cargo los parametros    ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Cargo los parametros\n");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////    Leo txt para kernel y bias conv1    ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv1_kernel = weight_load("./files_fromTrain/parametros/conv2d_1:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv1_bias = bias_load("./files_fromTrain/parametros/conv2d_1:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////    Leo txt para las conv2d y bias de fire_2   ////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_2_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire2:squeeze1x1:kernel:0.txt");
    fire_2_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire2:expand1x1:kernel:0.txt");
    fire_2_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire2:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_2_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire2:squeeze1x1:bias:0.txt");
    fire_2_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire2:expand1x1:bias:0.txt");
    fire_2_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire2:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_3   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_3_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire3:squeeze1x1:kernel:0.txt");
    fire_3_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire3:expand1x1:kernel:0.txt");
    fire_3_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire3:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_3_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire3:squeeze1x1:bias:0.txt");
    fire_3_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire3:expand1x1:bias:0.txt");
    fire_3_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire3:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_4   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_4_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire4:squeeze1x1:kernel:0.txt");
    fire_4_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire4:expand1x1:kernel:0.txt");
    fire_4_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire4:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_4_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire4:squeeze1x1:bias:0.txt");
    fire_4_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire4:expand1x1:bias:0.txt");
    fire_4_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire4:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_5   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_5_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire5:squeeze1x1:kernel:0.txt");
    fire_5_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire5:expand1x1:kernel:0.txt");
    fire_5_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire5:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_5_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire5:squeeze1x1:bias:0.txt");
    fire_5_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire5:expand1x1:bias:0.txt");
    fire_5_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire5:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_6   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_6_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire6:squeeze1x1:kernel:0.txt");
    fire_6_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire6:expand1x1:kernel:0.txt");
    fire_6_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire6:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_6_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire6:squeeze1x1:bias:0.txt");
    fire_6_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire6:expand1x1:bias:0.txt");
    fire_6_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire6:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_7   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_7_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire7:squeeze1x1:kernel:0.txt");
    fire_7_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire7:expand1x1:kernel:0.txt");
    fire_7_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire7:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_7_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire7:squeeze1x1:bias:0.txt");
    fire_7_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire7:expand1x1:bias:0.txt");
    fire_7_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire7:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_8   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_8_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire8:squeeze1x1:kernel:0.txt");
    fire_8_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire8:expand1x1:kernel:0.txt");
    fire_8_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire8:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_8_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire8:squeeze1x1:bias:0.txt");
    fire_8_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire8:expand1x1:bias:0.txt");
    fire_8_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire8:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_9   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_9_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire9:squeeze1x1:kernel:0.txt");
    fire_9_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire9:expand1x1:kernel:0.txt");
    fire_9_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire9:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_9_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire9:squeeze1x1:bias:0.txt");
    fire_9_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire9:expand1x1:bias:0.txt");
    fire_9_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire9:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_10  //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_10_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire10:squeeze1x1:kernel:0.txt");
    fire_10_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire10:expand1x1:kernel:0.txt");
    fire_10_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire10:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_10_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire10:squeeze1x1:bias:0.txt");
    fire_10_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire10:expand1x1:bias:0.txt");
    fire_10_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire10:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_11  //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_11_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire11:squeeze1x1:kernel:0.txt");
    fire_11_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire11:expand1x1:kernel:0.txt");
    fire_11_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire11:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_11_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire11:squeeze1x1:bias:0.txt");
    fire_11_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire11:expand1x1:bias:0.txt");
    fire_11_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire11:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////    Leo txt para kernel y bias conv12    ///////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv12_kernel = weight_load("./files_fromTrain/parametros/conv12:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv12_bias = bias_load("./files_fromTrain/parametros/conv12:bias:0.txt");
//////////////////////////////////////   Time measure  /////////////////////////////////////////////////////////////
    printf("Start to measure\n");
    begin = clock();
    printf("Start to precessing...\n");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////          SqueezeDet        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    img_padded = (float*) calloc( (img_width+1)*(img_height+1)*3 , sizeof(float) );
    padding(    img, img_width, img_height, 3,
                img_padded,
                1, 1); 
    output_conv1_width  = (((IMG_WIDTH  - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1); 
    output_conv1_height = (((IMG_HEIGHT - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1);
    conv1_output = (float*) calloc( output_conv1_width*output_conv1_height*CONV1_FILTERS, sizeof(float) );
    convolucion2d(  img_padded, (img_width+1), (img_height+1), 3,                           // Entrada: pointer, ancho, alto, profundidad
                    conv1_kernel, CONV1_KERNEL_SIZE,                                        // Kernel: pointer, size
                    conv1_bias,                                                             // Bias: pointer
                    2,                                                                      // Stride
                    CONV1_RELU,
                    conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS ); // Salida: pointer, ancho, alto, profundidad
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pool_width_1  = output_conv1_width/2;
    pool_height_1 = output_conv1_height/2;
    max_pool_1 = (float*) calloc( ( pool_width_1*pool_height_1*CONV1_FILTERS ), sizeof(float));
    maxPool2d(  MAXPOOL1_POOL_SIZE, MAXPOOL1_STRIDE, 
                conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS, 
                max_pool_1, pool_width_1, pool_height_1,
                MAXPOOL1_PAD);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_2 = (float*) calloc( ( pool_width_1*pool_height_1*(FIRE2_e1x1+FIRE2_e3x3) ), sizeof(float));
    fire_layer (    max_pool_1, pool_width_1, pool_height_1, CONV1_FILTERS,             // Entrada: pointer, ancho, alto, profundidad
                    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_2_kernel_s1x1, fire_2_bias_s1x1, FIRE2_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_2_kernel_e1x1, fire_2_bias_e1x1, FIRE2_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_2_kernel_e3x3, fire_2_bias_e3x3, FIRE2_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE2_RELU                                          );              // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_3 = (float*) calloc( ( pool_width_1*pool_height_1*(FIRE3_e1x1+FIRE3_e3x3) ), sizeof(float));
    fire_layer (    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_3_kernel_s1x1, fire_3_bias_s1x1, FIRE3_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_3_kernel_e1x1, fire_3_bias_e1x1, FIRE3_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_3_kernel_e3x3, fire_3_bias_e3x3, FIRE3_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE3_RELU                                        );                // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pool_width_3  = pool_width_1/2;
    pool_height_3 = pool_height_1/2;
    max_pool_3 = (float*) calloc( ( pool_width_3*pool_height_3*MAXPOOL3_FILTERS ), sizeof(float));
    maxPool2d(  MAXPOOL3_POOL_SIZE, MAXPOOL3_STRIDE, 
                fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3), 
                max_pool_3, pool_width_3, pool_height_3,
                MAXPOOL3_PAD);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_4 = (float*) calloc( ( pool_width_3*pool_height_3*(FIRE4_e1x1+FIRE4_e3x3) ), sizeof(float));
    fire_layer (    max_pool_3, pool_width_3, pool_height_3, (FIRE2_e1x1+FIRE2_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_4_kernel_s1x1, fire_4_bias_s1x1, FIRE4_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_4_kernel_e1x1, fire_4_bias_e1x1, FIRE4_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_4_kernel_e3x3, fire_4_bias_e3x3, FIRE4_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE4_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_5 = (float*) calloc( ( pool_width_3*pool_height_3*(FIRE5_e1x1+FIRE5_e3x3) ), sizeof(float));
    fire_layer (    fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_5_kernel_s1x1, fire_5_bias_s1x1, FIRE5_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_5_kernel_e1x1, fire_5_bias_e1x1, FIRE5_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_5_kernel_e3x3, fire_5_bias_e3x3, FIRE5_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE5_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pool_width_5  = (pool_width_3)/2;
    pool_height_5 = (pool_height_3)/2;
    max_pool_5 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE5_e1x1+FIRE5_e3x3) ), sizeof(float));
    maxPool2d(  MAXPOOL5_POOL_SIZE, MAXPOOL5_STRIDE, 
                fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3), 
                max_pool_5, pool_width_5, pool_height_5,
                MAXPOOL5_PAD);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_6 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE6_e1x1+FIRE6_e3x3) ), sizeof(float));
    fire_layer (    max_pool_5, pool_width_5, pool_height_5, (FIRE5_e1x1+FIRE5_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_6_kernel_s1x1, fire_6_bias_s1x1, FIRE6_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_6_kernel_e1x1, fire_6_bias_e1x1, FIRE6_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_6_kernel_e3x3, fire_6_bias_e3x3, FIRE6_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE6_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_7 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE7_e1x1+FIRE7_e3x3) ), sizeof(float));
    fire_layer (    fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_7_kernel_s1x1, fire_7_bias_s1x1, FIRE7_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_7_kernel_e1x1, fire_7_bias_e1x1, FIRE7_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_7_kernel_e3x3, fire_7_bias_e3x3, FIRE7_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE7_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_8 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE8_e1x1+FIRE8_e3x3) ), sizeof(float));
    fire_layer (    fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_8_kernel_s1x1, fire_8_bias_s1x1, FIRE8_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_8_kernel_e1x1, fire_8_bias_e1x1, FIRE8_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_8_kernel_e3x3, fire_8_bias_e3x3, FIRE8_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE8_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_9 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE9_e1x1+FIRE9_e3x3) ), sizeof(float));
    fire_layer (    fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_9_kernel_s1x1, fire_9_bias_s1x1, FIRE9_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_9_kernel_e1x1, fire_9_bias_e1x1, FIRE9_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_9_kernel_e3x3, fire_9_bias_e3x3, FIRE9_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE9_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_10 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE10_e1x1+FIRE10_e3x3) ), sizeof(float));
    fire_layer (    fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE8_e3x3),     // Salida: pointer, ancho, alto, profundidad
                    fire_10_kernel_s1x1, fire_10_bias_s1x1, FIRE10_s1x1,                // kernel_s1x1, bias_s1x1, s1x1
                    fire_10_kernel_e1x1, fire_10_bias_e1x1, FIRE10_e1x1,                // kernel_e1x1, bias_e1x1, e1x1
                    fire_10_kernel_e3x3, fire_10_bias_e3x3, FIRE10_e3x3,                // kernel_e3x3, bias_e3x3, e3x3
                    FIRE10_RELU                                     );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_11 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE11_e1x1+FIRE11_e3x3) ), sizeof(float));
    fire_layer (    fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE10_e3x3),    // Entrada: pointer, ancho, alto, profundidad
                    fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3),    // Salida: pointer, ancho, alto, profundidad
                    fire_11_kernel_s1x1, fire_11_bias_s1x1, FIRE11_s1x1,                // kernel_s1x1, bias_s1x1, s1x1
                    fire_11_kernel_e1x1, fire_11_bias_e1x1, FIRE11_e1x1,                // kernel_e1x1, bias_e1x1, e1x1
                    fire_11_kernel_e3x3, fire_11_bias_e3x3, FIRE11_e3x3,                // kernel_e3x3, bias_e3x3, e3x3
                    FIRE11_RELU                                     );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(img_padded);
    img_padded = (float*) calloc( (pool_width_5+CONV12_PAD)*(pool_height_5+CONV12_PAD)*(FIRE11_e1x1+FIRE11_e3x3) , sizeof(float) );
    padding(    fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3),
                img_padded,
                CONV12_PAD, CONV12_PAD); 
    output_conv12_width  = (((pool_width_5  - CONV12_KERNEL_SIZE + CONV12_PAD)/CONV12_STRIDE) + 1); 
    output_conv12_height = (((pool_height_5 - CONV12_KERNEL_SIZE + CONV12_PAD)/CONV12_STRIDE) + 1);
    conv12_output = (float*) calloc( output_conv12_width*output_conv12_height*CONV12_FILTERS, sizeof(float) );
    convolucion2d(  img_padded, (pool_width_5+CONV12_PAD), (pool_height_5+CONV12_PAD), (FIRE11_e1x1+FIRE11_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    conv12_kernel, CONV12_KERNEL_SIZE,                                                              // Kernel: pointer, size
                    conv12_bias,                                                                                    // Bias: pointer
                    CONV12_STRIDE,                                                                                  // Stride
                    CONV12_RELU,                                                                                    // Relu?
                    conv12_output, output_conv12_width, output_conv12_height, CONV12_FILTERS );                     // Salida: pointer, ancho, alto, profundidad
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Consigo un vector con las predicciones de la clase 
    pred_class = (float*) malloc( 2700*sizeof(float) );
     // Consigo un vector con puntuaciones de confianza
    pred_conf = (float*) malloc( 2700*sizeof(float) );
    // probabilidad
    prob = (float*) malloc( 2700*sizeof(float) );
    // boxes
    box_predicted = (float*) malloc( 10800*sizeof(float) );

    for(j=0;j<output_conv12_height;j++)
    {
        for(i=0;i<output_conv12_width;i++)
        {
            for(k=0;k<54;k++)
            {
                //*(reshape + l + CONV12_FILTERS*i + output_conv12_height*CONV12_FILTERS*k ) = *(conv12_output + k + output_conv12_width*i + output_conv12_width*output_conv12_height*l);
                if ( k < 9 )
                    *(pred_class + k + 9*i + 9*output_conv12_width*j) = *(conv12_output + i + output_conv12_width*j + output_conv12_width*output_conv12_height*k);
                if ( (k >= 9) && (k < 18))
                    *(pred_conf + (k - 9) + 9*i + 9*output_conv12_width*j) = *(conv12_output + i + output_conv12_width*j + output_conv12_width*output_conv12_height*k);
                if ( k >= 18)
                    *(box_predicted + (k - 18) + 36*i + 36*output_conv12_width*j) = *(conv12_output + i + output_conv12_width*j + output_conv12_width*output_conv12_height*k);
            }
        }
    }

    // Los normalizo 
    pred_class_softmax = softmax(pred_class, 2700); 

    // Los normalizo 
    pred_conf_sigmod = sigmoid(pred_conf, 2700);

    // Probabilidad
    for(i=0;i<2700;i++)
        *(prob + i) = *(pred_class_softmax + i) * *(pred_conf_sigmod + i);

    // Max prob
    for(i=0;i<2700;i++)
    {
        if( *(prob + i) > max)
        {
            max = *(prob + i);
            index_Ofmax = i;
        }
    }

    // Search delta max prob
    // Boxes from delta
    delta_x = *(box_predicted + index_Ofmax*4 + 0);
    delta_y = *(box_predicted + index_Ofmax*4 + 1);
    delta_w = *(box_predicted + index_Ofmax*4 + 2);
    delta_h = *(box_predicted + index_Ofmax*4 + 3);
    
    // Delta box
    anchors = anchorBox_load("./files_fromTrain/ANCHOR_BOX.txt");

    // Consigo los anchors box
    box_center_x = *(anchors + 0 + 4*index_Ofmax) + delta_x * *(anchors + 2 + 4*index_Ofmax);   //box_center_x = anchor_x + delta_x * anchor_w;
    box_center_y = *(anchors + 1 + 4*index_Ofmax) + delta_y * *(anchors + 3 + 4*index_Ofmax);   //box_center_y = anchor_y + delta_y * anchor_h;
    box_width    = *(anchors + 2 + 4*index_Ofmax) * exp(delta_w);                               //box_width    = anchor_w * safe_exp_np(delta_w, config.EXP_THRESH);
    box_height   = *(anchors + 3 + 4*index_Ofmax) * exp(delta_h);                               //box_height   = anchor_h * safe_exp_np(delta_h, config.EXP_THRESH);
    //anchor_x = config.ANCHOR_BOX[:, 0] //anchor_y = config.ANCHOR_BOX[:, 1] //anchor_w = config.ANCHOR_BOX[:, 2] //anchor_h = config.ANCHOR_BOX[:, 3]
 
    // extremos de la caja
    xmin = box_center_x-box_width/2;
    ymin = box_center_y-box_height/2;
    xmax = box_center_x+box_width/2;
    ymax = box_center_y+box_height/2;
    
    // Recorto boxes si esta por fuera de la imagen
    // Escribir...

    // Dibujo la caja
    process_png_file(img, xmin, xmax, ymin, ymax);

    // Guardo la imagen
    write_png_file("./test5MODIF.png");

    printf("Finished\n");

    clock_t end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time executing.. %f \n", time_spent);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    Libero Memoria dinamica     ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(img);
    free(img_padded);

    free(conv1_kernel);             free(conv12_kernel);                                     
    free(conv1_bias);               free(conv12_bias);                                                        
    free(conv1_output);             free(conv12_output);                                                                         
    
    free(max_pool_1);               free(max_pool_3);               free(max_pool_5);        

    free(fire_2            );       free(fire_3            );       free(fire_4            );       free(fire_5            ); 
    free(fire_2_kernel_s1x1);       free(fire_3_kernel_s1x1);       free(fire_4_kernel_s1x1);       free(fire_5_kernel_s1x1); 
    free(fire_2_bias_s1x1  );       free(fire_3_bias_s1x1  );       free(fire_4_bias_s1x1  );       free(fire_5_bias_s1x1  ); 
    free(fire_2_kernel_e1x1);       free(fire_3_kernel_e1x1);       free(fire_4_kernel_e1x1);       free(fire_5_kernel_e1x1); 
    free(fire_2_bias_e1x1  );       free(fire_3_bias_e1x1  );       free(fire_4_bias_e1x1  );       free(fire_5_bias_e1x1  ); 
    free(fire_2_kernel_e3x3);       free(fire_3_kernel_e3x3);       free(fire_4_kernel_e3x3);       free(fire_5_kernel_e3x3); 
    free(fire_2_bias_e3x3  );       free(fire_3_bias_e3x3  );       free(fire_4_bias_e3x3  );       free(fire_5_bias_e3x3  );    

    free(fire_6            );       free(fire_7            );       free(fire_8            );       free(fire_9            ); 
    free(fire_6_kernel_s1x1);       free(fire_7_kernel_s1x1);       free(fire_8_kernel_s1x1);       free(fire_9_kernel_s1x1); 
    free(fire_6_bias_s1x1  );       free(fire_7_bias_s1x1  );       free(fire_8_bias_s1x1  );       free(fire_9_bias_s1x1  ); 
    free(fire_6_kernel_e1x1);       free(fire_7_kernel_e1x1);       free(fire_8_kernel_e1x1);       free(fire_9_kernel_e1x1); 
    free(fire_6_bias_e1x1  );       free(fire_7_bias_e1x1  );       free(fire_8_bias_e1x1  );       free(fire_9_bias_e1x1  ); 
    free(fire_6_kernel_e3x3);       free(fire_7_kernel_e3x3);       free(fire_8_kernel_e3x3);       free(fire_9_kernel_e3x3); 
    free(fire_6_bias_e3x3  );       free(fire_7_bias_e3x3  );       free(fire_8_bias_e3x3  );       free(fire_9_bias_e3x3  );    

    free(fire_10            );      free(fire_11            );      free(pred_class        );
    free(fire_10_kernel_s1x1);      free(fire_11_kernel_s1x1);      free(pred_conf         );
    free(fire_10_bias_s1x1  );      free(fire_11_bias_s1x1  );      free(pred_class_softmax);
    free(fire_10_kernel_e1x1);      free(fire_11_kernel_e1x1);      free(pred_conf_sigmod  );
    free(fire_10_bias_e1x1  );      free(fire_11_bias_e1x1  );      free(prob              );
    free(fire_10_kernel_e3x3);      free(fire_11_kernel_e3x3);      free(box_predicted     );
    free(fire_10_bias_e3x3  );      free(fire_11_bias_e3x3  );      free(anchors           );  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
