#include <stdio.h>
#include <stdlib.h>



int main(void)
{
    int i,j,k,l;
    char s_width[20], s_height[20], s_depth[20], s_filters[20];
    int  i_width, i_height, i_depth, i_filters;
    FILE * fd;
    char * line = NULL;
    size_t len = 0;

    char param[100];
    float *array = NULL;

    fd = fopen("/home/martin/Escritorio/Tesis/pImagen/SqueezeNet/squeezedet-keras-master/main/model/parametros/conv2d_1:kernel:0.txt", "r");

    getline(&line, &len, fd);
    printf("line: %s\n", line);
    sscanf(line, " (%[^','], %[^','], %[^','], %[^','])\n", s_width, s_height, s_depth, s_filters);

    printf("Las dimensiones del array son: %sx%sx%sx%s \n", s_width, s_height, s_depth, s_filters);
    i_width = atoi(s_width);
    i_height = atoi(s_height);
    i_depth = atoi(s_depth);
    i_filters = atoi(s_filters);

    printf("Las dimensiones del array son: %dx%dx%dx%d \n", i_width, i_height, i_depth, i_filters);
    array = (float*) calloc( ( i_width*i_height*i_depth*i_filters ), sizeof(float));

    for( l=0 ; l<i_filters ; l++){
        for( k=0 ; k<i_depth ; k++){
            for( j=0 ; j<i_height ; j++){
                for( i=0 ; i<i_width ; i++){
                    getline(&line, &len, fd);
                    sscanf(line, "%s\n", param);
                    //printf("Voy a guardar: %s %f\n", param, atof(param));
                    *(array + i + i_width*j + i_width*i_height*k + i_depth*i_width*i_height*l) = atof(param);
                } 
            }
        }
    }

    //printf("El array es: \n");
    //for( l=0 ; l<i_filters ; l++){
    //    for( k=0 ; k<i_depth ; k++){
    //        for( j=0 ; j<i_height ; j++){
    //            for( i=0 ; i<i_width ; i++){
    //                printf(" %f ", *(array + i + i_width*j + i_width*i_height*k + i_depth*i_width*i_height*l));
    //            } 
    //        }
    //    }
    //}
    //printf("\n");


    return 0;
}