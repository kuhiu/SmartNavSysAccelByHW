<h1 align="center"> Modulo: Procesamiento de imagenes </h1> 

### Diseño del sistema 

<img src=./imagenes/vdmav1.png alt="Hardware" />

Un problema que se puede ver en este esquematico es que la camara sirve imagenes a una taza de 30fps (proveedor del sistema) y no existe ningun mecanismo para sincronizar al consumidor (codigo de la red CNN) por lo que es pausible que se procese una imagen incorrecta.

* [PC - Entrenamiento](https://github.com/Fuschetto97/Tesis/tree/main/pImagen/SqueezeDetTraining)
* [C y Python - Implementacion](https://github.com/Fuschetto97/Tesis/tree/main/pImagen/SqueezeDetFromScratch)
