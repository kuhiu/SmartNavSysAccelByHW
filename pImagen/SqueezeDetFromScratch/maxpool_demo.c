#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define IMG_WIDTH                    7
#define IMG_HEIGHT                   7
#define PIXEL_SIZE                   4
#define KERNEL_SIZE                  3
#define PAD_SIZE                     0 // x2
#define FILTERS                     10
#define STRIDE                       2
#define PADDING_VALID                0
#define PADDING_SAME                 1                        

// input 7x7x3 
// pool 3x3
// stride 2
// pad 2x4
// output 7x7

// input 7x7x3 
// pool 3x3
// stride 2
// pad 2x0
// output 7x7

void maxPool2d( int pool_size, int stride, int *img, int input_shape_width, int input_shape_height, int input_depth, int padding )
{
    int i, j, k, l, c;
    // Pads necesarios a agregar a la entrada
    int pad_along_width, pad_along_height;
    int pad_top, pad_bottom, pad_left, pad_right;
    // New shape
    int new_height, new_width;
    // Vectores 
    int *max_pool = NULL;
    int *input_padding = NULL;
    // Auxiliar
    int row, columns, col;

    if (padding == 0)
        printf("No agrego pads \n");
    else if (padding == 1)
    {
        printf("Agrego pads output_size igual al input_size\n");
        // Output_size igual al input_size
        //pad_along_width  = ((input_shape_width -1)*stride + pool_size - input_shape_width );
        //pad_along_height = ((input_shape_height-1)*stride + pool_size - input_shape_height);
        pad_along_width  = 2;
        pad_along_height = 2;
        printf("TEST: pads: %d, %d\n", pad_along_width, pad_along_height);
        // En caso de que sea necesario agregar impar el numero de pads se agregara 
        // en la parte inferior o derecha de la imagen de entrada.
        pad_top = pad_along_height / 2;
        pad_bottom = pad_along_height - pad_top;
        pad_left = pad_along_width / 2;
        pad_right = pad_along_width - pad_left;
        printf("TEST: pads: %d, %d, %d, %d\n", pad_top, pad_bottom, pad_left, pad_right);

        new_width  = input_shape_width+pad_along_width;
        new_height = input_shape_height+pad_along_height;

        input_padding = (int*) malloc( (new_height*new_width)*sizeof(int));
        for (i = 0; i < new_height; i++)    // col
        {
            for (j = 0; j < new_width; j++) // row
            {
                if (j < pad_left)
                    *(input_padding + i*new_height + j) = 0;
                else if ( j >= (new_width - pad_left) )
                    *(input_padding + i*new_height + j) = 0;
                else if ( i < pad_top )
                    *(input_padding + i*new_height + j) = 0;
                else if ( i >= (new_height-pad_bottom))
                    *(input_padding + i*new_height + j) = 0;
                else
                    *(input_padding + i*new_height + j) = *(img + (i-pad_bottom)*input_shape_height + (j-pad_left));
                
                //printf("El x e y en input2 son: %d, %d \n", (i-pad_bottom), (j-pad_left));
                //printf("input_padding: %d %d %d\n", *(input_padding + i*new_height + j) , i, j );
            }
        }
        printf("input_padding\n"); 
        for (i = 0; i < new_height; i++) {
            for (j = 0; j < new_width; j++) {
                printf("%d ", *(input_padding + i*9 + j)); 
            }
            printf("\n");
        }
    }


    printf("Procesdo con el poolling\n");
    max_pool = (int*) malloc( (4*4)*sizeof(int));

    for (c = 0; c < input_depth; ++c) { // Cantidad de filtros o canales de la entrada
        for (j = 0; j < 4; ++j) {       // Filas del salto
            for (i = 0; i < 4; ++i) {   // Columnas del salto
                for (l = 0; l < pool_size; ++l) {       // Filas del kernel
                    for (k = 0; k < pool_size; ++k) {   // Columnas del kernel
                        //printf("barrido: %d\n", *(input_padding + (i * stride + k) + 9*(j * stride + l) + 9*9*c) );
                        if (*(max_pool + i + j*4 + c*4*4) < *(input_padding + (i * stride + k) + 9*(j * stride + l) + 9*9*c) ) 
                            *(max_pool + i + j*4 + c*4*4) = *(input_padding + (i * stride + k) + 9*(j * stride + l) + 9*9*c);
                        //sleep(1);
                    }
                }
            //printf("barrido: %d\n", *(input_padding + (i * stride + k) + 9*(j * stride + l) + 9*9*c) );
            //sleep(1);
            }
        }
    }

    printf("max_pool: \n");
    for (i = 0; i < 4; i++) {      // Columnas
        for (j = 0; j < 4; j++) {   // Filas
            printf(" %d", *(max_pool + i*4 + j) );
        }
        printf("\n");
    }

    if (padding == 1)
        free(input_padding);

    return;
} 

int main(void)
{
    int i, j;
    int *img = NULL;
    
    img = (int*) malloc( IMG_WIDTH*IMG_HEIGHT*sizeof(int) );

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

    maxPool2d(3, 2, img, IMG_WIDTH, IMG_HEIGHT, 3, PADDING_SAME);
    
    free(img);
    return 0;
}