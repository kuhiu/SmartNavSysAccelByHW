#include "main.h"

// Imgage size
//int img_width, img_height;

// Read png lib
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}


int main(void)
{    
    struct timespec begin, end; 

    // Imagen de entrada
    int mem_fd;
    float *img              = NULL;
    float *img_padded1      = NULL;
    float *img_padded2      = NULL;

    // Conv1                                         // Conv12  
    float *conv1_kernel     = NULL;                  float *conv12_kernel    = NULL;                                     
    float *conv1_bias       = NULL;                  float *conv12_bias      = NULL;                                                        
    float *conv1_output     = NULL;                  float *conv12_output    = NULL;                                        
    int output_conv1_width, output_conv1_height;     int output_conv12_width, output_conv12_height;                                    
    
    // maxpool
    float *max_pool_1       = NULL;
    int pool_width_1, pool_height_1;
    float *max_pool_3       = NULL;
    int pool_width_3, pool_height_3;
    float *max_pool_5       = NULL;
    int pool_width_5, pool_height_5;

    // fire 2 layer                        // fire 3 layer                        // fire 4 layer       
    float *fire_2               = NULL;    float *fire_3 = NULL;                  float *fire_4               = NULL;
    float *fire_2_kernel_s1x1   = NULL;    float *fire_3_kernel_s1x1   = NULL;    float *fire_4_kernel_s1x1   = NULL;
    float *fire_2_bias_s1x1     = NULL;    float *fire_3_bias_s1x1     = NULL;    float *fire_4_bias_s1x1     = NULL;
    float *fire_2_kernel_e1x1   = NULL;    float *fire_3_kernel_e1x1   = NULL;    float *fire_4_kernel_e1x1   = NULL;
    float *fire_2_bias_e1x1     = NULL;    float *fire_3_bias_e1x1     = NULL;    float *fire_4_bias_e1x1     = NULL;
    float *fire_2_kernel_e3x3   = NULL;    float *fire_3_kernel_e3x3   = NULL;    float *fire_4_kernel_e3x3   = NULL;
    float *fire_2_bias_e3x3     = NULL;    float *fire_3_bias_e3x3     = NULL;    float *fire_4_bias_e3x3     = NULL;

    // fire 5 layer                        // fire 6 layer                        // fire 7 layer                  
    float *fire_5               = NULL;    float *fire_6               = NULL;    float *fire_7               = NULL;
    float *fire_5_kernel_s1x1   = NULL;    float *fire_6_kernel_s1x1   = NULL;    float *fire_7_kernel_s1x1   = NULL;
    float *fire_5_bias_s1x1     = NULL;    float *fire_6_bias_s1x1     = NULL;    float *fire_7_bias_s1x1     = NULL;
    float *fire_5_kernel_e1x1   = NULL;    float *fire_6_kernel_e1x1   = NULL;    float *fire_7_kernel_e1x1   = NULL;
    float *fire_5_bias_e1x1     = NULL;    float *fire_6_bias_e1x1     = NULL;    float *fire_7_bias_e1x1     = NULL;
    float *fire_5_kernel_e3x3   = NULL;    float *fire_6_kernel_e3x3   = NULL;    float *fire_7_kernel_e3x3   = NULL;
    float *fire_5_bias_e3x3     = NULL;    float *fire_6_bias_e3x3     = NULL;    float *fire_7_bias_e3x3     = NULL;

    // fire 8 layer                        // fire 9 layer                        // fire 10 layer  
    float *fire_8               = NULL;    float *fire_9               = NULL;    float *fire_10              = NULL;
    float *fire_8_kernel_s1x1   = NULL;    float *fire_9_kernel_s1x1   = NULL;    float *fire_10_kernel_s1x1  = NULL;
    float *fire_8_bias_s1x1     = NULL;    float *fire_9_bias_s1x1     = NULL;    float *fire_10_bias_s1x1    = NULL;
    float *fire_8_kernel_e1x1   = NULL;    float *fire_9_kernel_e1x1   = NULL;    float *fire_10_kernel_e1x1  = NULL;
    float *fire_8_bias_e1x1     = NULL;    float *fire_9_bias_e1x1     = NULL;    float *fire_10_bias_e1x1    = NULL;
    float *fire_8_kernel_e3x3   = NULL;    float *fire_9_kernel_e3x3   = NULL;    float *fire_10_kernel_e3x3  = NULL;
    float *fire_8_bias_e3x3     = NULL;    float *fire_9_bias_e3x3     = NULL;    float *fire_10_bias_e3x3    = NULL;

    // fire 11 layer                    
    float *fire_11               = NULL;
    float *fire_11_kernel_s1x1   = NULL;
    float *fire_11_bias_s1x1     = NULL;
    float *fire_11_kernel_e1x1   = NULL;
    float *fire_11_bias_e1x1     = NULL;
    float *fire_11_kernel_e3x3   = NULL;
    float *fire_11_bias_e3x3     = NULL;

    // Post-processing cnn
    float *pred_class           = NULL;
    float *pred_conf            = NULL;
    float *pred_class_softmax   = NULL;
    float *pred_conf_sigmod     = NULL;
    float *prob                 = NULL;
    float *box_predicted        = NULL;
    float *anchors              = NULL; 

    float xmax, xmin, ymax, ymin;
    float delta_x, delta_y, delta_w, delta_h, max=0;
    float box_center_x, box_center_y, box_width, box_height;  

    int index_Ofmax=0;            
    int i,j,k;

    int *img_write              = NULL;
    int *img_read               = NULL;
    int red, green, blue;

    // Escribo STATE.txt
    int DeltaX_left, DeltaX_right;
    char Tipo[20];
    char Direccion[20];

    // Semaforo
    key_t key;              
    int semid, fdp;
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op = -1; /* set to allocate resource */
    sb.sem_flg = SEM_UNDO;

    // File
    FILE* fdd_State;

    // Leds
    int fd_leds;
    uint32_t leds;

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////   State TXT  //////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////
  if ( (fdd_State = fopen("/home/root/Tesis/Apps/state.txt", "r+")) == NULL)
  {
      printf("Error abriendo state.txt\n");
      return -1;
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Inicializar semaforo  /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if((fdp=open("SemFile", O_RDONLY | O_CREAT, 0777))==-1){
        perror(" Open");
        exit(1);
    }
    if((key = ftok("SemFile", 'E'))==-1){
        perror(" ftok ");
        close(fdp);
        exit(1);
    }
    if ((semid = initsem(key, 1)) == -1) {      /* Configura el semaforo */
        perror("initsem");
        close(fdp);
        exit(1);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Control de leds       /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( (fd_leds = open("/dev/chardev_leds_EMIOgpio_PL", O_RDWR)) == -1){
        //perror("open");
        printf("Error abriendo/dev/chardev_leds_EMIOgpio_PL %d\n", fd_leds);
        return -1;
    }

////////////////////////////////        Leo el header       ////////////////////////////////////////////////////////
    //read_png_file("./test5.png");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////       Memoria dinamica     ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    img  = (float*) malloc( IMG_WIDTH*IMG_HEIGHT*3*sizeof(float) );
    //img2 = (float*) malloc( img_width*img_height*3*sizeof(float) );
////////////////////////////////        Leo la imagen       ////////////////////////////////////////////////////////
    //get_png_file(img);
    //printf("sysconf(_SC_PAGE_SIZE) es: %ld y %ld\n", sysconf(_SC_PAGE_SIZE), (FRAMEBUFFER_OFFSET/sysconf(_SC_PAGE_SIZE)) );
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if ( mem_fd == -1)
    {
        printf("Open /dev/mem Failed\n");
        return 1;
    }
    img_write = mmap(NULL, IMG_WIDTH*IMG_HEIGHT*3*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, (off_t)FRAMEBUFFER_WRITE_OFFSET );	// phys_addr should be page-aligned.	

    if(img_write == MAP_FAILED){
        printf("Mapping Failed\n");
        printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        return 1;
    }

    img_read = mmap(NULL, IMG_WIDTH*IMG_HEIGHT*3*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, (off_t)FRAMEBUFFER_READ_OFFSET );	// phys_addr should be page-aligned.	

    if(img_read == MAP_FAILED){
        printf("Mapping Failed\n");
        printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        return 1;
    }

    signal(SIGINT, intHandler);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    Cargo los parametros    ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Cargo los parametros\n");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////    Leo txt para kernel y bias conv1    ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv1_kernel = weight_load("./files_fromTrain/parametros/conv2d_1:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv1_bias = bias_load("./files_fromTrain/parametros/conv2d_1:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////    Leo txt para las conv2d y bias de fire_2   ////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_2_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire2:squeeze1x1:kernel:0.txt");
    fire_2_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire2:expand1x1:kernel:0.txt");
    fire_2_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire2:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_2_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire2:squeeze1x1:bias:0.txt");
    fire_2_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire2:expand1x1:bias:0.txt");
    fire_2_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire2:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_3   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_3_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire3:squeeze1x1:kernel:0.txt");
    fire_3_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire3:expand1x1:kernel:0.txt");
    fire_3_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire3:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_3_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire3:squeeze1x1:bias:0.txt");
    fire_3_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire3:expand1x1:bias:0.txt");
    fire_3_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire3:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_4   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_4_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire4:squeeze1x1:kernel:0.txt");
    fire_4_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire4:expand1x1:kernel:0.txt");
    fire_4_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire4:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_4_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire4:squeeze1x1:bias:0.txt");
    fire_4_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire4:expand1x1:bias:0.txt");
    fire_4_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire4:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_5   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_5_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire5:squeeze1x1:kernel:0.txt");
    fire_5_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire5:expand1x1:kernel:0.txt");
    fire_5_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire5:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_5_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire5:squeeze1x1:bias:0.txt");
    fire_5_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire5:expand1x1:bias:0.txt");
    fire_5_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire5:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_6   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_6_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire6:squeeze1x1:kernel:0.txt");
    fire_6_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire6:expand1x1:kernel:0.txt");
    fire_6_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire6:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_6_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire6:squeeze1x1:bias:0.txt");
    fire_6_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire6:expand1x1:bias:0.txt");
    fire_6_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire6:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_7   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_7_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire7:squeeze1x1:kernel:0.txt");
    fire_7_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire7:expand1x1:kernel:0.txt");
    fire_7_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire7:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_7_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire7:squeeze1x1:bias:0.txt");
    fire_7_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire7:expand1x1:bias:0.txt");
    fire_7_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire7:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_8   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_8_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire8:squeeze1x1:kernel:0.txt");
    fire_8_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire8:expand1x1:kernel:0.txt");
    fire_8_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire8:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_8_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire8:squeeze1x1:bias:0.txt");
    fire_8_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire8:expand1x1:bias:0.txt");
    fire_8_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire8:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_9   //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_9_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire9:squeeze1x1:kernel:0.txt");
    fire_9_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire9:expand1x1:kernel:0.txt");
    fire_9_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire9:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_9_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire9:squeeze1x1:bias:0.txt");
    fire_9_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire9:expand1x1:bias:0.txt");
    fire_9_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire9:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_10  //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_10_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire10:squeeze1x1:kernel:0.txt");
    fire_10_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire10:expand1x1:kernel:0.txt");
    fire_10_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire10:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_10_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire10:squeeze1x1:bias:0.txt");
    fire_10_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire10:expand1x1:bias:0.txt");
    fire_10_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire10:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////    Leo txt para kernel y bias de fire_11  //////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_11_kernel_s1x1 = weight_load("./files_fromTrain/parametros/fire11:squeeze1x1:kernel:0.txt");
    fire_11_kernel_e1x1 = weight_load("./files_fromTrain/parametros/fire11:expand1x1:kernel:0.txt");
    fire_11_kernel_e3x3 = weight_load("./files_fromTrain/parametros/fire11:expand3x3:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_11_bias_s1x1 = bias_load("./files_fromTrain/parametros/fire11:squeeze1x1:bias:0.txt");
    fire_11_bias_e1x1 = bias_load("./files_fromTrain/parametros/fire11:expand1x1:bias:0.txt");
    fire_11_bias_e3x3 = bias_load("./files_fromTrain/parametros/fire11:expand3x3:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////    Leo txt para kernel y bias conv12    ///////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv12_kernel = weight_load("./files_fromTrain/parametros/conv12:kernel:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    conv12_bias = bias_load("./files_fromTrain/parametros/conv12:bias:0.txt");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////    Delta box    ///////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Delta box
    anchors = anchorBox_load("./files_fromTrain/ANCHOR_BOX.txt");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////   Pre-Processing y Dinamic mem  ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    img_padded1 = (float*) calloc( (IMG_WIDTH+1)*(IMG_HEIGHT+1)*3 , sizeof(float) );
    output_conv1_width  = (((IMG_WIDTH  - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1); 
    output_conv1_height = (((IMG_HEIGHT - CONV1_KERNEL_SIZE + CONV1_PAD)/CONV1_STRIDE) + 1);
    conv1_output = (float*) calloc( output_conv1_width*output_conv1_height*CONV1_FILTERS, sizeof(float) );
    pool_width_1  = output_conv1_width/2;
    pool_height_1 = output_conv1_height/2;
    max_pool_1 = (float*) calloc( ( pool_width_1*pool_height_1*CONV1_FILTERS ), sizeof(float));
    fire_2 = (float*) calloc( ( pool_width_1*pool_height_1*(FIRE2_e1x1+FIRE2_e3x3) ), sizeof(float));
    fire_3 = (float*) calloc( ( pool_width_1*pool_height_1*(FIRE3_e1x1+FIRE3_e3x3) ), sizeof(float));
    pool_width_3  = pool_width_1/2;
    pool_height_3 = pool_height_1/2;
    max_pool_3 = (float*) calloc( ( pool_width_3*pool_height_3*MAXPOOL3_FILTERS ), sizeof(float));
    fire_4 = (float*) calloc( ( pool_width_3*pool_height_3*(FIRE4_e1x1+FIRE4_e3x3) ), sizeof(float));
    fire_5 = (float*) calloc( ( pool_width_3*pool_height_3*(FIRE5_e1x1+FIRE5_e3x3) ), sizeof(float));
    pool_width_5  = (pool_width_3)/2;
    pool_height_5 = (pool_height_3)/2;
    max_pool_5 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE5_e1x1+FIRE5_e3x3) ), sizeof(float));
    fire_6 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE6_e1x1+FIRE6_e3x3) ), sizeof(float));
    fire_7 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE7_e1x1+FIRE7_e3x3) ), sizeof(float));
    fire_8 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE8_e1x1+FIRE8_e3x3) ), sizeof(float));
    fire_9 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE9_e1x1+FIRE9_e3x3) ), sizeof(float));
    fire_10 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE10_e1x1+FIRE10_e3x3) ), sizeof(float));
    fire_11 = (float*) calloc( ( pool_width_5*pool_height_5*(FIRE11_e1x1+FIRE11_e3x3) ), sizeof(float));
    img_padded2 = (float*) calloc( (pool_width_5+CONV12_PAD)*(pool_height_5+CONV12_PAD)*(FIRE11_e1x1+FIRE11_e3x3) , sizeof(float) );
    output_conv12_width  = (((pool_width_5  - CONV12_KERNEL_SIZE + CONV12_PAD)/CONV12_STRIDE) + 1); 
    output_conv12_height = (((pool_height_5 - CONV12_KERNEL_SIZE + CONV12_PAD)/CONV12_STRIDE) + 1);
    conv12_output = (float*) calloc( output_conv12_width*output_conv12_height*CONV12_FILTERS, sizeof(float) );
    pred_class = (float*) malloc( 2700*sizeof(float) );         // Consigo un vector con las predicciones de la clase 
    pred_conf = (float*) malloc( 2700*sizeof(float) );          // Consigo un vector con puntuaciones de confianza
    prob = (float*) malloc( 2700*sizeof(float) );               // probabilidad
    box_predicted = (float*) malloc( 10800*sizeof(float) );     // boxes

