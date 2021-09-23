#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTE2READ 1*4

#define ADELANTE    (0x0<<0) | (0x1<<13) | (0x1<<9) | (0x0<<14) 
#define FRENAR      (0x0<<0) | (0x0<<13) | (0x0<<9) | (0x0<<14)
#define IZQUIERDA   (0x0<<0) | (0x1<<13) | (0x0<<9) | (0x1<<14)
#define DERECHA     (0x1<<0) | (0x0<<13) | (0x1<<9) | (0x0<<14) 

int main (int argc, char *argv[])
{
    int fd;
    uint32_t buff_send[BYTE2READ];

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

    if ( ( write(fd, buff_send, BYTE2READ)) == -1)
    {
        //perror("close"):
        printf("Error escribiendo leds_control_chardev\n");
        return -1;
    }

    close(fd);
    return 0;
}
