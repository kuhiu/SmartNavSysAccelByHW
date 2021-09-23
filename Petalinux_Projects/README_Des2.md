## Desarrollo "Linux-VDMA-ov7670(camara)-PNG.Server" [Tiempo = 5 dias]

### Introduccion 

<img src=./imagenes/arduinocam.png alt="Hardware"/>

El ov7670 es un sensor de imagen CMOS de bajo voltaje que proporciona la funcionalidad completa de una camara VGA fabricado por Omnivision, cuenta con la posibilidad de trabajar a distintas resoluciones (VGA, QVGA,CIF, QCIF) y tasa de imagen, hasta un máximo de 640 x 480 píxeles a 30 imágenes porsegundo, y diferentes espacios de color (RGB555, RGB565 y YUV). Es un SoC por lo que es capaz de realizar procesamiento de imagenes (ontrol de exposición, gamma, balance de blancos, saturación de color, control de tono) controlador por una interface SCCB (Bus de Control de Camara Serial). Ademas el sensor incluye filtros propios de eliminacion de ruido electrico.


<img src=./imagenes/ov7670_sch.png alt="Hardware" />

* Image Sensor Array: sensor de 656 x 488 píxeles para un total de 320.128 píxeles, de los cuales 640 x 480 son activos (307.200).
* Timming Generator: Control del sensor de imagen y de la generacion de frames, generacion de señales temporales internas, generacion de las señales de salida (VSYNC, HREF, PCLK).
* Analog Signal Processor: Se encarga de las funcionalidades analogicas de la imagen (AGC, AWB).
* A/D Converter: Despues del bloque de procesamiento analogico, la señal analogica alimenta al conversor analogico-digital el cual trabaja a una velocidad maxima de 12 Mhz. Ademas este bloque es capaz de lograr la calibracion digital del nivel de negro, controlar el retraso del canal UV y controles adicionales del rango del conversor A/D.
* Test Pattern Generator: Genera patrones de color y de escala de grises de 8 bits (barra de colores de 8 bits, shift "1" el pin de entrada o bien una barra de grises que se desvanece).
* DSP: Controla la interpolacion de la imagen original a RGB e incorpora un control de calidad de la imagen (realce de bordes, RGB2YUB, control de saturacion, correccion de pixeles y ruido, control automatico de gamma y un conversor de 10 a 8 bits.).
* Image Scaler: Este bloque controla todas las salidas y los datos con el formato requerido. 
* SCCB Interface: Bus de Control de Camara Serial, permite controlar toda la configuracion del sensor.

### Pinout  

<img src=./imagenes/ov7670_pin.png alt="Hardware" />
 
Señales de sincronizacion (VSYNC, HREF, PCLK) 

* HREF: Señal de sincronizacion horizontal. Indica cuando una comienza o termina una linea horizontal de pixeles del frame. 
* VREF: Señal de sincronizacion vertical. Indica cuando comienza o termina un frame.
* PCLK: Clk de salida del modulo, sincroniza el wrapper de obtencion de la imagen, indica cuando leer el dato de los pines de salida. 

### Formatos de color

El sensor dispone de distintos tipos de formatos de color, uno de ellos es el RGB444 el cual utiliza 12 bits para almacenar la informacion de cada pixel, a su vez la informacion es entregada en 2 bytes (deshechar los sobrantes) y en 2 ciclos de pclk.

<img src=./imagenes/ov7670_444.png alt="Hardware" />

De la misma forma podemos encontrar dos formatos mas RGB555 y RGB565 (formato completo, 2 bytes).

<img src=./imagenes/ov7670_555.png alt="Hardware" />
<img src=./imagenes/ov7670_565.png alt="Hardware" />

### Conexion del OV7670 a la Zybo

<img src=./imagenes/pmod.png alt="Hardware" />

