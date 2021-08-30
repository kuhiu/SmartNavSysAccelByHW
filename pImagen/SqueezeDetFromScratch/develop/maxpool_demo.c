#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <png.h>

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

int img_width, img_height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

void process_png_file(int *img) {
    int x, y, z;
    for(y = 0; y < img_height; y++) {
        png_bytep row = row_pointers[y];
        for(x = 0; x < img_width; x++) {
            png_bytep px = &(row[x * 4]);
            // Cargo la entrada
            for(z = 0; z<3; z++)
                *(img + x + img_height*y + img_height*img_width*z ) = px[z];

            //if (x == 0 && y == 0)
            //    printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d) \n", x, y, px[0], px[1], px[2], px[3]);     
            //if (x == 1 && y == 0)
            //    printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d) \n", x, y, px[0], px[1], px[2], px[3]);     
            //if (x == 2 && y == 0)
            //    printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d) \n", x, y, px[0], px[1], px[2], px[3]);    
        }
    }
}

void read_png_file(char *filename) {
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    img_width  = png_get_image_width(png, info);
    img_height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    if (row_pointers) abort();

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * img_height);
    for(int y = 0; y < img_height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);
}


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
        pad_along_width  = 1;
        pad_along_height = 1;
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

        input_padding = (int*) calloc( (new_height*new_width*3),sizeof(int));
        for(k = 0; k < 3; k++)
        {
            for (i = 0; i < new_height; i++)    // col
            {
                for (j = 0; j < new_width; j++) // row
                {
                    *(input_padding + j + i*new_height + k*new_width*new_height) = *(img + j + i*input_shape_height + k*input_shape_width*input_shape_height);
                    if (j < pad_left)
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    if ( j >= (new_width - pad_right) )
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    if ( i < pad_top )
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    if ( i >= (new_height-pad_bottom))
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    //printf("El x e y en input2 son: %d, %d \n", (i-pad_bottom), (j-pad_left));
                    //printf("input_padding: %d %d %d\n", *(input_padding + i*new_height + j) , i, j );
                }
            }
        }
        printf("input_padding\n"); 
        for (k = 0; k < 3; k++)
        {
            printf("input_padding channel %d: \n", k);
            for (i = 0; i < new_height; i++) {
                for (j = 0; j < new_width; j++) {
                    printf("%d ", *(input_padding + j + i*new_width + k*new_width*new_height)); 
                }
                printf("\n");
            }
        }
    }


    printf("Procesdo con el poolling\n");
    max_pool = (int*) calloc( (35*35), sizeof(int));

    for (c = 0; c < input_depth; ++c) { // Cantidad de filtros o canales de la entrada
        for (j = 0; j < 35; ++j) {       // Filas del salto
            for (i = 0; i < 35; ++i) {   // Columnas del salto
                for (l = 0; l < pool_size; ++l) {       // Filas del kernel
                    for (k = 0; k < pool_size; ++k) {   // Columnas del kernel
                        if (*(max_pool + i + j*35 + c*35*35) < *(input_padding + (i * stride + k) + 71*(j * stride + l) + 71*71*c) ) 
                            *(max_pool + i + j*35 + c*35*35) = *(input_padding + (i * stride + k) + 71*(j * stride + l) + 71*71*c);
                        //if(j==1) {
                        //    printf("  %d, max_pool: %d,i:%d\n", *(input_padding + (i * stride + k) + 71*(j * stride + l) + 71*71*c), *(max_pool + i + j*35 + c*35*35) , i );
                        //    sleep(1);
                        //}
                    }
                }
            }
        }
    }

    printf("max_pool: \n");
    for (k = 0; k < 2; k++)
    {
        printf("Max pool channel %d: \n", k);
        for (i = 0; i < 35; i++) {      // Columnas
            for (j = 0; j < 35; j++) {  // Filas
                printf(" %d", *(max_pool + j + i*35 + k*35*35) );
            }
            printf("\n");
        }
    }

    if (padding == 1)
        free(input_padding);

    return;
} 

int main(void)
{
    int i, j, k;
    int *img = NULL;

    read_png_file("test.png");
    img      = (int*) malloc( img_width*img_height*3*sizeof(int) );
    process_png_file(img);

    printf("Img: \n");
    for (k = 0; k < 3; k++){
        printf("Img channel %d: \n", k);
        for (i = 0; i < img_height; i++) {      // Columnas
            for (j = 0; j < img_width; j++) {   // Filas
                printf(" %d", *(img + j + i*img_height + k*img_height*img_width) );
            }
            printf("\n");
        }
    }

    maxPool2d(3, 2, img, img_width, img_height, 1, PADDING_SAME);
    
    free(img);
    return 0;
}