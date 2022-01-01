#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <png.h>

#define FRAMEBUFFER_OFFSET  0x01000000
#define IMG_WIDTH                   70
#define IMG_HEIGHT                  70
#define PIXEL_SIZE                   4
#define KERNEL_SIZE                  3
#define PAD_SIZE                     0 // x2
#define FILTERS                      2
#define STRIDE                       2

#define output_size_1 (((IMG_WIDTH  - KERNEL_SIZE + 2*PAD_SIZE)/STRIDE) + 1)  
#define output_size_2 (((IMG_HEIGHT - KERNEL_SIZE + 2*PAD_SIZE)/STRIDE) + 1)  

// input 7x7x3 
// filter 3x3
// stride 1
// pad 0
// output 5x5

// Read png lib
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


int ReLu(int input) {
    if(input <= 0)
        return 0;
    else 
        return input;
}


void convolucion2d (int *img, int input_shape_width, int input_shape_height, int *kernel, int kernel_size, int *bias, int *conv2d_1, int conv2d_output1, int conv2d_output2, int depth) 
{
    int i, j, k, l, c, v;

    for ( c = 0; c < depth ; ++c)   // depth o filtros
    {
        for ( i = 0; i < conv2d_output1; ++i)                 // Filas
        {
            for ( j = 0; j < conv2d_output2; ++j)             // Columnas
            {
                for ( v = 0; v < 3 ; ++v)   // depth or channel
                {
                    for ( k = 0; k < KERNEL_SIZE; ++k)           // Filas del kernel 
                    {
                        for ( l = 0; l < KERNEL_SIZE; ++l)       // Columnas del kernel 
                        {
                            *(conv2d_1 + j + i*conv2d_output1 + c*conv2d_output1*conv2d_output2) += *(img + 70*(k + STRIDE*i) + (l + STRIDE*j + 70*70*v) ) * *(kernel + l + k*KERNEL_SIZE + c*KERNEL_SIZE*KERNEL_SIZE);
                        }    
                    }
                }
                // Bias
                *(conv2d_1 + j + i*conv2d_output1 + c*conv2d_output1*conv2d_output2) = *(conv2d_1 + j + i*conv2d_output1 + c*conv2d_output1*conv2d_output2) + *(bias+c);
                // Relu
                *(conv2d_1 + j + i*conv2d_output1 + c*conv2d_output1*conv2d_output2) = ReLu( *(conv2d_1 + j + i*conv2d_output1 + c*conv2d_output1*conv2d_output2) );
                if(c==1){
                    printf(" %d__",*(conv2d_1 + j + i*conv2d_output1 + c*conv2d_output1*conv2d_output2) );
                    //sleep(1);
                }
            }   
        }
    }

    for(k = 0; k < FILTERS; k++) {
        printf("conv2d_1, salida: %d\n", k);
        for (i = 0; i < conv2d_output1; i++) {      // Columnas
            for (j = 0; j < conv2d_output2; j++) {   // Filas
                printf(" %d", *(conv2d_1 + i*conv2d_output2 + j + conv2d_output1*conv2d_output2*k) );
            }
            printf("\n");
        }
    }
}


int main()
{    
    // Test
    int i, j, k;

    // Imagen de entrada
    int *img      = NULL;
    int *kernel   = NULL;
    int *bias     = NULL;
    int *conv2d_1 = NULL;

    read_png_file("test.png");

    img      = (int*) calloc( img_width*img_height*3*sizeof(int) );
    kernel   = (int*) malloc( KERNEL_SIZE*KERNEL_SIZE*FILTERS*sizeof(int) );
    bias     = (int*) malloc( FILTERS*sizeof(int) );
    conv2d_1 = (int*) calloc( output_size_1*output_size_2*FILTERS, sizeof(int) );

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

    // Cargo el kernel
    for(k = 0; k < FILTERS; k++) {
        for (i = 0; i < 3; i++) {       // Columnas
            for (j = 0; j < 3; j++) {   // Filas
                *(kernel + j + i*3 + 3*3*k) = 1+i; 
            }
        }
    }

    for(k = 0; k < FILTERS; k++) {
        printf("kernel, filter: %d\n", k);
        for (i = 0; i < 3; i++) {      // Columnas
            for (j = 0; j < 3; j++) {   // Filas
                printf(" %d", *(kernel + j + i*3 + 3*3*k) );
            }
            printf("\n");
        }
    }

    // Cargo las bias
    for (i = 0; i < FILTERS; i++) {     
            *(bias + i) = 2+i; 
    }
    
    printf("Bias: \n");
    for (i = 0; i < FILTERS; i++) 
        printf(" %d", *(bias + i) );
    printf("\n");

    printf("El tamanio de la salida de conv1 es: %dx%d \n", output_size_1, output_size_2);

    convolucion2d( img, img_width, img_height, kernel, KERNEL_SIZE, bias, conv2d_1, output_size_1, output_size_2, FILTERS );

    return 0;
}