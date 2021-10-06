#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTE2READ 1*4

#define ADELANTE    (0x1<<0) | (0x0<<13) | (0x1<<9) | (0x0<<14) 
#define FRENAR      (0x0<<0) | (0x0<<13) | (0x0<<9) | (0x0<<14)
#define IZQUIERDA   (0x1<<0) | (0x0<<13) | (0x0<<9) | (0x1<<14)
#define DERECHA     (0x0<<0) | (0x1<<13) | (0x1<<9) | (0x0<<14) 
#define ATRAS       (0x0<<0) | (0x1<<13) | (0x0<<9) | (0x1<<14) 

int main (int argc, char *argv[])
{
    int fd;
    uint32_t buff_send[BYTE2READ];
    int ubuff[BYTE2READ];

    *buff_send = atoi(argv[1]) ;

    switch (*buff_send)
    {
        case 0:
            *buff_send = FRENAR;
            break;
        case 1:
            *buff_send = ADELANTE;
            break;
        case 2:
            *buff_send = IZQUIERDA;
            break;
        case 3:
            *buff_send = DERECHA;
            break;
        case 4:
            *buff_send = ATRAS;
            break;
        default:
            *buff_send = FRENAR;
            break;
    }

    if ( (fd = open("/dev/chardev_MIOgpio_PS", O_RDWR)) == -1)
    {
        //perror("open");
        printf("Error abriendo chardev_MIOgpio_PS\n");
        return -1;
    }

    //if ( ( write(fd, buff_send, BYTE2READ)) == -1)
    //{
    //    //perror("close"):
    //    printf("Error escribiendo leds_control_chardev\n");
    //    return -1;
    //}

    if ( ( read(fd, ubuff, BYTE2READ)) == -1)
    {
        //perror("close"):
        printf("Error leyendo leds_control_chardev\n");
        return -1;
    }
    printf("Distancia = %f cms \n", (*ubuff*0.01715));

    close(fd);
    return 0;
}
