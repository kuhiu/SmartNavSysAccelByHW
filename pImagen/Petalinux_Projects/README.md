<h1 align="center"> Modulo: Procesamiento de imagenes </h1> 

## Introduccion

Con el objetivo de comenzar con este modulo lo primero que se propuso fue evaluar que modulo de camara utilizar para el robot. 

Camara de arduino          |  Camara USB
:-------------------------:|:-------------------------:
![](https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/arduinocam.png)  |  ![](https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/usbcamara.png)

Fueron algunas de las opciones. 

Camara de arduino          		|  Camara USB
:--------------------------------------:|:----------------------------------------:
Muy economica (~400$)		        |  Cara (~3500$)
Requiere algunos dias de desarrollo 	|  Se conecta y se usa,  no funciona si no se consigue su Driver
Al requerirse un wrapper sobre la FPGA y luego un Driver para el wrapper de la FPGA sobre Linux permite un pre-procesado al tener la imagen que pasar por dentro de la logica programable antes de llegar a la memoria RAM del procesador.   |  La imagen es obtenida por USB directamente sobre la RAM del procesador. 

Por cuestiones economicas y de aprendizaje se decidio realizar una prueba con el modulo OV7670 para tomar la decision. 

## Desarrollo "Linux-VDMA-TPG-PNG.Server" [Tiempo = 2 semanas]

Como hacer que todo funcione bien en el primer intento es casi imposible, se propone sustituir la camara por un generador arbitrario de imagenes llamado "Test Pattern Generator". 

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/tpg.png" alt="Hardware" width="1000" height="300"/>

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/mapaDirecciones.png" alt="Hardware" width="1000" height="300"/>

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/res1-2.png" alt="Hardware" />

## Resultados 

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/res1-1.png" alt="Hardware" width="700" height="400"/>



## Conclusiones
    
## Referencias
