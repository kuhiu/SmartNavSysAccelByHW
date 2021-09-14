void convolucion2d_2 (float *input, int input_shape_width, int input_shape_height, int input_depth, 
                    float *kernel, int kernel_size, 
                    float *bias, 
                    int stride,
                    int Relu,
                    float *conv2d_1, int output_conv1_width, int output_conv1_height, int filtros) 
{
    int i, j, k, l, c, v;
    int conv2d_1_row, conv2d_1_filter; 
    int h;
    int kernel_depth;
    int input_channel, input_StepsFilter, input_StepsFilterROW, input_row;

    for ( c = 0; c < filtros ; ++c)   // depth o filtros
    {
        conv2d_1_filter = c*output_conv1_width*output_conv1_height;
        h = c*input_depth*kernel_size*kernel_size;
        for ( i = 0; i < output_conv1_height; ++i)          // Filas
        {
            conv2d_1_row = i*output_conv1_width;
            input_StepsFilterROW = input_shape_width*stride*i;
            for ( j = 0; j < output_conv1_width; ++j)       // Columnas
            {
                input_StepsFilter = stride*j;
                for ( v = 0; v < input_depth ; ++v)   // depth or channel input
                {
                    kernel_depth = v*kernel_size*kernel_size;
                    input_channel = input_shape_width*input_shape_height*v;


//                    pixel = vld1_u8(&data);                                     
//                    pixel_last = data[8];                                       
//                    result = vmul_u8(kernel,pixel); 


                    for ( k = 0; k < kernel_size; ++k)           // Filas del kernel 
                    {
                        input_row = input_shape_width*k;
                        for ( l = 0; l < kernel_size; ++l)       // Columnas del kernel 
                            *(conv2d_1 + j + conv2d_1_row + conv2d_1_filter) += *(input + l + input_row + input_StepsFilterROW + input_StepsFilter + input_channel ) * *(kernel + l + k*kernel_size + kernel_depth + h);
                    }
                }
            }   
        }
    }
    //printVector(conv2d_1, output_conv1_width, output_conv1_height, filtros);
}