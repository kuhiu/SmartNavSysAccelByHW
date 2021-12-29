


void convolucion2d (int *input, int input_shape_width, int input_shape_height, int input_depth, 
                    int *kernel, int kernel_size, int *bias, int *conv2d_1, int output_conv1_width, int output_conv1_height, int filtros) 
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
                    for ( k = 0; k < CONV1_KERNEL_SIZE; ++k)           // Filas del kernel 
                    {
                        for ( l = 0; l < CONV1_KERNEL_SIZE; ++l)       // Columnas del kernel 
                        {
                            *(conv2d_1 + j + i*output_conv1_width + c*output_conv1_width*output_conv1_height) += *(input + input_shape_width*(k + CONV1_STRIDE*i) + (l + CONV1_STRIDE*j + input_shape_width*input_shape_height*v) ) * *(kernel + l + k*CONV1_KERNEL_SIZE + c*CONV1_KERNEL_SIZE*CONV1_KERNEL_SIZE);
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


void fire_layer (   int *input, int input_shape_width, int input_shape_height, int input_depth, 
                    int *output, int output_shape_width, int output_shape_width, int output_shape_width,
                    int *kernel_s1x1, int *bias_s1x1, int s1x1, 
                    int *kernel_e1x1, int *bias_e1x1, int e1x1, 
                    int *kernel_e3x3, int *bias_e3x3, int e3x3      ) 
{
    int *conv2d_s1x1 = (int*) malloc( input_shape_width*input_shape_height*s1x1*sizeof(int) );
    int *conv2d_e1x1 = (int*) malloc( input_shape_width*input_shape_height*e1x1*sizeof(int) );
    int *conv2d_e3x3 = (int*) malloc( input_shape_width*input_shape_height*e3x3*sizeof(int) );

    convolucion2d(  input, input_shape_width, input_shape_height, input_depth, 
                    kernel_s1x1, 1, bias_s1x1, conv2d_s1x1, input_shape_width, input_shape_height, s1x1); 

    convolucion2d(  conv2d_s1x1, input_shape_width, input_shape_height, s1x1, 
                    kernel_e1x1, 1, bias_e1x1, conv2d_e1x1, input_shape_width, input_shape_height, e1x1);

    convolucion2d(  conv2d_s1x1, input_shape_width, input_shape_height, s1x1,  
                    kernel_e3x3, 3, bias_e3x3, conv2d_e3x3, input_shape_width, input_shape_height, e3x3);   

}