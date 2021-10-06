#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define BYTE2READ 1*4

int main (void)
{
    printf("Llegue \n");
    int fd;
    int ubuff[BYTE2READ];

    if ( (fd = open("/dev/chardev_hcsr04_EMIOgpio", O_RDWR)) == -1)
    {
        //perror("open");
        printf("Error abriendo leds_control_chardev\n");
        return -1;
    }

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
