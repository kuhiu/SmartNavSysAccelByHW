#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <png.h>


#define FRAMEBUFFER_OFFSET  0x01000000  // Image ram position

#define IMG_WIDTH             320   // Image size
#define IMG_HEIGHT            240
#define PIXEL_SIZE              4   // number of bytes per pixel

#define CONV1_KERNEL_SIZE       3
#define CONV1_PAD               1   // Asumo pad_width = pad_height
#define CONV1_FILTERS           2
#define CONV1_ACTIVATION        1     
#define CONV1_STRIDE            2

#define MAXPOOL1_POOL_SIZE      3 
#define MAXPOOL1_PAD            1   // Asumo pad_width = pad_height
#define MAXPOOL1_FILTERS        2     
#define MAXPOOL1_STRIDE         2 

#define FIRE2_s1x1              2
#define FIRE2_e1x1              2
#define FIRE2_e3x3              2

#define FIRE3_s1x1              2
#define FIRE3_e1x1              2
#define FIRE3_e3x3              2

void read_png_file(char *filename);
void process_png_file(int *img);
void convolucion2d (int *input, int input_shape_width, int input_shape_height, int input_depth, int *kernel, int kernel_size, int *bias, int stride, int *conv2d_1, int output_conv1_width, int output_conv1_height, int filtros); 
void printVector(int *in, int in_width, int in_height, int depth);
void maxPool2d( int pool_size, int stride, int *in, int input_shape_width, int input_shape_height, int input_depth, int *max_pool, int pool_width, int pool_height,int padding );
void fire_layer(int *, int, int, int, int*, int, int, int, int *, int*, int, int*, int*, int, int*, int*, int); 
void padding(int *input, int input_shape_width, int input_shape_height, int input_depth, int *output, int pad_along_width, int pad_along_height);
int * weight_load(FILE * fd, int  i_width, int i_height, int i_depth, int i_filters);
int * bias_load(FILE * fd, int i_depth);

// Read png lib
extern int img_width, img_height;
extern png_byte color_type;
extern png_byte bit_depth;
extern png_bytep *row_pointers;


