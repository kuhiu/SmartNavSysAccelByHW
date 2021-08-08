## Desarrollo "Linux-VDMA-ov7670(camara)-PNG.Server" [Tiempo = 5 dias]

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/arduinocam.png" alt="Hardware"/>

El ov7670 es un sensor de imagen CMOS de bajo voltaje que proporciona la funcionalidad completa de una camara VGA fabricado por Omnivision, cuenta con la posibilidad de trabajar a distintas resoluciones (VGA, QVGA,CIF, QCIF) y tasa de imagen, hasta un máximo de 640 x 480 píxeles a 30 imágenes porsegundo, y diferentes espacios de color (RGB555, RGB565 y YUV). Es un SoC por lo que es capaz de realizar procesamiento de imagenes (ontrol de exposición, gamma, balance de blancos, saturación de color, control de tono) controlador por una interface SCCB (Bus de Control de Camara Serial). Ademas el sensor incluye filtros propios de eliminacion de ruido electrico.


<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/ov7670_sch.png" alt="Hardware" />

* Image Sensor Array: sensor de 656 x 488 píxeles para un total de 320.128 píxeles, de los cuales 640 x 480 son activos (307.200).
* Timming Generator: Control del sensor de imagen y de la generacion de frames, generacion de señales temporales internas, generacion de las señales de salida (VSYNC, HREF, PCLK).
* Analog Signal Processor: Se encarga de las funcionalidades analogicas de la imagen (AGC, AWB).
* A/D Converter: Despues del bloque de procesamiento analogico, la señal analogica alimenta al conversor analogico-digital el cual trabaja a una velocidad maxima de 12 Mhz. Ademas este bloque es capaz de lograr la calibracion digital del nivel de negro, controlar el retraso del canal UV y controles adicionales del rango del conversor A/D.
* Test Pattern Generator: Genera patrones de color y de escala de grises de 8 bits (barra de colores de 8 bits, shift "1" el pin de entrada o bien una barra de grises que se desvanece).
* DSP: Controla la interpolacion de la imagen original a RGB e incorpora un control de calidad de la imagen (realce de bordes, RGB2YUB, control de saturacion, correccion de pixeles y ruido, control automatico de gamma y un conversor de 10 a 8 bits.).




<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/ov7670.png" alt="Hardware" width="1000" height="300"/>
