## Introduccion

Con el objetivo de comenzar con este modulo lo primero que se propuso fue evaluar que modulo de camara utilizar para el robot. 

Camara de arduino          |  Camara USB
:-------------------------:|:-------------------------:
![](https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/arduinocam.png)  |  ![](https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/usbcamara.png)

Fueron algunas de las opciones. 

OV7670         				|  Camara USB
:--------------------------------------:|:----------------------------------------:
Muy economica (~400$)		        |  Cara (~3500$)
Requiere algunos dias de desarrollo 	|  Se conecta y se usa,  no funciona si no se consigue su Driver
Al requerirse un wrapper sobre la FPGA y luego un Driver para el wrapper de la FPGA sobre Linux permite un pre-procesado al tener la imagen que pasar por dentro de la logica programable antes de llegar a la memoria RAM del procesador.   |  La imagen es obtenida por USB directamente sobre la RAM del procesador. 

Por cuestiones economicas y de aprendizaje se decidio realizar una prueba con el modulo OV7670 para tomar la decision. 

## Desarrollo "Linux-VDMA-TPG-PNG.Server" [Tiempo = 2 semanas]

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/tpg.png" alt="Hardware" width="1000" height="300"/>

Como hacer que todo funcione bien en el primer intento es casi imposible, se propone sustituir la camara por un generador arbitrario de imagenes llamado "Test Pattern Generator". 

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/mapaDirecciones.png" alt="Hardware" width="1000" height="300"/>

En este caso el registro de control y estado del VDMA se encuentran mapeados en 0x4400_0000 usando AXI_LITE y esa direccion de memoria se puede escribir para iniciar una transferencia DMA. Por supuesto que se necesita escribir un Driver sobre Linux. 
Como mi objetivo es transferir una imagen desde el generador arbitrario a la memoria DDR del procesador la transferencia MMS2 queda deshabilitada y la S2MM del controlador VDMA tiene acceso a todo el rango de memoria fisica (512 mb de la Zybo). Para tranquilidad del lector, solo una pequena seccion de esta memoria sera utilizada para almacenar la imagen. 

En mi caso solo quiero guardar en la DDR una imagen de 640x480 RGB (8 bits por color) por lo tanto se necesitaran aproximadamente 7.5 mBytes de memoria RAM.

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/res1-2.png" alt="Hardware" />

### Captura de Frames sobre HTTP

Una vez que la transferencia VDMA corre, se puede levantar un servidor PNG con muy pocas lineas sobre python para servir esto sobre un navegador.  

## Resultados 

Aqui se puede ver la Zybo corriendo con Petalinux, el TPG (Test Patttern Generator), el VDMA y el servidor sobre la red LAN de mi casa. 

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/res1-1.png" alt="Hardware" width="700" height="400"/>
    
## Referencias

* [Zynq-7000 SoC Technical Reference Manual](https://www.xilinx.com/support/documentation/user_guides/ug585-Zynq-7000-TRM.pdf) 
* [AXI Video Direct Memory Access](https://www.xilinx.com/support/documentation/ip_documentation/axi_vdma/v6_2/pg020_axi_vdma.pdf) 
* [Video Test Pattern Generator v8.0](https://www.xilinx.com/support/documentation/ip_documentation/v_tpg/v8_0/pg103-v-tpg.pdf) 
* [Video capture with VDMA - LauriBlog](https://lauri.xn--vsandi-pxa.com/hdl/zynq/xilinx-video-capture.html) 
* [The png Module](https://pypng.readthedocs.io/en/latest/png.html) 
* [AXI GPIO](https://www.xilinx.com/support/documentation/ip_documentation/axi_gpio/v2_0/pg144-axi-gpio.pdf)

## Notas

* Navegador http://192.168.100.130/80 

## Trabajo futuro 

* Quitar el generador arbitrario y escribir el wrapper (verilog) de la OV7670, el Driver del wrapper para Linux.


