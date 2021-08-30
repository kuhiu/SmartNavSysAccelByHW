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
    int i, j, k;

    // Imagen de entrada
    int *img      = NULL;
    int *kernel   = NULL;
    int *bias     = NULL;
    int *conv2d_1 = NULL;

    // Salida de conv1
    int output_conv1_width, output_conv1_height;

    printf("Leo caracteristicas de la Imagen\n");
    read_png_file("test.png");

    img      = (int*) malloc( img_width*img_height*3*sizeof(int) );
    kernel   = (int*) malloc( CONV1_KERNEL_SIZE*CONV1_KERNEL_SIZE*CONV1_FILTERS*sizeof(int) );
    bias     = (int*) malloc( CONV1_FILTERS*sizeof(int) );

    printf("Leo la Imagen\n");
    process_png_file(img);
    
    printf("Img: \n");
    printVector(img, img_width, img_height, 3);

    // Cargo el kernel
    for(k = 0; k < CONV1_FILTERS; k++) {
        for (i = 0; i < 3; i++) {       // Columnas
            for (j = 0; j < 3; j++) {   // Filas
                *(kernel + j + i*3 + 3*3*k) = 1+i; 
            }
        }
    }

    printf("Kernel: \n");
    printVector(kernel, CONV1_KERNEL_SIZE, CONV1_KERNEL_SIZE, CONV1_FILTERS);

    // Cargo las bias
    for (i = 0; i < CONV1_FILTERS; i++) {     
            *(bias + i) = 2+i; 
    }
    
    printf("Bias: \n");
    for (i = 0; i < CONV1_FILTERS; i++) 
        printf(" %d", *(bias + i) );
    printf("\n");

    output_conv1_width  = (((IMG_WIDTH  - CONV1_KERNEL_SIZE + 2*CONV1_PAD)/CONV1_STRIDE) + 1); 
    output_conv1_height = (((IMG_HEIGHT - CONV1_KERNEL_SIZE + 2*CONV1_PAD)/CONV1_STRIDE) + 1);
    printf("El tamanio de la salida de conv1 es: %dx%d \n", output_conv1_width, output_conv1_height);

    conv2d_1 = (int*) calloc( output_conv1_width*output_conv1_height*CONV1_FILTERS, sizeof(int) );
    convolucion2d( img, img_width, img_height, kernel, CONV1_KERNEL_SIZE, bias, conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS );

    maxPool2d(MAXPOOL1_POOL_SIZE, MAXPOOL1_STRIDE, conv2d_1, output_conv1_width, output_conv1_height, CONV1_FILTERS, MAXPOOL1_PAD);

    return 0;
}