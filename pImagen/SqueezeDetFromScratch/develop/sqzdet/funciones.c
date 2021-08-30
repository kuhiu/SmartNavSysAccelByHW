#include "sqz.h"


void process_png_file(int *img) {
    int x, y, z;
    for(y = 0; y < img_height; y++) {
        png_bytep row = row_pointers[y];
        for(x = 0; x < img_width; x++) {
            png_bytep px = &(row[x * 4]);
            for(z = 0; z<3; z++)
                *(img + x + img_height*y + img_height*img_width*z ) = px[z]; 
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


void convolucion2d (int *input, int input_shape_width, int input_shape_height, int *kernel, int kernel_size, int *bias, int *conv2d_1, int output_conv1_width, int output_conv1_height, int filtros) 
{
    int i, j, k, l, c, v;

    for ( c = 0; c < filtros ; ++c)   // depth o filtros
    {
        for ( i = 0; i < output_conv1_height; ++i)          // Filas
        {
            for ( j = 0; j < output_conv1_width; ++j)       // Columnas
            {
                for ( v = 0; v < 3 ; ++v)   // depth or channel
                {
                    for ( k = 0; k < CONV1_KERNEL_SIZE; ++k)           // Filas del kernel 
                    {
                        for ( l = 0; l < CONV1_KERNEL_SIZE; ++l)       // Columnas del kernel 
                        {
                            *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) += *(input + 70*(k + CONV1_STRIDE*i) + (l + CONV1_STRIDE*j + 70*70*v) ) * *(kernel + l + k*CONV1_KERNEL_SIZE + c*CONV1_KERNEL_SIZE*CONV1_KERNEL_SIZE);
                        }    
                    }
                }
                // Bias
                *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) = *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) + *(bias+c);
                // Relu
                *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) = ReLu( *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) );
            }   
        }
    }
    printVector(conv2d_1, output_conv1_width, output_conv1_height, filtros);
}


void printVector(int *in, int in_width, int in_height, int depth)
{
    int i,j,k;

    for (k = 0; k < depth; k++){
        printf("depth %d: \n", k);
        for (i = 0; i < in_height; i++) {      // Columnas
            for (j = 0; j < in_width; j++) {   // Filas
                printf(" %d", *(in + j + i*in_height + k*in_height*in_width) );
            }
            printf("\n");
        }
    }
    return;
}


void maxPool2d( int pool_size, int stride, int *in, int input_shape_width, int input_shape_height, int input_depth, int padding )
{
    int i, j, k, l, c;
    // Pads necesarios a agregar a la entrada
    int pad_along_width, pad_along_height;
    int pad_top, pad_bottom, pad_left, pad_right;
    // New shape
    int new_height, new_width, pool_width, pool_height;
    // Vectores 
    int *max_pool = NULL;
    int *input_padding = NULL;

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

        printf("TEST: new_width: %d, new_height: %d \n", new_width, new_height);

        input_padding = (int*) calloc( (new_height*new_width*input_depth),sizeof(int));
        for(k = 0; k < input_depth; k++)
        {
            for (i = 0; i < new_height; i++)    // col
            {
                for (j = 0; j < new_width; j++) // row
                {
                    *(input_padding + j + i*new_height + k*new_width*new_height) = *(in + j + i*input_shape_height + k*input_shape_width*input_shape_height);
                    if (j < pad_left)
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    if ( j >= (new_width - pad_right) )
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    if ( i < pad_top )
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                    if ( i >= (new_height-pad_bottom))
                        *(input_padding + j + i*new_height + k*new_width*new_height) = 0;
                }
            }
        }
        printf("input_padding\n"); 
        printVector(input_padding, new_width, new_height, input_depth);
    }

    printf("Procedo con el poolling\n");
    pool_width  = input_shape_width/2;
    pool_height = input_shape_height/2;
    printf("TEST: pool_width: %d, pool_height: %d, input_depth: %d \n", pool_width, pool_height, input_depth);
    max_pool = (int*) calloc( ( pool_width*pool_height*input_depth ), sizeof(int));

    for (c = 0; c < input_depth; ++c) {     // Cantidad de filtros o canales de la entrada
        for (j = 0; j < pool_height; ++j) {         // Filas del salto
            for (i = 0; i < pool_width; ++i) {      // Columnas del salto
                for (l = 0; l < pool_size; ++l) {       // Filas del kernel
                    for (k = 0; k < pool_size; ++k) {   // Columnas del kernel
                        if (*(max_pool + i + j*pool_width + c*pool_width*pool_height) < *(input_padding + (i * stride + k) + new_width*(j * stride + l) + new_width*new_height*c) ) 
                            *(max_pool + i + j*pool_width + c*pool_width*pool_height) = *(input_padding + (i * stride + k) + new_width*(j * stride + l) + new_width*new_height*c);
                        //if(c==1) {
                        //    printf("  %d, max_pool: %d \n", *(input_padding + (i * stride + k) + new_width*(j * stride + l) + new_width*new_height*c), *(max_pool + i + j*pool_width + c*pool_width*pool_height) );
                        //    sleep(1);
                        //}
                    
                    }
                }
            }
        }
    }

    printf("max_pool: \n");
    printVector(max_pool, pool_width, pool_width, input_depth);

    if (padding == 1)
        free(input_padding);

    return;
} 