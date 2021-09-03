#include "sqz.h"


void process_png_file(int *img) {
    int x, y, z;
    for(y = 0; y < img_height; y++) {
        png_bytep row = row_pointers[y];
        for(x = 0; x < img_width; x++) {
            png_bytep px = &(row[x * 4]);
            for(z = 0; z<3; z++)
                *(img + x + img_width*y + img_height*img_width*z ) = px[z]; 
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


void convolucion2d (int *input, int input_shape_width, int input_shape_height, int input_depth, 
                    int *kernel, int kernel_size, 
                    int *bias, 
                    int stride,
                    int *conv2d_1, int output_conv1_width, int output_conv1_height, int filtros) 
{
    int i, j, k, l, c, v;

    for ( c = 0; c < filtros ; ++c)   // depth o filtros
    {
        for ( i = 0; i < output_conv1_height; ++i)          // Filas
        {
            for ( j = 0; j < output_conv1_width; ++j)       // Columnas
            {
                for ( v = 0; v < input_depth ; ++v)   // depth or channel input
                {
                    for ( k = 0; k < kernel_size; ++k)           // Filas del kernel 
                    {
                        for ( l = 0; l < kernel_size; ++l)       // Columnas del kernel 
                        {
                            *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) += *(input + input_shape_width*(k + stride*i) + (l + stride*j + input_shape_width*input_shape_height*v) ) * *(kernel + l + k*kernel_size + c*kernel_size*kernel_size);
                            //printf("acum: %d\n", *(input + input_shape_width*(k + stride*i) + (l + stride*j + input_shape_width*input_shape_height*v) ));
                            //usleep(300000);
                        }    
                    }
                }
                //printVector(conv2d_1, output_conv1_width, output_conv1_height, 1);
                //printf("acum: %d\n",*(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height));
                //usleep(300000);
                // Bias
                *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) = *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) + *(bias+c);
                // Relu
                *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) = ReLu( *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) );
            }   
        }
    }
    //printVector(conv2d_1, output_conv1_width, output_conv1_height, filtros);
}


void printVector(int *in, int in_width, int in_height, int depth)
{
    int i,j,k;

    for (k = 0; k < depth; k++){
        printf("depth %d: \n", k);
        for (i = 0; i < in_height; i++) {      // Columnas
            for (j = 0; j < in_width; j++) {   // Filas
                printf(" %d", *(in + j + i*in_width + k*in_height*in_width) );
            }
            printf("\n");
        }
    }
    return;
}


void padding(   int *input, int input_shape_width, int input_shape_height, int input_depth, 
                int *output,
                int pad_along_width, int pad_along_height) 
{
    int i, j, k;
    int new_width, new_height;
    int pad_top, pad_bottom, pad_left, pad_right; 
    
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

    for(k = 0; k < input_depth; k++)
    {
        for (i = 0; i < input_shape_height; i++)    // eow
        {
            for (j = 0; j < input_shape_width; j++) // col
                *(output + j + pad_left  + i*new_width + pad_top*new_width + k*new_width*new_height) = *(input + j + i*input_shape_width + k*input_shape_width*input_shape_height);
        }
    }
    //printf("output\n"); 
    //printVector(output, new_width, new_height, input_depth);
}

void maxPool2d( int pool_size, int stride, 
                int *in, int input_shape_width, int input_shape_height, int input_depth, 
                int *max_pool, int pool_width, int pool_height,
                int pad_type )
{
    int i, j, k, l, c;
    // Pads necesarios a agregar a la entrada
    //int pad_along_width, pad_along_height;
    //int pad_top, pad_bottom, pad_left, pad_right;
    // New shape
    int new_height, new_width;
    // Vectores 
    int *in_padded = NULL;

    if (pad_type == 0)
        printf("No agrego pads \n");
    else if (pad_type == 1)
    {
        new_width = input_shape_width+1;
        new_height = input_shape_height+1;
        in_padded = (int*) calloc( new_width*new_height*3 , sizeof(int) );
        padding (   in, input_shape_width, input_shape_height, 3,
                    in_padded,
                    1, 1); 
        //printf("input_padding\n"); 
        //printVector(in_padded, new_width, new_height, input_depth);
    }

    printf("Procedo con el poolling\n");

    for (c = 0; c < input_depth; ++c) {     // Cantidad de filtros o canales de la entrada
        for (j = 0; j < pool_height; ++j) {         // Filas del salto
            for (i = 0; i < pool_width; ++i) {      // Columnas del salto
                for (l = 0; l < pool_size; ++l) {       // Filas del kernel
                    for (k = 0; k < pool_size; ++k) {   // Columnas del kernel
                        if (*(max_pool + i + j*pool_width + c*pool_width*pool_height) < *(in_padded + (i * stride + k) + new_width*(j * stride + l) + new_width*new_height*c) ) 
                            *(max_pool + i + j*pool_width + c*pool_width*pool_height) = *(in_padded + (i * stride + k) + new_width*(j * stride + l) + new_width*new_height*c);
                        //if(c==1) {
                        //    printf("  %d, max_pool: %d \n", *(input_padding + (i * stride + k) + new_width*(j * stride + l) + new_width*new_height*c), *(max_pool + i + j*pool_width + c*pool_width*pool_height) );
                        //    sleep(1);
                        //}
                    
                    }
                }
            }
        }
    }
    //printf("max_pool: \n");
    //printVector(max_pool, pool_width, pool_height, input_depth);
    if (pad_type == 1)
        free(in_padded);

    return;
} 


