#include "./Funciones/func.h"

int main (int argc, char *argv[])
{
    /* state file */
        int fd_State;
        FILE* fdd_State;
        ssize_t loffset, lread;
        char * line = NULL;
        size_t len = 0;  
        int lineNro;
        char aux[10];

    /* Variables sensores de distancia*/
        int fd_MIOgpio_PS;
        long long int rb_MIOgpio_PS[BYTE2READ_MIOgpio_PS];
        long long int rightSensor=-1, centerSensor=-1, leftSensor=-1;

    /* Variables brujula */
        int fd_brujula;                 // File descriptor para brujula
        int addr_brujula = 0x0D;        // The I2C address brujula
        float heading=0;                // Angulo absoluto al norte
        int xlow  = -12391;             // Calibracion
        int xhigh =   1301;
        int ylow  =  -6210;
        int yhigh =     75;   

    /* Variables encoder */
        int fd_encoder;
        int rb_encoder[BYTE2READ_encoder];
        float revoluciones_rpm_s1, revoluciones_rpm_s2;
        float distance_cm_s1, distance_cm_s2;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////   State TXT  //////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    if ( (fd_State = open("/home/root/Tesis/Apps/state.txt", O_RDWR)) == -1)
    {
        printf("Error abriendo state.txt\n");
        return -1;
    }
    fdd_State=fdopen(fd_State, "r+");
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////  Sensores de distancia  ///////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    if ( (fd_MIOgpio_PS = open("/dev/chardev_MIOgpio_PS", O_RDWR)) == -1)
    {
        printf("Error abriendo chardev_MIOgpio_PS\n");
        return -1;
    }

    if ( read(fd_MIOgpio_PS, rb_MIOgpio_PS, BYTE2READ_MIOgpio_PS) == -1)
    {
        printf("Error leyendo chardev_MIOgpio_PS\n");
        return -1;
    }

    rightSensor  = to_distance_cm(rb_MIOgpio_PS[0]);
    centerSensor = to_distance_cm(rb_MIOgpio_PS[1]);
    leftSensor   = to_distance_cm(rb_MIOgpio_PS[2]);

    printf("Distancia = Left: %lld cms, Center: %lld cms, Right: %lld cms \n", leftSensor, centerSensor, rightSensor);
    
    close(fd_MIOgpio_PS);

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////  Brujula  //////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if ( (fd_brujula = open("/dev/i2c-0", O_WRONLY)) == -1)
    {
        printf("Error abriendo/dev/HMC5883L\n");
        return -1;
    }

    if (ioctl(fd_brujula, I2C_SLAVE, addr_brujula) < 0) {
        printf("Error ioctl HMC5883L: %s\n", strerror(errno));
        return -1;
    }
    
    if (CALIBRACION_ENABLE){
        printf("HMC5883L: Calibracion... \n");
        if ( calibracion_HMC5883L(fd_brujula, &xlow, &xhigh, &ylow, &yhigh) == -1)
        {
            printf("HMC5883L: Fallo la Calibracion... \n");
            return -1;
        }
        printf("HMC5883L: La calibracion termino.\n");
        printf("HMC5883L: xlow: %d, xhigh: %d, ylow: %d, yhigh: %d\n", xlow, xhigh, ylow, yhigh);
    }


    while (1){
        heading = get_headeing_degree(fd_brujula, xlow, xhigh, ylow, yhigh);
        
        if (heading == -1){   // hubo ov
            continue;
        }

        if (heading == -2){   // Data skipped
            continue;
        }

        if (heading == -3){   // Data no available
            usleep(10000); // Sleep 80 mseg
            continue;
        }
        
        printf("heading: %f \n", heading);
        break;

    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////  Encoder  //////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( (fd_encoder = open("/dev/chardev_encoder_EMIOgpio_PL", O_RDWR)) == -1)
    {
        printf("Error abriendo chardev_encoder_EMIOgpio_PL\n");
        return -1;
    }

    if ( ( read(fd_encoder, rb_encoder, BYTE2READ_encoder)) == -1)
    {
        printf("Error leyendo chardev_encoder_EMIOgpio_PL\n");
        return -1;
    }
    printf("Revoluciones: %d - %d \n", rb_encoder[0], rb_encoder[2] );
    printf("Vueltas: %d - %d \n", rb_encoder[1], rb_encoder[3] );
    revoluciones_rpm_s1 = get_revoluciones_rpm(rb_encoder[0]);
    revoluciones_rpm_s2 = get_revoluciones_rpm(rb_encoder[2]);
    printf("Revoluciones: %f - %f rpm \n", revoluciones_rpm_s1, revoluciones_rpm_s2 );
    distance_cm_s1 = get_distance_cm(rb_encoder[1]);
    distance_cm_s2 = get_distance_cm(rb_encoder[3]);
    printf("Distancia recorrida Total: %f - %f cms \n", distance_cm_s1, distance_cm_s2);




    //////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////  State.txt update  /////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////    
    
    /* Busco a lo largo del archivo "Sensores, rightSensor =" */
    lineNro = 0;
    loffset = 0;
    while ( (lread=getline(&line, &len, fdd_State )) != -1)
    {
        lineNro++;
        loffset = loffset + lread;
        //printf("lineNro: %d \n", lineNro);
        //printf("line %s \n", line); 
        switch (sscanf(line, "Sensores, rightSensor = %s\n", aux ))
        {
            case EOF:       // Error
                perror("sscanf");
                exit(1);
                break;
            case 0:         // No encontro
                //printf("No se encontro la linea: Sensores, rightSensor \n");
                break;
            default:        // Encontro
                //printf("La linea es la nro: %d, offset: %d \n", lineNro, loffset); 
                //printf("line %s \n", line); 
                sprintf(line, "Sensores, rightSensor = %lld cms", rightSensor);
                fseek(fdd_State, (loffset-lread), SEEK_SET);
                if ( ( fwrite(line, sizeof(char), strlen(line), fdd_State)) != strlen(line))
                {
                    printf("Error escribiendo\n");
                    return -1;
                }
                fseek(fdd_State, (loffset), SEEK_SET);
                break;
        }

        switch (sscanf(line, "Sensores, centerSensor = %s\n", aux ))
        {
            case EOF:       // Error
                perror("sscanf");
                exit(1);
                break;
            case 0:         // No encontro
                break;
            default:        // Encontro
                sprintf(line, "Sensores, centerSensor = %lld cms", centerSensor); 
                fseek(fdd_State, (loffset-lread), SEEK_SET);
                if ( ( fwrite(line, sizeof(char), strlen(line), fdd_State)) != strlen(line))
                {
                    printf("Error escribiendo\n");
                    return -1;
                }
                fseek(fdd_State, (loffset), SEEK_SET);
                break;
        }

        switch (sscanf(line, "Sensores, leftSensor = %s\n", aux ))
        {
            case EOF:       // Error
                perror("sscanf");
                exit(1);
                break;
            case 0:         // No encontro
                break;
            default:        // Encontro
                sprintf(line, "Sensores, leftSensor = %lld cms", leftSensor); 
                fseek(fdd_State, (loffset-lread), SEEK_SET);
                if ( ( fwrite(line, sizeof(char), strlen(line), fdd_State)) != strlen(line))
                {
                    printf("Error escribiendo\n");
                    return -1;
                }
                fseek(fdd_State, (loffset), SEEK_SET);
                break;
        }

        switch (sscanf(line, "Brujula, Angulo = %s grados", aux ))
        {
            case EOF:       // Error
                perror("sscanf");
                exit(1);
                break;
            case 0:         // No encontro
                //printf("No se encontro: Brujula, Angulo\n");
                break;
            default:        // Encontro
                sprintf(line, "Brujula, Angulo = %f grados", heading); 
                fseek(fdd_State, (loffset-lread), SEEK_SET);
                if ( ( fwrite(line, sizeof(char), strlen(line), fdd_State)) != strlen(line))
                {
                    printf("Error escribiendo\n");
                    return -1;
                }
                fseek(fdd_State, (loffset), SEEK_SET);
                break;
        }

        switch (sscanf(line, "Encoder, Rev = %s - %s rpm", aux, aux ))
        {
            case EOF:       // Error
                perror("sscanf");
                exit(1);
                break;
            case 0:         // No encontro
                //printf("No se encontro: Brujula, Angulo\n");
                break;
            default:        // Encontro
                sprintf(line, "Encoder, Rev = %f - %f rpm", revoluciones_rpm_s1, revoluciones_rpm_s2); 
                fseek(fdd_State, (loffset-lread), SEEK_SET);
                if ( ( fwrite(line, sizeof(char), strlen(line), fdd_State)) != strlen(line))
                {
                    printf("Error escribiendo\n");
                    return -1;
                }
                fseek(fdd_State, (loffset), SEEK_SET);
                break;
        }

        switch (sscanf(line, "Encoder, Distancia = %s - %s cms", aux, aux ))
        {
            case EOF:       // Error
                perror("sscanf");
                exit(1);
                break;
            case 0:         // No encontro
                //printf("No se encontro: Brujula, Angulo\n");
                break;
            default:        // Encontro
                sprintf(line, "Encoder, Distancia = %f - %f cms", distance_cm_s1, distance_cm_s2); 
                fseek(fdd_State, (loffset-lread), SEEK_SET);
                if ( ( fwrite(line, sizeof(char), strlen(line), fdd_State)) != strlen(line))
                {
                    printf("Error escribiendo\n");
                    return -1;
                }
                fseek(fdd_State, (loffset), SEEK_SET);
                break;
        }

    }  







    close(fd_State);
    return 0;
}
