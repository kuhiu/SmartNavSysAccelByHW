#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#define FRAMEBUFFER_OFFSET  0x01000000
#define IMG_WIDTH                    7
#define IMG_HEIGHT                   7
#define PIXEL_SIZE                   4
#define KERNEL_SIZE                  3
#define PAD_SIZE                     0 // x2
#define FILTERS                     10
#define STRIDE                       2

#define output_size_1 (((IMG_WIDTH  - KERNEL_SIZE + 2*PAD_SIZE)/STRIDE) + 1)  
#define output_size_2 (((IMG_HEIGHT - KERNEL_SIZE + 2*PAD_SIZE)/STRIDE) + 1)  

// input 7x7x3 
// filter 3x3
// stride 1
// pad 0
// output 5x5

void convolucion2d (int *img, int input_shape_width, int input_shape_height, int *kernel, int kernel_size, int *conv2d_1, int depth) 
{
    int i, j, k, l, c;
    
    for ( c = 0; c < depth ; ++c)   // depth
    {
        for ( i = 0; i < output_size_1; ++i)                 // Filas
        {
            for ( j = 0; j < output_size_2; ++j)             // Columnas
            {
                for ( k = 0; k < KERNEL_SIZE; ++k)           // Filas del kernel 
                {
                    for ( l = 0; l < KERNEL_SIZE; ++l)       // Columnas del kernel 
                        *(conv2d_1 + j + i*output_size_1 + c*output_size_1*output_size_2) += *(img + 7*(k + STRIDE*i) + (l + STRIDE*j) + 7*7*c ) * *(kernel + l + k*KERNEL_SIZE);
                }
            }
        }
    }

    printf("conv2d_1: \n");
    for (i = 0; i < output_size_1; i++) {      // Columnas
        for (j = 0; j < output_size_2; j++) {   // Filas
            printf(" %d", *(conv2d_1 + i*output_size_2 + j) );
        }
        printf("\n");
    }
}


int main()
{    
    // Test
    int i, j;

    // Imagen de entrada
    int *img      = NULL;
    int *kernel   = NULL;
    int *conv2d_1 = NULL;
    
    img      = (int*) malloc( IMG_WIDTH*IMG_HEIGHT*sizeof(int) );
    kernel   = (int*) malloc( KERNEL_SIZE*KERNEL_SIZE*sizeof(int) );
    conv2d_1 = (int*) calloc( output_size_1*output_size_2, sizeof(int) );

    // Cargo la entrada
    for (i = 0; i < IMG_HEIGHT; i++) {      // Columnas
        for (j = 0; j < IMG_WIDTH; j++) {   // Filas
            *(img + i*IMG_HEIGHT + j) = 1+i; 
        }
    }
    
    printf("Img: \n");
    for (i = 0; i < IMG_HEIGHT; i++) {      // Columnas
        for (j = 0; j < IMG_WIDTH; j++) {   // Filas
            printf(" %d", *(img + i*IMG_HEIGHT + j) );
        }
        printf("\n");
    }

    // Cargo el kernel
    for (i = 0; i < 3; i++) {       // Columnas
        for (j = 0; j < 3; j++) {   // Filas
            *(kernel + i*3 + j) = 1+i; 
        }
    }
    
    printf("kernel: \n");
    for (i = 0; i < 3; i++) {      // Columnas
        for (j = 0; j < 3; j++) {   // Filas
            printf(" %d", *(kernel + i*3 + j) );
        }
        printf("\n");
    }

    printf("El tamanio de la salida es: %dx%d \n", output_size_1, output_size_2);

    convolucion2d( img, IMG_WIDTH, IMG_HEIGHT, kernel, KERNEL_SIZE, conv2d_1, 3 );

    return 0;
}