void fire_layer (   int *input, int input_shape_width, int input_shape_height, int input_depth, 
                    int *output, int output_shape_width, int output_shape_height, int output_depth,
                    int *kernel_s1x1, int *bias_s1x1, int s1x1, 
                    int *kernel_e1x1, int *bias_e1x1, int e1x1, 
                    int *kernel_e3x3, int *bias_e3x3, int e3x3      ) 
{
    int i, j, k, offset;
    int *conv2d_s1x1 = (int*) calloc( input_shape_width*input_shape_height*s1x1, sizeof(int) );
    int *conv2d_e1x1 = (int*) calloc( input_shape_width*input_shape_height*e1x1, sizeof(int) );
    int *conv2d_e3x3 = (int*) calloc( input_shape_width*input_shape_height*e3x3, sizeof(int) );
    int *input_padded = NULL;

    printf("Conv1 de fire\n");
    convolucion2d(  input, input_shape_width, input_shape_height, input_depth,          // Entrada: pointer, ancho, alto, profundidad
                    kernel_s1x1, 1,                                                     // Kernel: pointer, size                            
                    bias_s1x1,                                                          // Bias: pointer                                        
                    1,                                                                  // Stride                                           
                    conv2d_s1x1, input_shape_width, input_shape_height, s1x1);          // Salida: pointer, ancho, alto, profundidad

    printf("Conv2 de fire\n");
    convolucion2d(  conv2d_s1x1, input_shape_width, input_shape_height, s1x1,           // Entrada: pointer, ancho, alto, profundidad                    
                    kernel_e1x1, 1,                                                     // Kernel: pointer, size                             
                    bias_e1x1,                                                          // Bias: pointer                                         
                    1,                                                                  // Stride                                    
                    conv2d_e1x1, input_shape_width, input_shape_height, e1x1);          // Salida: pointer, ancho, alto, profundidad

    input_padded =  (int*) calloc( (input_shape_width+2)*(input_shape_height+2)*s1x1,sizeof(int) );

    printf("Padeo\n");
    padding(        conv2d_s1x1, input_shape_width, input_shape_height, s1x1,           // Entrada: pointer, ancho, alto, profundidad
                    input_padded,                                                       // Salida: pointer
                    2, 2);                                                              // padd to add along width and height

    printf("Conv3 de fire\n");
    convolucion2d(  input_padded, (input_shape_width+2), (input_shape_height+2), s1x1,  // Entrada: pointer, ancho, alto, profundidad   
                    kernel_e3x3, 3,                                                     // Kernel: pointer, size                       
                    bias_e3x3,                                                          // Bias: pointer                                           
                    1,                                                                  // Stride                                                  
                    conv2d_e3x3, input_shape_width, input_shape_height, e3x3);          // Salida: pointer, ancho, alto, profundidad

    
    for (k = 0; k < e1x1; k++){
        for (i = 0; i < input_shape_height; i++) {      // Columnas
            for (j = 0; j < input_shape_width; j++) {   // Filas
                *(output + j + i*output_shape_width + k*output_shape_width*output_shape_height) = *(conv2d_e1x1 + j + i*input_shape_width + k*input_shape_width*input_shape_height);
            }
        }
    }
    offset = (input_shape_width) + (input_shape_height-1)*output_shape_width + (e1x1-1)*output_shape_width*output_shape_height; 
    for (k = 0; k < e3x3; k++){
        for (i = 0; i < input_shape_height; i++) {      // Columnas
            for (j = 0; j < input_shape_width; j++) {   // Filas
                *(output + j + i*output_shape_width + k*output_shape_width*output_shape_height + offset) = *(conv2d_e3x3 + j + i*input_shape_width + k*input_shape_width*input_shape_height);
            }
        }
    }
    //printf("output: \n");
    //printVector(output, input_shape_width, input_shape_height, 4);
}



int * weight_load(FILE * fd, int  i_width, int i_height, int i_depth, int i_filters)
{
    int i,j,k,l;
    size_t len = 0;
    char param[100];
    char * line = NULL;
    int *array = NULL;

    array = (int*) malloc( ( i_width*i_height*i_depth*i_filters ) * sizeof(int));

    for( l=0 ; l<i_filters ; l++){
        for( k=0 ; k<i_depth ; k++){
            for( j=0 ; j<i_height ; j++){
                for( i=0 ; i<i_width ; i++){
                    getline(&line, &len, fd);
                    sscanf(line, "%s\n", param);
                    //printf("Voy a guardar: %s %d\n", param, atoi(param));
                    *(array + i + i_width*j + i_width*i_height*k + i_depth*i_width*i_height*l) = atoi(param);
                    //sleep(1);
                } 
            }
        }
    }

    return array;

}

int * bias_load(FILE * fd, int i_depth)
{
    int i;
    size_t len = 0;
    char param[100];
    char * line = NULL;
    int *array = NULL;

    array = (int*) malloc( i_depth * sizeof(int));

    for( i=0 ; i<i_depth ; i++){
        getline(&line, &len, fd);
        sscanf(line, "%s\n", param);
        *(array + i ) = atoi(param);
    }

    return array;
}
