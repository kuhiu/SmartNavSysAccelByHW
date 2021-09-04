#include "sqz.h"

// Imgage size
int img_width, img_height;

// Read png lib
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

int main(void)
{    
    // Archivo
    FILE * fd;
    char s_width[20], s_height[20], s_depth[20], s_filters[20];
    int  i_width, i_height, i_depth, i_filters;
    char * line = NULL;
    size_t len = 0;

    // Imagen de entrada
    float *img        = NULL;
    float *img_padded = NULL;

    // Conv1
    float *kernel   = NULL;
    float *bias     = NULL;
    float *conv2d_1 = NULL;
    int output_conv1_width, output_conv1_height;

    // maxpool
    int pool_width_1, pool_height_1;
    int pool_width_2, pool_height_2;
    float *max_pool_1 = NULL;
    float *max_pool_2 = NULL;

    // fire 2 layer
    float *fire_2 = NULL;
    float *fire_2_kernel_s1x1 = NULL;
    float *fire_2_bias_s1x1 = NULL;
    float *fire_2_kernel_e1x1 = NULL;
    float *fire_2_bias_e1x1 = NULL;
    float *fire_2_kernel_e3x3 = NULL;
    float *fire_2_bias_e3x3 = NULL;
 
    // fire 3 layer
    float *fire_3 = NULL;
    float *fire_3_kernel_s1x1 = NULL;
    float *fire_3_bias_s1x1 = NULL;
    float *fire_3_kernel_e1x1 = NULL;
    float *fire_3_bias_e1x1 = NULL;
    float *fire_3_kernel_e3x3 = NULL;
    float *fire_3_bias_e3x3 = NULL;

    // fire 4 layer
    float *fire_4 = NULL;
    float *fire_4_kernel_s1x1 = NULL;
    float *fire_4_bias_s1x1 = NULL;
    float *fire_4_kernel_e1x1 = NULL;
    float *fire_4_bias_e1x1 = NULL;
    float *fire_4_kernel_e3x3 = NULL;
    float *fire_4_bias_e3x3 = NULL;

    // fire 5 layer
    float *fire_5 = NULL;
    float *fire_5_kernel_s1x1 = NULL;
    float *fire_5_bias_s1x1 = NULL;
    float *fire_5_kernel_e1x1 = NULL;
    float *fire_5_bias_e1x1 = NULL;
    float *fire_5_kernel_e3x3 = NULL;
    float *fire_5_bias_e3x3 = NULL;

    // fire 6 layer
    float *fire_6 = NULL;
    float *fire_6_kernel_s1x1 = NULL;
    float *fire_6_bias_s1x1 = NULL;
    float *fire_6_kernel_e1x1 = NULL;
    float *fire_6_bias_e1x1 = NULL;
    float *fire_6_kernel_e3x3 = NULL;
    float *fire_6_bias_e3x3 = NULL;

    // fire 7 layer
    float *fire_7 = NULL;
    float *fire_7_kernel_s1x1 = NULL;
    float *fire_7_bias_s1x1 = NULL;
    float *fire_7_kernel_e1x1 = NULL;
    float *fire_7_bias_e1x1 = NULL;
    float *fire_7_kernel_e3x3 = NULL;
    float *fire_7_bias_e3x3 = NULL;

    // fire 8 layer
    float *fire_8 = NULL;
    float *fire_8_kernel_s1x1 = NULL;
    float *fire_8_bias_s1x1 = NULL;
    float *fire_8_kernel_e1x1 = NULL;
    float *fire_8_bias_e1x1 = NULL;
    float *fire_8_kernel_e3x3 = NULL;
    float *fire_8_bias_e3x3 = NULL;

    // fire 9 layer
    float *fire_9 = NULL;
    float *fire_9_kernel_s1x1 = NULL;
    float *fire_9_bias_s1x1 = NULL;
    float *fire_9_kernel_e1x1 = NULL;
    float *fire_9_bias_e1x1 = NULL;
    float *fire_9_kernel_e3x3 = NULL;
    float *fire_9_bias_e3x3 = NULL;

    // fire 10 layer
    float *fire_10 = NULL;
    float *fire_10_kernel_s1x1 = NULL;
    float *fire_10_bias_s1x1 = NULL;
    float *fire_10_kernel_e1x1 = NULL;
    float *fire_10_bias_e1x1 = NULL;
    float *fire_10_kernel_e3x3 = NULL;
    float *fire_10_bias_e3x3 = NULL;

    // fire 9 layer
    float *fire_11 = NULL;
    float *fire_11_kernel_s1x1 = NULL;
    float *fire_11_bias_s1x1 = NULL;
    float *fire_11_kernel_e1x1 = NULL;
    float *fire_11_bias_e1x1 = NULL;
    float *fire_11_kernel_e3x3 = NULL;
    float *fire_11_bias_e3x3 = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////        Leo la imagen       ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Leo el header de la imagen\n");
    read_png_file("test4.png");
    printf("Termino read_png_file\n");
    img = (float*) malloc( img_width*img_height*3*sizeof(float) );
    printf("Leo la Imagen\n");
    process_png_file(img);

    //printf("Img: \n");
    //printVector(img, img_width, img_height, 3);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    Cargo los parametros    ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////    Leo txt para kernel y bias conv1    ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/conv2d_1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("CONV2d_1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    kernel = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("kernel: \n");
    //printVector(kernel, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/conv2d_1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("CONV2d_1: Las dimensiones de las bias es: %d \n", i_width);
    bias = bias_load(fd, i_width);
    // Test
    //printf("Bias: \n");
    //printVector(bias, i_depth, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////    Leo txt para las conv2d y bias de fire_2   ////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire2:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_2_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_2_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire2:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_2_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_2_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire2:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_2_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_2_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire2:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_2_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_2_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire2:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_2_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_2_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire2:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_2_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_2_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_3   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire3:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_3_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_3_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire3:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_3_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_3_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire3:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_3_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_3_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire3:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_3_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_3_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire3:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_3_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_3_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire3:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_3_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_3_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////          SqueezeDet        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    img_padded = (float*) calloc( (img_width+1)*(img_height+1)*3 , sizeof(float) );
    padding(    img, img_width, img_height, 3,
                img_padded,
                1, 1); 
    printf("Creo puntero para Conv2D_1\n");
    output_conv1_width  = (((IMG_WIDTH  - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1); 
    output_conv1_height = (((IMG_HEIGHT - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1);
    printf("Conv1 output size: %dx%dx%d \n", output_conv1_width, output_conv1_height, CONV1_FILTERS);
    conv2d_1 = (float*) calloc( output_conv1_width*output_conv1_height*CONV1_FILTERS, sizeof(float) );
    printf("Conv2D_1... \n");
    convolucion2d(  img_padded, (img_width+1), (img_height+1), 3,                           // Entrada: pointer, ancho, alto, profundidad
                    kernel, CONV1_KERNEL_SIZE,                                              // Kernel: pointer, size
                    bias,                                                                   // Bias: pointer
                    2,                                                                      // Stride
                    conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS );     // Salida: pointer, ancho, alto, profundidad
    //printf("TEST: Conv2D_1: \n");
    //printVector(conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para pool_1 \n");
    pool_width_1  = output_conv1_width/2;
    pool_height_1 = output_conv1_height/2;
    printf("MaxPool_1 output size: %dx%dx%d \n", pool_width_1, pool_height_1, CONV1_FILTERS);
    max_pool_1 = (float*) calloc( ( pool_width_1*pool_height_1*CONV1_FILTERS ), sizeof(float));
    printf("MaxPool_1... \n");
    maxPool2d(  MAXPOOL1_POOL_SIZE, MAXPOOL1_STRIDE, 
                conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS, 
                max_pool_1, pool_width_1, pool_height_1,
                MAXPOOL1_PAD);
    //printf("TEST: MaxPool_1: \n");
    //printVector(max_pool, pool_width, pool_height, 1, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_2 \n");
    printf("FireLayer_2 output size: %dx%dx%d \n", pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3) );
    fire_2 = (float*) calloc( ( pool_width_1*pool_height_1*(FIRE2_e1x1+FIRE2_e3x3) ), sizeof(float));
    //printf("TEST asdasdsaa: fire_2_kernel_s1x1: \n");
    //printVector(fire_2_kernel_s1x1, 1, 1, 1, 64 );
    printf("Fire2... \n");
    fire_layer (    max_pool_1, pool_width_1, pool_height_1, CONV1_FILTERS,                 // Entrada: pointer, ancho, alto, profundidad
                    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),
                    fire_2_kernel_s1x1, fire_2_bias_s1x1, FIRE2_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_2_kernel_e1x1, fire_2_bias_e1x1, FIRE2_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_2_kernel_e3x3, fire_2_bias_e3x3, FIRE2_e3x3      );            // kernel_e3x3, bias_e3x3, e3x3
    //printf("Fire2: \n");
    //printVector(fire_2, pool_width, pool_height, 2, 1);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_3 \n");
    printf("FireLayer_3 output size: %dx%dx%d \n", pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3) );
    fire_3 = (float*) calloc( ( pool_width_1*pool_height_1*CONV1_FILTERS ), sizeof(float));

    printf("Fire3... \n");
    fire_layer (    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),           // Entrada: pointer, ancho, alto, profundidad
                    fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3),
                    fire_3_kernel_s1x1, fire_3_bias_s1x1, FIRE3_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_3_kernel_e1x1, fire_3_bias_e1x1, FIRE3_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_3_kernel_e3x3, fire_3_bias_e3x3, FIRE3_e3x3      );            // kernel_e3x3, bias_e3x3, e3x3
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para pool_3 \n");
    pool_width_2  = pool_width_1/2;
    pool_height_2 = pool_height_1/2;
    printf("MaxPool_3 output size: %dx%dx%d \n", pool_width_2, pool_height_2, MAXPOOL3_FILTERS);
    max_pool_2 = (float*) calloc( ( pool_width_2*pool_height_2*MAXPOOL3_FILTERS ), sizeof(float));
    printf("MaxPool_3... \n");
    maxPool2d(  MAXPOOL1_POOL_SIZE, MAXPOOL1_STRIDE, 
                fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3), 
                max_pool_2, pool_width_2, pool_height_2,
                MAXPOOL1_PAD);
    printf("TEST: MaxPool_2: \n");
    printVector(max_pool_2, pool_width_2, pool_height_2, 1, 1 );
    
    
    
    
    
    //printf("Fire3: \n");
    //printVector(fire_3, pool_width, pool_height, 2, 1 );


    return 0;
}





