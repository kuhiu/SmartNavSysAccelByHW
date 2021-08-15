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
    int fd;
    //uint32_t *buff_send;

    printf("Test empieza\n");
    
    if ( (fd = open("/dev/ov7670", O_WRONLY)) == -1)
    {
        //perror("open");
        printf("Error abriendo/dev/ov7670\n");
        return -1;
    }

    /*printf("Llegue\n");

    if ( ( write(fd, buff_send, BYTE2READ)) == -1)
    {
        //perror("close"):
        printf("Error escribiendo ov7670\n");
        return -1;
    }*/

    printf("Test termina\n"); 
    close(fd);
    
    return 0;
}