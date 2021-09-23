## Desarrollo "i2c controller handling SCCB" [Tiempo = 4 dias]

### Introduccion 

El proposito de este desarrollo es el de quitar el controlador SCCB (i2c compatible) de la logica programable y utilizar el controlador i2c del procesador.

Las ventajas de llevar a cabo esto seran: 

* Mas recursos disponibles en la FPGA.
* La posibilidad de configurar la camara desde el user space de Linux, y poder modificar dinamicamente el DSP integrado en la ov7670.

### SCCB vs I2C

I2C es un puerto y protocolo de comunicación serial, define la trama de datos y las conexiones físicas para transferir bits entre 2 dispositivos digitales. El puerto incluye dos cables de comunicación, SDA y SCL. Además el protocolo permite conectar hasta 127 dispositivos esclavos con esas dos líneas, con hasta velocidades de 100, 400 y 1000 kbits/s.

#### I2C - Esquema de comunicacion 

Existen dos elementos basicos, un MAESTRO y un ESCLAVO. El MAESTRO I2C se encarga de controlar al cable de reloj, y de iniciar y parar la comunicacion. Cabe aclarar que dos maestros no pueden hacer uso del bus.

<img src=./imagenes/i2c.png alt="Hardware" />

* Start – S
* Parada – P
* Confirmación – ACK
* NoConfirmación – NACK
* Lectura-/Escritura – L/W
* 7 bits para la dirección del dispositivo esclavo/maestro
* 8 bits de dirección ( para algunos sensores pueden ser 16 bits)
* 8 bits de datos

#### SCCB 

SCCB es un protocolo I2C simplificado, SIO-l es una línea de entrada de reloj y SIO-O es una línea de datos bidireccional en serie, que es equivalente a SCL y SDA del protocolo I2C, respectivamente. La sincronización del bus de SCCB es básicamente la misma que la de I2C.

En la actualidad, el protocolo de comunicación del bus SCCB solo admite velocidades de transmisión de 100 Kb / so 400 Kb / s, forma de dirección:

* Direccion del esclavo de 8 bits, los 7 superiores son utilizados para el chip select y el bit inferior es el bit de control lectura/escritura.


Su señal de respuesta ACK se denomina noveno bit de una unidad de transmisión, que se divide en 'don't care' o 'NA' bit dependiendo de si es escritura o lectura.

El bit 'don't care' es el noveno bit de transmision emitido por el maestro. El maestro asume que no hay ningún error de transmisión durante la transmisión de datos y el proposito de este bit es el de indicar la finalizacion de la transmision. 

#### Conclusion

SCCB es un protocolo compatible con I2C. Algo muy importante a tener en cuenta es que en SCCB se llama direccion o address a {slave addr, r/w bit} mientras que en I2C la direccion es simplemente la direccion del esclavo, entonces cuando en el datasheet del ov7670 dice que su direccion es 0x42 para la escritura lo que esta diciendo es que la direccion del esclavo es 0x21 y el bit de control 0.

### Resultados 

<img src=./imagenes/test1.png alt="Hardware" />

Primera imagen tomada con el controlador i2c del procesador (+linux driver). Resolucion VGA (640x480).

<img src=./imagenes/test2.png alt="Hardware" />

Resolucion qvga (320x240).

#### Resultados - DSP ov7670 

<img src=./imagenes/sinfiltro.png alt="Hardware" /> <img src=./imagenes/test_denoise.png alt="Hardware" />

Normal / De-noise 

<img src=./imagenes/test_edgeengan.png alt="Hardware" /> <img src=./imagenes/test_edgeengan2.png alt="Hardware" />

Edge Enhanced 

<img src=./imagenes/test_normal.png alt="Hardware" /> <img src=./imagenes/test_denoiseplusedgeenhan.png alt="Hardware"/>

Normal / Edge Enhanced + de-noise

#### Otros

<img src=./imagenes/final.png alt="Hardware" /> <img src=./imagenes/final2.png alt="Hardware" />

<img src=./imagenes/final3.png alt="Hardware" /> <img src=./imagenes/final4.png alt="Hardware" />

<img src=./imagenes/final5.png alt="Hardware" />


<img src=./imagenes/night.png alt="Hardware" /> <img src=./imagenes/night_withmode.png alt="Hardware" />

<img src=./imagenes/finall.png alt="Hardware" />

### Referencias

* https://hetpro-store.com/TUTORIALES/i2c/
* https://www.waveshare.com/w/upload/1/14OmniVision_Technologies_Seril_Camera_Control_Bus%28SCCB%29_Specificatio.pdf
* http://web.mit.edu/6.111/www/f2016/tools/OV7670_2006.pdf
* http://www.diegm.uniud.it/loghi/CE/slides/usense.pdf








