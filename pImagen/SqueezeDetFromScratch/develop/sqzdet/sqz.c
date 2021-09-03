#include "sqz.h"

// Imgage size
int img_width, img_height;

// Read png lib
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

int main(void)
{    
    // Test
    int i, j, k, l;

    // asdds
    FILE * fd;
    char s_width[20], s_height[20], s_depth[20], s_filters[20];
    int  i_width, i_height, i_depth, i_filters;
    char * line = NULL;
    size_t len = 0;

    // Imagen de entrada
    int *img        = NULL;
    int *img_padded = NULL;

    // Conv1
    int *kernel   = NULL;
    int *bias     = NULL;
    int *conv2d_1 = NULL;
    int output_conv1_width, output_conv1_height;

    // maxpool
    int pool_width, pool_height;
    int *max_pool = NULL;

    // fire 2 layer
    int *fire_2 = NULL;
    int *fire_2_kernel_s1x1 = NULL;
    int *fire_2_bias_s1x1 = NULL;
    int *fire_2_kernel_e1x1 = NULL;
    int *fire_2_bias_e1x1 = NULL;
    int *fire_2_kernel_e3x3 = NULL;
    int *fire_2_bias_e3x3 = NULL;
 
    // fire 3 layer
    int *fire_3 = NULL;
    int *fire_3_kernel_s1x1 = NULL;
    int *fire_3_bias_s1x1 = NULL;
    int *fire_3_kernel_e1x1 = NULL;
    int *fire_3_bias_e1x1 = NULL;
    int *fire_3_kernel_e3x3 = NULL;
    int *fire_3_bias_e3x3 = NULL;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////        Leo la imagen       ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Leo el header de la imagen\n");
    read_png_file("test4.png");
    printf("Termino read_png_file\n");
    img = (int*) malloc( img_width*img_height*3*sizeof(int) );
    printf("Leo la Imagen\n");
    process_png_file(img);

    //printf("Img: \n");
    //printVector(img, img_width, img_height, 3);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////    Cargo los parametros    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Conv1
    //kernel = (int*) malloc( CONV1_KERNEL_SIZE*CONV1_KERNEL_SIZE*CONV1_FILTERS*sizeof(int) );
    //bias   = (int*) malloc( 1*1*CONV1_FILTERS*sizeof(int) );
    // Fire2
    fire_2_kernel_s1x1 = (int*) malloc( 1*1*FIRE2_s1x1*sizeof(int) );  
    fire_2_bias_s1x1   = (int*) malloc( 1*1*FIRE2_s1x1*sizeof(int) ); 
    fire_2_kernel_e1x1 = (int*) malloc( 1*1*FIRE2_e1x1*sizeof(int) ); 
    fire_2_bias_e1x1   = (int*) malloc( 1*1*FIRE2_e1x1*sizeof(int) );  
    fire_2_kernel_e3x3 = (int*) malloc( 3*3*FIRE2_e3x3*sizeof(int) ); 
    fire_2_bias_e3x3   = (int*) malloc( 1*1*FIRE2_e3x3*sizeof(int) ); 
    // Fire3
    fire_3_kernel_s1x1 = (int*) malloc( 1*1*FIRE3_s1x1*sizeof(int) );  
    fire_3_bias_s1x1   = (int*) malloc( 1*1*FIRE3_s1x1*sizeof(int) ); 
    fire_3_kernel_e1x1 = (int*) malloc( 1*1*FIRE3_e1x1*sizeof(int) ); 
    fire_3_bias_e1x1   = (int*) malloc( 1*1*FIRE3_e1x1*sizeof(int) );  
    fire_3_kernel_e3x3 = (int*) malloc( 3*3*FIRE3_e3x3*sizeof(int) ); 
    fire_3_bias_e3x3   = (int*) malloc( 1*1*FIRE3_e3x3*sizeof(int) ); 

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Leo txt para kernel conv1
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/conv2d_1:kernel:0.txt", "r");
    getline(&line, &len, fd);
    //printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);
    i_width = atoi(s_width); i_height = atoi(s_height); i_depth = atoi(s_depth); i_filters = atoi(s_filters);
    printf("Las dimensiones del kernel son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    
    kernel = weight_load(fd, i_width, i_height, i_depth, i_filters);

    // Test
    printf("Kernel conv1: \n");
    for( l=0 ; l<i_filters ; l++){
        for( k=0 ; k<i_depth ; k++){
            for( j=0 ; j<i_height ; j++){
                for( i=0 ; i<i_width ; i++){
                    printf("%d ", *(kernel + i + i_width*j + i_width*i_height*k + i_depth*i_width*i_height*l));
                } 
            }
        }
    }
    printf("\n");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Leo txt para bias conv1
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fd = fopen("../../../SqueezeNet/squeezedet-keras-master/main/model/parametros/conv2d_1:bias:0.txt", "r");
    getline(&line, &len, fd);
    sscanf(line, " (%[^','], )\n", s_depth);
    i_depth = atoi(s_depth);
    printf("Las dimensiones de las bias es: %d \n", i_depth);

    bias = bias_load(fd, i_depth);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Cargo el kernel
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(k = 0; k < CONV1_FILTERS; k++) {
        for (i = 0; i < 3; i++) {       // Columnas
            for (j = 0; j < 3; j++) {   // Filas
                //*(kernel + j + i*3 + 3*3*k) = 1+i; 
                *(fire_2_kernel_e3x3 + j + i*3 + 3*3*k) = 1+i;
                *(fire_3_kernel_e3x3 + j + i*3 + 3*3*k) = 1+i;
            }
        }
    }

    for(k = 0; k < FIRE2_s1x1; k++) {
        for (i = 0; i < 1; i++) {       // Columnas
            for (j = 0; j < 1; j++) {   // Filas
                *(fire_2_kernel_s1x1 + j + i*1 + 1*1*k) = 1+i; 
                *(fire_2_kernel_e1x1 + j + i*1 + 1*1*k) = 1+i;
                *(fire_3_kernel_s1x1 + j + i*1 + 1*1*k) = 1+i; 
                *(fire_3_kernel_e1x1 + j + i*1 + 1*1*k) = 1+i;
            }
        }
    }

    for (i = 0; i < 2; i++) {     
        *(fire_2_bias_s1x1 + i) = 2+i; 
        *(fire_2_bias_e1x1 + i) = 2+i; 
        *(fire_2_bias_e3x3 + i) = 2+i;
        *(fire_3_bias_s1x1 + i) = 2+i; 
        *(fire_3_bias_e1x1 + i) = 2+i; 
        *(fire_3_bias_e3x3 + i) = 2+i;
    }


    //printVector(kernel, CONV1_KERNEL_SIZE, CONV1_KERNEL_SIZE, CONV1_FILTERS);
    
//    printf("e3x3 fire2: \n");
//    printVector(fire_2_kernel_e3x3, 3, 3, 2);
//
//    printf("s1x1 y bias fire2: \n");
//    printVector(fire_2_kernel_s1x1, 1, 1, 2);
//    printf("Bias: \n");
//    for (i = 0; i < 2; i++) 
//        printf(" %d", *(fire_2_bias_s1x1 + i) );
//    printf("\n");
//
//    printf("e3x3 fire3: \n");
//    printVector(fire_3_kernel_e3x3, 3, 3, 2);
//
//    printf("s1x1 y bias fire3: \n");
//    printVector(fire_3_kernel_s1x1, 1, 1, 2);
//    printf("Bias: \n");
//    for (i = 0; i < 2; i++) 
//        printf(" %d", *(fire_3_bias_s1x1 + i) );
//    printf("\n");

//    // Cargo las bias
//    for (i = 0; i < CONV1_FILTERS; i++) {     
//            *(bias + i) = 2+i; 
//    }
//    
    printf("Bias: \n");
    for (i = 0; i < 64; i++) 
        printf(" %d", *(bias + i) );
    printf("\n");

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////          SqueezeDet        ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    img_padded = (int*) calloc( (img_width+1)*(img_height+1)*3 , sizeof(int) );
    padding(    img, img_width, img_height, 3,
                img_padded,
                1, 1); 
    
    printf("Creo puntero para Conv2D_1\n");
    output_conv1_width  = (((IMG_WIDTH  - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1); 
    output_conv1_height = (((IMG_HEIGHT - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1);
    printf("Conv1 output size: %dx%dx%d \n", output_conv1_width, output_conv1_height, CONV1_FILTERS);
    conv2d_1 = (int*) calloc( output_conv1_width*output_conv1_height*CONV1_FILTERS, sizeof(int) );

    printf("Conv2D_1... \n");
    convolucion2d(  img_padded, (img_width+1), (img_height+1), 3,                           // Entrada: pointer, ancho, alto, profundidad
                    kernel, CONV1_KERNEL_SIZE,                                              // Kernel: pointer, size
                    bias,                                                                   // Bias: pointer
                    2,                                                                      // Stride
                    conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS );     // Salida: pointer, ancho, alto, profundidad
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo puntero para pool_1 \n");
    pool_width  = output_conv1_width/2;
    pool_height = output_conv1_height/2;
    printf("MaxPool_1 output size: %dx%dx%d \n", pool_width, pool_height, CONV1_FILTERS);
    max_pool = (int*) calloc( ( pool_width*pool_height*CONV1_FILTERS ), sizeof(int));

    printf("MaxPool_1... \n");
    maxPool2d(  MAXPOOL1_POOL_SIZE, MAXPOOL1_STRIDE, 
                conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS, 
                max_pool, pool_width, pool_height,
                MAXPOOL1_PAD);

    printf("TEST: MaxPool_1: \n");
    printVector(max_pool, pool_width, pool_height, CONV1_FILTERS );

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    printf("Creo puntero para fire_2 \n");
//    printf("FireLayer_2 output size: %dx%dx%d \n", pool_width, pool_height, (FIRE2_e1x1+FIRE2_e3x3) );
//    fire_2 = (int*) calloc( ( pool_width*pool_height*(FIRE2_e1x1+FIRE2_e3x3) ), sizeof(int));
//
//    printf("Fire2... \n");
//    fire_layer (    max_pool, pool_width, pool_height, CONV1_FILTERS,                   // Entrada: pointer, ancho, alto, profundidad
//                    fire_2, pool_width, pool_height, (FIRE2_e1x1+FIRE2_e3x3),
//                    fire_2_kernel_s1x1, fire_2_bias_s1x1, FIRE2_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
//                    fire_2_kernel_e1x1, fire_2_bias_e1x1, FIRE2_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
//                    fire_2_kernel_e3x3, fire_2_bias_e3x3, FIRE2_e3x3      );            // kernel_e3x3, bias_e3x3, e3x3
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    printf("Fire2: \n");
//    printVector(fire_2, pool_width, pool_height, 1 );
//    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    printf("Creo puntero para fire_3 \n");
//    printf("FireLayer_3 output size: %dx%dx%d \n", pool_width, pool_height, (FIRE3_e1x1+FIRE3_e3x3) );
//    fire_3 = (int*) calloc( ( pool_width*pool_height*CONV1_FILTERS ), sizeof(int));
//
//    printf("Fire3... \n");
//    fire_layer (    fire_2, pool_width, pool_height, (FIRE2_e1x1+FIRE2_e3x3),           // Entrada: pointer, ancho, alto, profundidad
//                    fire_3, pool_width, pool_height, (FIRE3_e1x1+FIRE3_e3x3),
//                    fire_3_kernel_s1x1, fire_3_bias_s1x1, FIRE3_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
//                    fire_3_kernel_e1x1, fire_3_bias_e1x1, FIRE3_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
//                    fire_3_kernel_e3x3, fire_3_bias_e3x3, FIRE3_e3x3      );            // kernel_e3x3, bias_e3x3, e3x3
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    printf("Fire3: \n");
//    printVector(fire_3, pool_width, pool_height, (FIRE3_e1x1+FIRE3_e3x3) );

    return 0;
}





