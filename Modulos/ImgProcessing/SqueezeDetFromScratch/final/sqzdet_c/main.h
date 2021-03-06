#include <png.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdint.h>

#define BYTE2READ 4

#define FRAMEBUFFER_READ_OFFSET  0x01000000  // Image ram position
#define FRAMEBUFFER_WRITE_OFFSET 0x02000000
#define PROB_TRSH 0.8

#define IMG_WIDTH                 320   // Image size
#define IMG_HEIGHT                240
#define IMG_CHANNEL                 3
#define PIXEL_SIZE                  4   // number of bytes per pixel

#define CONV1_KERNEL_SIZE           3
#define CONV1_PAD                   1   // Asumo pad_width = pad_height
#define CONV1_FILTERS              64
#define CONV1_ACTIVATION            1     
#define CONV1_STRIDE                2
#define CONV1_RELU                  1

#define MAXPOOL1_POOL_SIZE          3 
#define MAXPOOL1_PAD                1   // Asumo pad_width = pad_height
#define MAXPOOL1_FILTERS           64     
#define MAXPOOL1_STRIDE             2 

#define FIRE2_s1x1                 16
#define FIRE2_e1x1                 64
#define FIRE2_e3x3                 64
#define FIRE2_RELU                  1

#define FIRE3_s1x1                 16
#define FIRE3_e1x1                 64
#define FIRE3_e3x3                 64
#define FIRE3_RELU                  1

#define MAXPOOL3_POOL_SIZE          3 
#define MAXPOOL3_PAD                1   // Asumo pad_width = pad_height
#define MAXPOOL3_FILTERS          128     
#define MAXPOOL3_STRIDE             2 

#define FIRE4_s1x1                 32
#define FIRE4_e1x1                128
#define FIRE4_e3x3                128
#define FIRE4_RELU                  1

#define FIRE5_s1x1                 32
#define FIRE5_e1x1                128
#define FIRE5_e3x3                128
#define FIRE5_RELU                  1

#define MAXPOOL5_POOL_SIZE          3 
#define MAXPOOL5_PAD                1   // Asumo pad_width = pad_height
#define MAXPOOL5_FILTERS          256     
#define MAXPOOL5_STRIDE             2 

#define FIRE6_s1x1                 48
#define FIRE6_e1x1                192
#define FIRE6_e3x3                192
#define FIRE6_RELU                  1

#define FIRE7_s1x1                 48
#define FIRE7_e1x1                192
#define FIRE7_e3x3                192
#define FIRE7_RELU                  1

#define FIRE8_s1x1                 64
#define FIRE8_e1x1                256
#define FIRE8_e3x3                256
#define FIRE8_RELU                  1

#define FIRE9_s1x1                 64
#define FIRE9_e1x1                256
#define FIRE9_e3x3                256
#define FIRE9_RELU                  1

#define FIRE10_s1x1                96
#define FIRE10_e1x1               384
#define FIRE10_e3x3               384
#define FIRE10_RELU                  1

#define FIRE11_s1x1                96
#define FIRE11_e1x1               384
#define FIRE11_e3x3               384
#define FIRE11_RELU                  1

#define CONV12_KERNEL_SIZE          3
#define CONV12_PAD                  2   // Asumo pad_width = pad_height
#define CONV12_FILTERS      9*(1+1+4)   // anchors per grid * (classes + 1 + 4)
#define CONV12_ACTIVATION           1     
#define CONV12_STRIDE               1
#define CONV12_RELU                 0

void read_png_file(char *filename);
void write_png_file(char *filename);
void process_png_file(float *img, int xmin, int xmax, int ymin, int ymax);
void get_png_file(float *img);
void convolucion2d (float *input, int input_shape_width, int input_shape_height, int input_depth, float *kernel, int kernel_size, float *bias, int stride, int Relu, float *conv2d_1, int output_conv1_width, int output_conv1_height, int filtros); 
void printVector(float *kernel, int i_width, int i_height, int i_depth, int i_filters);
void maxPool2d( int pool_size, int stride, float *in, int input_shape_width, int input_shape_height, int input_depth, float *max_pool, int pool_width, int pool_height,int padding );
void fire_layer(float *, int, int, int, float*, int, int, int, float *, float*, int, float*, float*, int, float*, float*, int, int); 
void padding(float *input, int input_shape_width, int input_shape_height, int input_depth, float *output, int pad_along_width, int pad_along_height);
float *weight_load(char *filename);
float *bias_load(char *filename);
float *anchorBox_load(char *filename);
float *softmax( float *x, int  i_width);
float *sigmoid( float *x, int  i_width);
void emptyVector(float *in, int i_width, int i_height, int i_depth);
int initsem(key_t key, int nsems);
int put_system_outputs(FILE* fdd_State, char Tipo[], char Direccion[]);

// Read png lib
//extern int img_width, img_height;
extern png_byte color_type;
extern png_byte bit_depth;
extern png_bytep *row_pointers;

// Semaforo
#define MAX_RETRIES 10
union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};