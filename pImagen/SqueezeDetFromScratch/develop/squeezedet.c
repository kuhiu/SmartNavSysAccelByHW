#include <stdio.h>
#include <limits.h>

#define FRAMEBUFFER_OFFSET  0x01000000
#define WIDTH                        7
#define HEIGHT                       7
#define PIXEL_SIZE                   4
#define KERNEL_SIZE                  3
#define PAD_SIZE                     0 // x2
#define FILTERS                     10
#define STRIDE                       2

#define output_size_1 (((WIDTH  - KERNEL_SIZE + 2*PAD_SIZE)/STRIDE) + 1)  
#define output_size_2 (((HEIGHT - KERNEL_SIZE + 2*PAD_SIZE)/STRIDE) + 1)  

void convolucion2d (int img[WIDTH][HEIGHT][PIXEL_SIZE], int kernel[KERNEL_SIZE][KERNEL_SIZE], int conv2d_1[output_size_1][output_size_2][FILTERS]) 
{
    int i, j, k, l;
    int img_slice[KERNEL_SIZE][KERNEL_SIZE];
    // Test
    int row, columns;

    for (int i = 0; i < output_size_1; ++i)                 // Filas
    {
        for (int j = 0; j < output_size_2; ++j)             // Columnas
        {
            for (int k = 0; k < KERNEL_SIZE; ++k)           // Filas del kernel 
            {
                for (int l = 0; l < KERNEL_SIZE; ++l)       // Columnas del kernel 
                {
                    //printf( "conv2d_1 asd %d\n" , conv2d_1[i][j][0]);
                    conv2d_1[i][j][0] += img[k + STRIDE*i][l + STRIDE*j][0] * kernel[k][l];
                    //printf( "conv2d_1 %d\n" , img[k + STRIDE*i][l + STRIDE*j][0] * kernel[k][l]);
                }
            }
        }
    }
}



int main()
{
    // Imagen de entrada
    int img[WIDTH][HEIGHT][PIXEL_SIZE];
    // Filtro
    int kernel[KERNEL_SIZE][KERNEL_SIZE];
    // Salida
    int conv2d_1[output_size_1][output_size_2][FILTERS];


    printf("El tamanio de la salida es: %dx%d \n", output_size_1, output_size_2);

    convolucion2d( img, kernel, conv2d_1 );

















    return 0;
}