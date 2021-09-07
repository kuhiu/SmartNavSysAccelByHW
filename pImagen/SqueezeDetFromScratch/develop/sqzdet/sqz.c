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
    size_t len = 0;
    char * line             = NULL;

    // Imagen de entrada
    float *img              = NULL;
    float *img_padded       = NULL;
    float *reshape          = NULL;

    // Conv1
    float *conv1_kernel     = NULL;
    float *conv1_bias       = NULL;
    float *conv1_output     = NULL;
    int output_conv1_width, output_conv1_height;
    // Conv12
    float *conv12_kernel    = NULL;
    float *conv12_bias      = NULL;
    float *conv12_output    = NULL;
    int output_conv12_width, output_conv12_height;

    // maxpool
    float *max_pool_1       = NULL;
    int pool_width_1, pool_height_1;
    float *max_pool_3       = NULL;
    int pool_width_3, pool_height_3;
    float *max_pool_5       = NULL;
    int pool_width_5, pool_height_5;

    // fire 2 layer
    float *fire_2 = NULL;
    float *fire_2_kernel_s1x1   = NULL;
    float *fire_2_bias_s1x1     = NULL;
    float *fire_2_kernel_e1x1   = NULL;
    float *fire_2_bias_e1x1     = NULL;
    float *fire_2_kernel_e3x3   = NULL;
    float *fire_2_bias_e3x3     = NULL;
 
    // fire 3 layer
    float *fire_3 = NULL;
    float *fire_3_kernel_s1x1   = NULL;
    float *fire_3_bias_s1x1     = NULL;
    float *fire_3_kernel_e1x1   = NULL;
    float *fire_3_bias_e1x1     = NULL;
    float *fire_3_kernel_e3x3   = NULL;
    float *fire_3_bias_e3x3     = NULL;

    // fire 4 layer
    float *fire_4               = NULL;
    float *fire_4_kernel_s1x1   = NULL;
    float *fire_4_bias_s1x1     = NULL;
    float *fire_4_kernel_e1x1   = NULL;
    float *fire_4_bias_e1x1     = NULL;
    float *fire_4_kernel_e3x3   = NULL;
    float *fire_4_bias_e3x3     = NULL;

    // fire 5 layer
    float *fire_5               = NULL;
    float *fire_5_kernel_s1x1   = NULL;
    float *fire_5_bias_s1x1     = NULL;
    float *fire_5_kernel_e1x1   = NULL;
    float *fire_5_bias_e1x1     = NULL;
    float *fire_5_kernel_e3x3   = NULL;
    float *fire_5_bias_e3x3     = NULL;

    // fire 6 layer
    float *fire_6               = NULL;
    float *fire_6_kernel_s1x1   = NULL;
    float *fire_6_bias_s1x1     = NULL;
    float *fire_6_kernel_e1x1   = NULL;
    float *fire_6_bias_e1x1     = NULL;
    float *fire_6_kernel_e3x3   = NULL;
    float *fire_6_bias_e3x3     = NULL;

    // fire 7 layer
    float *fire_7               = NULL;
    float *fire_7_kernel_s1x1   = NULL;
    float *fire_7_bias_s1x1     = NULL;
    float *fire_7_kernel_e1x1   = NULL;
    float *fire_7_bias_e1x1     = NULL;
    float *fire_7_kernel_e3x3   = NULL;
    float *fire_7_bias_e3x3     = NULL;

    // fire 8 layer
    float *fire_8               = NULL;
    float *fire_8_kernel_s1x1   = NULL;
    float *fire_8_bias_s1x1     = NULL;
    float *fire_8_kernel_e1x1   = NULL;
    float *fire_8_bias_e1x1     = NULL;
    float *fire_8_kernel_e3x3   = NULL;
    float *fire_8_bias_e3x3     = NULL;

    // fire 9 layer
    float *fire_9               = NULL;
    float *fire_9_kernel_s1x1   = NULL;
    float *fire_9_bias_s1x1     = NULL;
    float *fire_9_kernel_e1x1   = NULL;
    float *fire_9_bias_e1x1     = NULL;
    float *fire_9_kernel_e3x3   = NULL;
    float *fire_9_bias_e3x3     = NULL;

    // fire 10 layer
    float *fire_10              = NULL;
    float *fire_10_kernel_s1x1  = NULL;
    float *fire_10_bias_s1x1    = NULL;
    float *fire_10_kernel_e1x1  = NULL;
    float *fire_10_bias_e1x1    = NULL;
    float *fire_10_kernel_e3x3  = NULL;
    float *fire_10_bias_e3x3    = NULL;

    // fire 9 layer
    float *fire_11              = NULL;
    float *fire_11_kernel_s1x1  = NULL;
    float *fire_11_bias_s1x1    = NULL;
    float *fire_11_kernel_e1x1  = NULL;
    float *fire_11_bias_e1x1    = NULL;
    float *fire_11_kernel_e3x3  = NULL;
    float *fire_11_bias_e3x3    = NULL;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////        Leo la imagen       ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Leo el header de la imagen\n");
    read_png_file("test5.png");
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
    conv1_kernel = weight_load(fd, i_width, i_height, i_depth, i_filters);
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
    conv1_bias = bias_load(fd, i_width);
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
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_4   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire4:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_4_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_4_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire4:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_4_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_4_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire4:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_4_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_4_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire4:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_4_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_4_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire4:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_4_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_4_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire4:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_4_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_4_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_5   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire5:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_5_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_5_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire5:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_5_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_5_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire5:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_5_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_5_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire5:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_5_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_5_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire5:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_5_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_5_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire5:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_5_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_5_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_6   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire6:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_6_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_6_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire6:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_6_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_6_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire6:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_6_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_6_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire6:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_6_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_6_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire6:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_6_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_6_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire6:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_6_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_6_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_7   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire7:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_7_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_7_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire7:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_7_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_7_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire7:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_7_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_7_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire7:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_7_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_7_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire7:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_7_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_7_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire7:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_7_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_7_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_8   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire8:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_8_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_8_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire8:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_8_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_8_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire8:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_8_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_8_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire8:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_8_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_8_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire8:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_8_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_8_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire8:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_8_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_8_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_9   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire9:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_9_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_9_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire9:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_9_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_9_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire9:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_9_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_9_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire9:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_9_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_9_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire9:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_9_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_9_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire9:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_9_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_9_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_10   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire10:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_10_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_10_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire10:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_10_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_10_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire10:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_10_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_10_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire10:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_10_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_10_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire10:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_10_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_10_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire10:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_10_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_10_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_11   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire11:squeeze1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_11_kernel_s1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_11_kernel_s1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("Kernel fire2 s1x1: \n");
    //printVector(fire_2_kernel_s1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire11:expand1x1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_11_kernel_e1x1: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_11_kernel_e1x1 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e1x1: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire11:expand3x3:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("fire_11_kernel_e3x3: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    fire_11_kernel_e3x3 = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    // printf("fire_2_kernel_e3x3: \n");
    // printVector(fire_2_kernel_e1x1, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire11:squeeze1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_11_bias_s1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_11_bias_s1x1 = bias_load(fd, i_width);
    // Test
    //printf("squeeze1x1 Bias: \n");
    //printVector(fire_2_bias_s1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire11:expand1x1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_11_bias_e1x1: Las dimensiones de las bias es: %d \n", i_width);
    fire_11_bias_e1x1 = bias_load(fd, i_width);
    // Test
    //printf("expand1x1 Bias: \n");
    //printVector(fire_2_bias_e1x1, i_width, 1, 1, 1);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/fire11:expand3x3:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("fire_11_bias_e3x3: Las dimensiones de las bias es: %d \n", i_width);
    fire_11_bias_e3x3 = bias_load(fd, i_width);
    // Test
    //printf("expand3x3 Bias: \n");
    //printVector(fire_2_bias_e3x3, i_width, 1, 1, 1);
    fclose(fd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////    Leo txt para kernel y bias conv12    ///////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/conv12:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atof(s_width); i_height = atof(s_height); i_depth = atof(s_depth); i_filters = atof(s_filters);
    printf("conv12: Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    conv12_kernel = weight_load(fd, i_width, i_height, i_depth, i_filters);
    // Test
    //printf("kernel: \n");
    //printVector(kernel, i_width, i_height, i_depth, i_filters);
    fclose(fd);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/conv12:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_width);
    i_width = atoi(s_width);
    printf("conv12: Las dimensiones de las bias es: %d \n", i_width);
    conv12_bias = bias_load(fd, i_width);
    // Test
    //printf("Bias: \n");
    //printVector(bias, i_depth, 1, 1, 1);
    fclose(fd);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////          SqueezeDet        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Padding\n");
    img_padded = (float*) calloc( (img_width+1)*(img_height+1)*3 , sizeof(float) );
    padding(    img, img_width, img_height, 3,
                img_padded,
                1, 1); 
    printf("Creo puntero para Conv2D_1\n");
    output_conv1_width  = (((IMG_WIDTH  - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1); 
    output_conv1_height = (((IMG_HEIGHT - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1);
    printf("Conv1 output size: %dx%dx%d \n", output_conv1_width, output_conv1_height, CONV1_FILTERS);
    conv1_output = (float*) calloc( output_conv1_width*output_conv1_height*CONV1_FILTERS, sizeof(float) );
    printf("Conv2D_1... \n");
    convolucion2d(  img_padded, (img_width+1), (img_height+1), 3,                           // Entrada: pointer, ancho, alto, profundidad
                    conv1_kernel, CONV1_KERNEL_SIZE,                                        // Kernel: pointer, size
                    conv1_bias,                                                             // Bias: pointer
                    2,                                                                      // Stride
                    CONV1_RELU,
                    conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS ); // Salida: pointer, ancho, alto, profundidad
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
                conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS, 
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
    fire_layer (    max_pool_1, pool_width_1, pool_height_1, CONV1_FILTERS,             // Entrada: pointer, ancho, alto, profundidad
                    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_2_kernel_s1x1, fire_2_bias_s1x1, FIRE2_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_2_kernel_e1x1, fire_2_bias_e1x1, FIRE2_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_2_kernel_e3x3, fire_2_bias_e3x3, FIRE2_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE2_RELU                                          );              // Relu?
    //printf("Fire2: \n");
    //printVector(fire_2, pool_width, pool_height, 2, 1);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_3 \n");
    printf("FireLayer_3 output size: %dx%dx%d \n", pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3) );
    fire_3 = (float*) calloc( ( pool_width_1*pool_height_1*(FIRE3_e1x1+FIRE3_e3x3) ), sizeof(float));

    printf("Fire3... \n");
    fire_layer (    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_3_kernel_s1x1, fire_3_bias_s1x1, FIRE3_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_3_kernel_e1x1, fire_3_bias_e1x1, FIRE3_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_3_kernel_e3x3, fire_3_bias_e3x3, FIRE3_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE3_RELU                                        );                // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para pool_3 \n");
    pool_width_3  = pool_width_1/2;
    pool_height_3 = pool_height_1/2;
    printf("MaxPool_3 output size: %dx%dx%d \n", pool_width_3, pool_height_3, MAXPOOL3_FILTERS);
    max_pool_3 = (float*) calloc( ( pool_width_3*pool_height_3*MAXPOOL3_FILTERS ), sizeof(float));
    printf("MaxPool_3... \n");
    maxPool2d(  MAXPOOL3_POOL_SIZE, MAXPOOL3_STRIDE, 
                fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3), 
                max_pool_3, pool_width_3, pool_height_3,
                MAXPOOL3_PAD);
    //printf("TEST: MaxPool_3: \n");
    //printVector(max_pool_3, pool_width_3, pool_height_3, 1, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_4 \n");
    printf("FireLayer_4 output size: %dx%dx%d \n", pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3) );
    fire_4 = (float*) calloc( ( pool_width_3*pool_height_3*(FIRE4_e1x1+FIRE4_e3x3) ), sizeof(float));

    printf("Fire4... \n");
    fire_layer (    max_pool_3, pool_width_3, pool_height_3, (FIRE2_e1x1+FIRE2_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_4_kernel_s1x1, fire_4_bias_s1x1, FIRE4_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_4_kernel_e1x1, fire_4_bias_e1x1, FIRE4_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_4_kernel_e3x3, fire_4_bias_e3x3, FIRE4_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE4_RELU                                      );                  // Relu?
    //printf("Fire4: \n");
    //printVector(fire_4, pool_width_3, pool_height_3, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_5 \n");
    printf("FireLayer_5 output size: %dx%dx%d \n", pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3) );
    fire_5 = (float*) calloc( ( pool_width_3*pool_height_3*(FIRE5_e1x1+FIRE5_e3x3) ), sizeof(float));

    printf("Fire5... \n");
    fire_layer (    fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_5_kernel_s1x1, fire_5_bias_s1x1, FIRE5_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_5_kernel_e1x1, fire_5_bias_e1x1, FIRE5_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_5_kernel_e3x3, fire_5_bias_e3x3, FIRE5_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE5_RELU                                      );                  // Relu?
    //printf("Fire5: \n");  
    //printVector(fire_5, pool_width_3, pool_height_3, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para pool_5 \n");
    pool_width_5  = (pool_width_3)/2;
    pool_height_5 = (pool_height_3)/2;
    printf("MaxPool_5 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE5_e1x1+FIRE5_e3x3));
    max_pool_5 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE5_e1x1+FIRE5_e3x3) ), sizeof(float));
    printf("MaxPool_5... \n");
    maxPool2d(  MAXPOOL5_POOL_SIZE, MAXPOOL5_STRIDE, 
                fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3), 
                max_pool_5, pool_width_5, pool_height_5,
                MAXPOOL5_PAD);
    //printf("max_pool_5: \n");
    //printVector(max_pool_5, pool_width_5, pool_height_5, 256, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_6 \n");
    printf("FireLayer_6 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3) );
    fire_6 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE6_e1x1+FIRE6_e3x3) ), sizeof(float));

    printf("Fire6... \n");
    fire_layer (    max_pool_5, pool_width_5, pool_height_5, (FIRE5_e1x1+FIRE5_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_6_kernel_s1x1, fire_6_bias_s1x1, FIRE6_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_6_kernel_e1x1, fire_6_bias_e1x1, FIRE6_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_6_kernel_e3x3, fire_6_bias_e3x3, FIRE6_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE6_RELU                                      );                  // Relu?
    //printf("Fire6: \n");
    //printVector(fire_6, pool_width_5, pool_height_5, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_7 \n");
    printf("FireLayer_7 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3) );
    fire_7 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE7_e1x1+FIRE7_e3x3) ), sizeof(float));

    printf("Fire7... \n");
    fire_layer (    fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_7_kernel_s1x1, fire_7_bias_s1x1, FIRE7_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_7_kernel_e1x1, fire_7_bias_e1x1, FIRE7_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_7_kernel_e3x3, fire_7_bias_e3x3, FIRE7_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE7_RELU                                      );                  // Relu?
    //printf("Fire7: \n");
    //printVector(fire_7, pool_width_5, pool_height_5, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_8 \n");
    printf("FireLayer_8 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3) );
    fire_8 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE8_e1x1+FIRE8_e3x3) ), sizeof(float));

    printf("Fire8... \n");
    fire_layer (    fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_8_kernel_s1x1, fire_8_bias_s1x1, FIRE8_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_8_kernel_e1x1, fire_8_bias_e1x1, FIRE8_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_8_kernel_e3x3, fire_8_bias_e3x3, FIRE8_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE8_RELU                                      );                  // Relu?
    //printf("Fire8: \n");
    //printVector(fire_8, pool_width_5, pool_height_5, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_9 \n");
    printf("FireLayer_9 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3) );
    fire_9 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE9_e1x1+FIRE9_e3x3) ), sizeof(float));

    printf("Fire9... \n");
    fire_layer (    fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_9_kernel_s1x1, fire_9_bias_s1x1, FIRE9_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_9_kernel_e1x1, fire_9_bias_e1x1, FIRE9_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_9_kernel_e3x3, fire_9_bias_e3x3, FIRE9_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE9_RELU                                      );                  // Relu?
    //printf("Fire9: \n");
    //printVector(fire_9, pool_width_5, pool_height_5, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_10 \n");
    printf("FireLayer_10 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE10_e3x3) );
    fire_10 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE10_e1x1+FIRE10_e3x3) ), sizeof(float));

    printf("Fire10... \n");
    fire_layer (    fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE8_e3x3),     // Salida: pointer, ancho, alto, profundidad
                    fire_10_kernel_s1x1, fire_10_bias_s1x1, FIRE10_s1x1,                // kernel_s1x1, bias_s1x1, s1x1
                    fire_10_kernel_e1x1, fire_10_bias_e1x1, FIRE10_e1x1,                // kernel_e1x1, bias_e1x1, e1x1
                    fire_10_kernel_e3x3, fire_10_bias_e3x3, FIRE10_e3x3,                // kernel_e3x3, bias_e3x3, e3x3
                    FIRE10_RELU                                     );                  // Relu?
    //printf("Fire10: \n");
    //printVector(fire_10, pool_width_5, pool_height_5, 2, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para fire_11 \n");
    printf("FireLayer_11 output size: %dx%dx%d \n", pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3) );
    fire_11 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE11_e1x1+FIRE11_e3x3) ), sizeof(float));

    printf("Fire11... \n");
    fire_layer (    fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE10_e3x3),    // Entrada: pointer, ancho, alto, profundidad
                    fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3),    // Salida: pointer, ancho, alto, profundidad
                    fire_11_kernel_s1x1, fire_11_bias_s1x1, FIRE11_s1x1,                // kernel_s1x1, bias_s1x1, s1x1
                    fire_11_kernel_e1x1, fire_11_bias_e1x1, FIRE11_e1x1,                // kernel_e1x1, bias_e1x1, e1x1
                    fire_11_kernel_e3x3, fire_11_bias_e3x3, FIRE11_e3x3,                // kernel_e3x3, bias_e3x3, e3x3
                    FIRE11_RELU                                     );                  // Relu?
    //printf("Fire11: \n");
    //printVector(fire_11, pool_width_5, pool_height_5, 4, 1 );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Padding\n");
    free(img_padded);
    img_padded = (float*) calloc( (pool_width_5+CONV12_PAD)*(pool_height_5+CONV12_PAD)*(FIRE11_e1x1+FIRE11_e3x3) , sizeof(float) );
    padding(    fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3),
                img_padded,
                CONV12_PAD, CONV12_PAD); 
    printf("Creo puntero para Conv2D_1\n");
    output_conv12_width  = (((pool_width_5  - CONV12_KERNEL_SIZE + CONV12_PAD)/CONV12_STRIDE) + 1); 
    output_conv12_height = (((pool_height_5 - CONV12_KERNEL_SIZE + CONV12_PAD)/CONV12_STRIDE) + 1);
    printf("Conv1 output size: %dx%dx%d \n", output_conv12_width, output_conv12_height, CONV12_FILTERS);
    conv12_output = (float*) calloc( output_conv12_width*output_conv12_height*CONV12_FILTERS, sizeof(float) );
    printf("Conv2D_1... \n");
    convolucion2d(  img_padded, (pool_width_5+CONV12_PAD), (pool_height_5+CONV12_PAD), (FIRE11_e1x1+FIRE11_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    conv12_kernel, CONV12_KERNEL_SIZE,                                                              // Kernel: pointer, size
                    conv12_bias,                                                                                    // Bias: pointer
                    CONV12_STRIDE,                                                                                  // Stride
                    CONV12_RELU,                                                                                    // Relu?
                    conv12_output, output_conv12_width, output_conv12_height, CONV12_FILTERS );                     // Salida: pointer, ancho, alto, profundidad
    //printf("TEST: conv12_output: \n");
    //printVector(conv12_output, output_conv12_width, output_conv12_height, 54, 1 );

    // Reshape
    printf("Reshape \n");
    reshape = (float*) malloc( output_conv12_width*output_conv12_height*CONV12_FILTERS*sizeof(float) );
    for( int k=0 ; k<output_conv12_width ; k++){
       for( int i=0 ; i<output_conv12_height ; i++){
           for( int l=0 ; l<CONV12_FILTERS ; l++){
                printf(" %f", *(conv12_output + k + output_conv12_width*i + output_conv12_width*output_conv12_height*l));
                //printf("Voy a guardar: %s %d\n", param, atoi(param));
                //sleep(1);
                if( ((l % (6) ) == 5) && (l!=0) ) 
                    printf(" \n");
           }
       }
    }

    return 0;
}





