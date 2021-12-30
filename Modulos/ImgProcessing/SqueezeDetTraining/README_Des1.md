## Entrenamiento de la red neuronal [Tiempo = 14 dias]

### Introduccion y objetivos 

El proposito de este desarrollo es el de entrenar la red neuronal propuesta sobre la PC y comprobar si es apta para su uso.

* Crear un pequeño dataset propio.
* Entrenar la red neuronal.
* Conseguir los parametros de la CNN entrenada.

### Creacion de un pequeno dataset propio 

A modo de realizar una primera prueba se llevo a cabo la construccion de un pequeño dataset de 225 imagenes, las cuales fueron etiquetadas con el programa "Alp’s Labeling Tool (ALT)". 

Esta aplicacion te permite dibujar rectángulos alrededor de los objetos, nombrar estos rectángulos, rotar la imagen con los rectángulos por completo, guardar toda esta información y volver a cargar más tarde junto con la imagen seleccionada para continuar etiquetando desde donde lo dejaste. Los archivos de etiquetas ".txt" se crean en la misma carpeta que la imagen y contienen etiquetas y sus coordenadas de cuadro delimitador, por lo que una vez completado el trabajo de etiquetado, puede mover los archivos de etiquetas ".txt" relevantes al conjunto de datos real, a las carpetas de "etiquetas" en "tren" y "val".

<img src=./imagenes/etiquetas.png alt="cnn" /> 

### Entrenamiento de una red neuronal SqueezeDet

Para el entrenamiento de esta red se utilizo un repositorio con la implementacion de la [SqueezeDet](https://arxiv.org/pdf/1612.01051.pdf) en Keras el cual se puede encontrar [aqui](https://github.com/omni-us/squeezedet-keras).

### Resultados 

Primeros resultados con un dataset proprio de 225 imagenes (entrenamiento y validacion).

<img src=./imagenes/tomate_250loss.png alt="cnn" /> <img src=./imagenes/tomate_250p.png alt="cnn" /> 

Aqui se pueden ver las imagenes utilizadas como validacion. El cuadro verde es la etiqueta hecha con Alp’s Labeling Tool y las rojas las predicciones hechas por la red con un treshold en la probabilidad superior al 30%.  

<img src=./imagenes/tomate_250_im.png alt="cnn" /> 

### Obtencion de los parametros

### Conclusion

Con este pequeño desarrollo se pudo comprobar que la red es apta para el uso que se le quiere dar.


### Referencias

* [Omni-us / squeezedet-keras](https://github.com/omni-us/squeezedet-keras)
* [Alp’s Labeling Tool ALT](https://alpslabel.wordpress.com/2017/01/26/alt/)
* [SqueezeDet paper](https://arxiv.org/pdf/1612.01051.pdf)
* [Squeezedet-keras github implementacion](https://github.com/omni-us/squeezedet-keras)