Los puertos pmod son conectores hembra de 2x6, en angulo recto, espaciados cada 100 mil. El puerto pmod de 12 pines proporciona dos señales VCC de 3,3v, dos señales de tierra y ocho logicas. Los pines VCC y Ground pueden suministrar hasta 1 A de corriente, pero se debe tener cuidado no exceder ninguno de los presupuestos de energía de los reguladores a bordo o la fuente de alimentación externa.

<img src=./imagenes/pmod_tipos_1.png alt="Hardware" />

<img src=./imagenes/pmod_tipos_2.png alt="Hardware" />


#### PMOD Standard 

Conectados al Zynq PL a traves de resistencias en series de 200 ohm. Estas evitan cortocircuitos que pueden ocurrir si el usuario usa como salida accidentalmente una señal que debiera utilizarse como entrada. La desventaja de esta proteccion adicional es que estas resistencias limitan la velocidad maxima de conmutacion de datos de estos pines. En caso de no necesitar alta velocidad, entonces es recomendable utilizar estos puertos para prevenir daños en el dispositivo. 

#### MIO PMOD 

El puerto MIO Pmod está conectado al bus MIO en el Zynq PS a través de resistencias en serie de 200 Ohm, al igual que el standard. Dado que estas señales estan conectadas a la interfaz MIO solo se puede acceder a ellas a traves del controlador periferico PS. Los núcleos GPIO, UART, I2C y SPI pueden usarse para controlar dispositivos conectados a este Pmod.

#### Dual Analog/Digital Pmod (XADC Pmod) 

El puerto pmod etiquetado como "XADC" esta conectado a los pines de entrada analogica auxiliar del PL (Este puerto puede utilizarse tambien para meter entradas analogicas diferenciales al conversor ADC). En el modo de entrada analogica debe limitarse a 1v la entrada. 

Los dispositivos Zynq-7000 AP SoC combinan un convertidor de analógico a digital flexible con lógica programable para abordar una amplia gama de requisitos de monitoreo y adquisición de datos analógicos. El XADC es parte de un tema más amplio de señales mixtas analógicas (AMS) que es una combinación de circuitos analógicos y digitales.

#### High Speed PMOD

Los puertos Pmod de alta velocidad utilizan el conector Pmod estándar, pero sus señales de datos se enrutan como impedancia pares diferenciales emparejados para velocidades máximas de conmutación. Los Pmods no ofrecen protección contra cortocircuitos, pero permiten velocidades de conmutación mucho más rápidas. Las señales se emparejan con las señales adyacentes en la misma fila: pines 1 y 2, pines 3 y 4, pines 7 y 8 y pines 9 y 10.


### Módulo de adquisición de imagen

https://github.com/laurivosandi/hdl/tree/master/zynq

### Resultados

<img src=./imagenes/ov7670_res.png alt="Hardware" />

<img src=./imagenes/report2.png alt="Hardware" />

### Referencias

* [Zybo Z7 Board Reference Manual](https://reference.digilentinc.com/_media/reference/programmable-logic/zybo-z7/zybo-z7_rm.pdf)
* [Zybo Z7 Board Schematic](https://reference.digilentinc.com/_media/reference/programmable-logic/zybo-z7/zybo_z7_sch-public.pdf)
* [Zybo Z7 Packing and Pinout](https://www.xilinx.com/support/documentation/user_guides/ug865-Zynq-7000-Pkg-Pinout.pdf)
* [Zynq 7010 Package](https://www.xilinx.com/support/packagefiles/z7packages/xc7z010clg400pkg.txt)
* [Lauri github](https://github.com/laurivosandi/hdl/tree/master/zynq)
* [Lauri Blog](https://lauri.xn--vsandi-pxa.com/hdl/zynq/zybo-ov7670-to-vga.html)
* [Lauri Blog 2](https://lauri.xn--vsandi-pxa.com/hdl/zynq/xilinx-video-capture.html)


### Trabajo futuro

* Escribir mi propio capturador de la imagen.
* Quitar el controlador SCCB de la FPGA y utilizar el del procesador para ahorrar recursos.
* Utilizar el DSP del ov7670 para mejorar la calidad de la imagen.


