while(keepRunning)
{
//////////////////////////////////////   Time measure  /////////////////////////////////////////////////////////////
    printf("Start to measure\n");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begin);	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    max = 0;
    index_Ofmax = 0;
    emptyVector(img_padded1, (IMG_WIDTH+1), (IMG_HEIGHT+1), IMG_CHANNEL);
    emptyVector(conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS);
    emptyVector(max_pool_1, pool_width_1, pool_height_1, CONV1_FILTERS);
    emptyVector(fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3));
    emptyVector(fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3));
    emptyVector(max_pool_3, pool_width_3, pool_height_3, MAXPOOL3_FILTERS);
    emptyVector(fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3));
    emptyVector(fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3));
    emptyVector(max_pool_5, pool_width_5, pool_height_5, MAXPOOL5_FILTERS);
    emptyVector(fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3));
    emptyVector(fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3));
    emptyVector(fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3));
    emptyVector(fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3));
    emptyVector(fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE10_e3x3));
    emptyVector(fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3));
    emptyVector(img_padded2, (pool_width_5+CONV12_PAD), (pool_height_5+CONV12_PAD), (FIRE11_e1x1+FIRE11_e3x3));
    emptyVector(conv12_output, output_conv12_width, output_conv12_height, CONV12_FILTERS);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Leo de RAM 0x0100_0000
    for(j=0;j<IMG_HEIGHT;j++)
    {
        for(i=0;i<IMG_WIDTH;i++)
        {
            *(img + i + IMG_WIDTH*j + IMG_WIDTH*IMG_HEIGHT*0) = (float) ( ( (*(img_read + i + j*IMG_WIDTH) >>  0) & 0xFF)  );
            *(img + i + IMG_WIDTH*j + IMG_WIDTH*IMG_HEIGHT*1) = (float) ( ( (*(img_read + i + j*IMG_WIDTH) >>  8) & 0xFF)  );
            *(img + i + IMG_WIDTH*j + IMG_WIDTH*IMG_HEIGHT*2) = (float) ( ( (*(img_read + i + j*IMG_WIDTH) >> 16) & 0xFF)  );
            //printf("Red %d, Blue %d\n",  ( ( (*(img_read + i + j*IMG_WIDTH) >>  0) & 0xFF)  ), ( ( (*(img_read + i + j*IMG_WIDTH) >>  8) & 0xFF)  ) );
            //*(img + i + IMG_WIDTH*j) = (int)( ( (red & 0xFF) << 0 ) | ( (green & 0xFF) << 8 ) | ( (blue & 0xFF) << 16 ) | ( 0xFF << 24 ) );
        }
    }
    printf("Start to precessing...\n");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////          SqueezeDet        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    padding(    img, IMG_WIDTH, IMG_HEIGHT, 3,
                img_padded1,
                1, 1); 
    convolucion2d(  img_padded1, (IMG_WIDTH+1), (IMG_HEIGHT+1), 3,                          // Entrada: pointer, ancho, alto, profundidad
                    conv1_kernel, CONV1_KERNEL_SIZE,                                        // Kernel: pointer, size
                    conv1_bias,                                                             // Bias: pointer
                    2,                                                                      // Stride
                    CONV1_RELU,
                    conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS ); // Salida: pointer, ancho, alto, profundidad
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    maxPool2d(  MAXPOOL1_POOL_SIZE, MAXPOOL1_STRIDE, 
                conv1_output, output_conv1_width, output_conv1_height, CONV1_FILTERS, 
                max_pool_1, pool_width_1, pool_height_1,
                MAXPOOL1_PAD);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    max_pool_1, pool_width_1, pool_height_1, CONV1_FILTERS,             // Entrada: pointer, ancho, alto, profundidad
                    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_2_kernel_s1x1, fire_2_bias_s1x1, FIRE2_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_2_kernel_e1x1, fire_2_bias_e1x1, FIRE2_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_2_kernel_e3x3, fire_2_bias_e3x3, FIRE2_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE2_RELU                                          );              // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_2, pool_width_1, pool_height_1, (FIRE2_e1x1+FIRE2_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_3_kernel_s1x1, fire_3_bias_s1x1, FIRE3_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_3_kernel_e1x1, fire_3_bias_e1x1, FIRE3_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_3_kernel_e3x3, fire_3_bias_e3x3, FIRE3_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE3_RELU                                        );                // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    maxPool2d(  MAXPOOL3_POOL_SIZE, MAXPOOL3_STRIDE, 
                fire_3, pool_width_1, pool_height_1, (FIRE3_e1x1+FIRE3_e3x3), 
                max_pool_3, pool_width_3, pool_height_3,
                MAXPOOL3_PAD);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    max_pool_3, pool_width_3, pool_height_3, (FIRE2_e1x1+FIRE2_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_4_kernel_s1x1, fire_4_bias_s1x1, FIRE4_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_4_kernel_e1x1, fire_4_bias_e1x1, FIRE4_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_4_kernel_e3x3, fire_4_bias_e3x3, FIRE4_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE4_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_4, pool_width_3, pool_height_3, (FIRE4_e1x1+FIRE4_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_5_kernel_s1x1, fire_5_bias_s1x1, FIRE5_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_5_kernel_e1x1, fire_5_bias_e1x1, FIRE5_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_5_kernel_e3x3, fire_5_bias_e3x3, FIRE5_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE5_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    maxPool2d(  MAXPOOL5_POOL_SIZE, MAXPOOL5_STRIDE, 
                fire_5, pool_width_3, pool_height_3, (FIRE5_e1x1+FIRE5_e3x3), 
                max_pool_5, pool_width_5, pool_height_5,
                MAXPOOL5_PAD);
    usleep(500000); // Sleep 0.5 seg.
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    max_pool_5, pool_width_5, pool_height_5, (FIRE5_e1x1+FIRE5_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_6_kernel_s1x1, fire_6_bias_s1x1, FIRE6_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_6_kernel_e1x1, fire_6_bias_e1x1, FIRE6_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_6_kernel_e3x3, fire_6_bias_e3x3, FIRE6_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE6_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_6, pool_width_5, pool_height_5, (FIRE6_e1x1+FIRE6_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_7_kernel_s1x1, fire_7_bias_s1x1, FIRE7_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_7_kernel_e1x1, fire_7_bias_e1x1, FIRE7_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_7_kernel_e3x3, fire_7_bias_e3x3, FIRE7_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE7_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_7, pool_width_5, pool_height_5, (FIRE7_e1x1+FIRE7_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_8_kernel_s1x1, fire_8_bias_s1x1, FIRE8_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_8_kernel_e1x1, fire_8_bias_e1x1, FIRE8_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_8_kernel_e3x3, fire_8_bias_e3x3, FIRE8_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE8_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_8, pool_width_5, pool_height_5, (FIRE8_e1x1+FIRE8_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3),       // Salida: pointer, ancho, alto, profundidad
                    fire_9_kernel_s1x1, fire_9_bias_s1x1, FIRE9_s1x1,                   // kernel_s1x1, bias_s1x1, s1x1
                    fire_9_kernel_e1x1, fire_9_bias_e1x1, FIRE9_e1x1,                   // kernel_e1x1, bias_e1x1, e1x1
                    fire_9_kernel_e3x3, fire_9_bias_e3x3, FIRE9_e3x3,                   // kernel_e3x3, bias_e3x3, e3x3
                    FIRE9_RELU                                      );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_9, pool_width_5, pool_height_5, (FIRE9_e1x1+FIRE9_e3x3),       // Entrada: pointer, ancho, alto, profundidad
                    fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE8_e3x3),     // Salida: pointer, ancho, alto, profundidad
                    fire_10_kernel_s1x1, fire_10_bias_s1x1, FIRE10_s1x1,                // kernel_s1x1, bias_s1x1, s1x1
                    fire_10_kernel_e1x1, fire_10_bias_e1x1, FIRE10_e1x1,                // kernel_e1x1, bias_e1x1, e1x1
                    fire_10_kernel_e3x3, fire_10_bias_e3x3, FIRE10_e3x3,                // kernel_e3x3, bias_e3x3, e3x3
                    FIRE10_RELU                                     );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    fire_layer (    fire_10, pool_width_5, pool_height_5, (FIRE10_e1x1+FIRE10_e3x3),    // Entrada: pointer, ancho, alto, profundidad
                    fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3),    // Salida: pointer, ancho, alto, profundidad
                    fire_11_kernel_s1x1, fire_11_bias_s1x1, FIRE11_s1x1,                // kernel_s1x1, bias_s1x1, s1x1
                    fire_11_kernel_e1x1, fire_11_bias_e1x1, FIRE11_e1x1,                // kernel_e1x1, bias_e1x1, e1x1
                    fire_11_kernel_e3x3, fire_11_bias_e3x3, FIRE11_e3x3,                // kernel_e3x3, bias_e3x3, e3x3
                    FIRE11_RELU                                     );                  // Relu?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    padding(    fire_11, pool_width_5, pool_height_5, (FIRE11_e1x1+FIRE11_e3x3),
                img_padded2,
                CONV12_PAD, CONV12_PAD); 
    convolucion2d(  img_padded2, (pool_width_5+CONV12_PAD), (pool_height_5+CONV12_PAD), (FIRE11_e1x1+FIRE11_e3x3),   // Entrada: pointer, ancho, alto, profundidad
                    conv12_kernel, CONV12_KERNEL_SIZE,                                                              // Kernel: pointer, size
                    conv12_bias,                                                                                    // Bias: pointer
                    CONV12_STRIDE,                                                                                  // Stride
                    CONV12_RELU,                                                                                    // Relu?
                    conv12_output, output_conv12_width, output_conv12_height, CONV12_FILTERS );                     // Salida: pointer, ancho, alto, profundidad
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for(j=0;j<output_conv12_height;j++)
    {
        for(i=0;i<output_conv12_width;i++)
        {
            for(k=0;k<54;k++)
            {
                //*(reshape + l + CONV12_FILTERS*i + output_conv12_height*CONV12_FILTERS*k ) = *(conv12_output + k + output_conv12_width*i + output_conv12_width*output_conv12_height*l);
                if ( k < 9 )
                    *(pred_class + k + 9*i + 9*output_conv12_width*j) = *(conv12_output + i + output_conv12_width*j + output_conv12_width*output_conv12_height*k);
                if ( (k >= 9) && (k < 18))
                    *(pred_conf + (k - 9) + 9*i + 9*output_conv12_width*j) = *(conv12_output + i + output_conv12_width*j + output_conv12_width*output_conv12_height*k);
                if ( k >= 18)
                    *(box_predicted + (k - 18) + 36*i + 36*output_conv12_width*j) = *(conv12_output + i + output_conv12_width*j + output_conv12_width*output_conv12_height*k);
            }
        }
    }

    // Los normalizo 
    pred_class_softmax = softmax(pred_class, 2700); 

    // Los normalizo 
    pred_conf_sigmod = sigmoid(pred_conf, 2700);

    // Probabilidad
    for(i=0;i<2700;i++)
        *(prob + i) = *(pred_class_softmax + i) * *(pred_conf_sigmod + i);

    // Max prob
    for(i=0;i<2700;i++)
    {
        if( *(prob + i) > max)
        {
            max = *(prob + i);
            index_Ofmax = i;
        }
    }
    printf("Probabilidad maxima encontrada: %f \n", max);
    // Search delta max prob
    // Boxes from delta
    delta_x = *(box_predicted + index_Ofmax*4 + 0);
    delta_y = *(box_predicted + index_Ofmax*4 + 1);
    delta_w = *(box_predicted + index_Ofmax*4 + 2);
    delta_h = *(box_predicted + index_Ofmax*4 + 3);

    // Consigo los anchors box
    box_center_x = *(anchors + 0 + 4*index_Ofmax) + delta_x * *(anchors + 2 + 4*index_Ofmax);   //box_center_x = anchor_x + delta_x * anchor_w;
    box_center_y = *(anchors + 1 + 4*index_Ofmax) + delta_y * *(anchors + 3 + 4*index_Ofmax);   //box_center_y = anchor_y + delta_y * anchor_h;
    box_width    = *(anchors + 2 + 4*index_Ofmax) * exp(delta_w);                               //box_width    = anchor_w * safe_exp_np(delta_w, config.EXP_THRESH);
    box_height   = *(anchors + 3 + 4*index_Ofmax) * exp(delta_h);                               //box_height   = anchor_h * safe_exp_np(delta_h, config.EXP_THRESH);
    //anchor_x = config.ANCHOR_BOX[:, 0] //anchor_y = config.ANCHOR_BOX[:, 1] //anchor_w = config.ANCHOR_BOX[:, 2] //anchor_h = config.ANCHOR_BOX[:, 3]
 
    // extremos de la caja
    xmin = box_center_x-box_width/2;
    ymin = box_center_y-box_height/2;
    xmax = box_center_x+box_width/2;
    ymax = box_center_y+box_height/2;
    
    // Recorto boxes si esta por fuera de la imagen
    // Escribir...

    // Dibujo la caja, informo resultados
    if (max > PROB_TRSH)
    {
        process_png_file(img, xmin, xmax, ymin, ymax);

        DeltaX_left  = (int)IMG_WIDTH/2 - (int)xmin;
        DeltaX_right = (int)xmax - (int)IMG_WIDTH/2;

        strcpy(Tipo,"TOMATE      ");

        // Enciendo el led de reconocimiento
        if ( read(fd_leds, &leds, BYTE2READ) == -1){
            perror("read");
            return -1;
        }
        leds = leds | 0x01;    /* Seteo el leds[0] */
        printf("Voy a escribir leds = %d\n", leds);
        if ( ( write(fd_leds, &leds, BYTE2READ)) == -1){
            printf("Error escribiendo\n");
            return -1;
        }

        sb.sem_op = -1;         /* Asignar recurso */
        if (semop(semid, &sb, 1) == -1) {           /* semop setea, chequea o limpia uno o varios semaforos */
            perror("semop");
            exit(1);
        }
        if ( DeltaX_left >=  DeltaX_right)
        {
            // Escribo en STATE que tengo que ir a la derecha 
            printf("Tengo que ir a la derecha \n");
            strcpy(Direccion,"DERECHA     ");
            put_system_outputs(fdd_State, Tipo, Direccion);
        }
        else 
        {
            // Escribo en STATE que tengo que ir a la izquierda
            printf("Tengo que ir a la izquierda \n");
            strcpy(Direccion,"IZQUIERDA   ");
            put_system_outputs(fdd_State, Tipo, Direccion);
        }
        sb.sem_op = 1;          /* Libera el recurso */
        if (semop(semid, &sb, 1) == -1) {
            perror("semop");
            exit(1);
        }
    }
    else 
    {
        // Escribo en STATE que no se reconocio el obstaculo
        printf("Direccion y obstaculo desconocidos \n");
        strcpy(Direccion,"DESCONOCIDO ");
        strcpy(Tipo,"DESCONOCIDO ");
        put_system_outputs(fdd_State, Tipo, Direccion);
        // Apago el led de reconocimiento
        if ( read(fd_leds, &leds, BYTE2READ) == -1){
            perror("read");
            return -1;
        }
        leds = leds & ~(uint32_t)0x01;    /* Borro el leds[0] */
        printf("Voy a escribir leds = %d\n", leds);
        if ( ( write(fd_leds, &leds, BYTE2READ)) == -1){
            printf("Error escribiendo\n");
            return -1;
        }

    }
    // Guardo la imagen
    //write_png_file("./test5MODIF.png");

    // Guardo en RAM 0x0200_0000
    for(j=0;j<IMG_HEIGHT;j++)
    {
        for(i=0;i<IMG_WIDTH;i++)
        {
            red   = (int) *(img + i + IMG_WIDTH*j + IMG_WIDTH*IMG_HEIGHT*0);
            green = (int) *(img + i + IMG_WIDTH*j + IMG_WIDTH*IMG_HEIGHT*1);
            blue  = (int) *(img + i + IMG_WIDTH*j + IMG_WIDTH*IMG_HEIGHT*2);
            //printf("Red %d, green %d, blue %d, composicion %d\n", red, green, blue, ( ( (red & 0xFF) << 24 ) & ( (green & 0xFF) << 16 ) & ( (blue & 0xFF) << 8 ) ));
            *(img_write + i + IMG_WIDTH*j) = (int)( ( (red & 0xFF) << 0 ) | ( (green & 0xFF) << 8 ) | ( (blue & 0xFF) << 16 ) | ( 0xFF << 24 ) );

        }
    }

    printf("Finished\n");
    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
    printf("Time executing.. %f \n", elapsed);

    usleep(500000); // Sleep 0.5 seg.

    free(pred_class_softmax);
    free(pred_conf_sigmod);
}
    printf("Sali del loop\n");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////    Libero Memoria dinamica     ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    free(img);
    free(img_padded1);
    free(img_padded2);

    free(conv1_kernel);             free(conv12_kernel);                                     
    free(conv1_bias);               free(conv12_bias);                                                        
    free(conv1_output);             free(conv12_output);                                                                         
    
    free(max_pool_1);               free(max_pool_3);               free(max_pool_5);        

    free(fire_2            );       free(fire_3            );       free(fire_4            );       free(fire_5            ); 
    free(fire_2_kernel_s1x1);       free(fire_3_kernel_s1x1);       free(fire_4_kernel_s1x1);       free(fire_5_kernel_s1x1); 
    free(fire_2_bias_s1x1  );       free(fire_3_bias_s1x1  );       free(fire_4_bias_s1x1  );       free(fire_5_bias_s1x1  ); 
    free(fire_2_kernel_e1x1);       free(fire_3_kernel_e1x1);       free(fire_4_kernel_e1x1);       free(fire_5_kernel_e1x1); 
    free(fire_2_bias_e1x1  );       free(fire_3_bias_e1x1  );       free(fire_4_bias_e1x1  );       free(fire_5_bias_e1x1  ); 
    free(fire_2_kernel_e3x3);       free(fire_3_kernel_e3x3);       free(fire_4_kernel_e3x3);       free(fire_5_kernel_e3x3); 
    free(fire_2_bias_e3x3  );       free(fire_3_bias_e3x3  );       free(fire_4_bias_e3x3  );       free(fire_5_bias_e3x3  );    

    free(fire_6            );       free(fire_7            );       free(fire_8            );       free(fire_9            ); 
    free(fire_6_kernel_s1x1);       free(fire_7_kernel_s1x1);       free(fire_8_kernel_s1x1);       free(fire_9_kernel_s1x1); 
    free(fire_6_bias_s1x1  );       free(fire_7_bias_s1x1  );       free(fire_8_bias_s1x1  );       free(fire_9_bias_s1x1  ); 
    free(fire_6_kernel_e1x1);       free(fire_7_kernel_e1x1);       free(fire_8_kernel_e1x1);       free(fire_9_kernel_e1x1); 
    free(fire_6_bias_e1x1  );       free(fire_7_bias_e1x1  );       free(fire_8_bias_e1x1  );       free(fire_9_bias_e1x1  ); 
    free(fire_6_kernel_e3x3);       free(fire_7_kernel_e3x3);       free(fire_8_kernel_e3x3);       free(fire_9_kernel_e3x3); 
    free(fire_6_bias_e3x3  );       free(fire_7_bias_e3x3  );       free(fire_8_bias_e3x3  );       free(fire_9_bias_e3x3  );    

    free(fire_10            );      free(fire_11            );      free(pred_class        );
    free(fire_10_kernel_s1x1);      free(fire_11_kernel_s1x1);      free(pred_conf         );
    free(fire_10_bias_s1x1  );      free(fire_11_bias_s1x1  );      
    free(fire_10_kernel_e1x1);      free(fire_11_kernel_e1x1);      
    free(fire_10_bias_e1x1  );      free(fire_11_bias_e1x1  );      free(prob              );
    free(fire_10_kernel_e3x3);      free(fire_11_kernel_e3x3);      free(box_predicted     );
    free(fire_10_bias_e3x3  );      free(fire_11_bias_e3x3  );      free(anchors           );  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}
