<h1 align="center"> Modulo: Procesamiento de imagenes </h1> 


## Linux-VDMA-TPG-PNG.Server

# Introduccion

Con el objetivo de comenzar con este modulo lo primero que se propuso fue evaluar que modulo de camara utilizar para el robot. 

Camara de arduino          |  Camara USB
:-------------------------:|:-------------------------:
![](https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/arduinocam.png)  |  ![](https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/usbcamara.png)

Fueron algunas de las opciones. 

Camara de arduino          		|  Camara USB
:--------------------------------------:|:----------------------------------------:
Muy economica (~400$)		        |  Cara (~3000$)
Requiere algunos dias de desarrollo 	|  Se conecta y se usa,  no funciona si no se consigue su Driver
Al requerirse un wrapper sobre la FPGA y luego un Driver para el wrapper de la FPGA sobre Linux permite un pre-procesado al tener la imagen que pasar por dentro de la logica programable antes de llegar a la memoria RAM del procesador.   |  La imagen es obtenida por USB directamente sobre la RAM del procesador. 




<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/tpg.png" alt="Hardware" width="1000" height="300"/>

# Desarrollo 

# Resultados 

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/res1-1.png" alt="Hardware" width="700" height="400"/>

<img src="https://github.com/Fuschetto97/Tesis/blob/main/pImagen/Petalinux_Projects/imagenes/res1-2.png" alt="Hardware" />


# Conclusiones
    
# Referencias
