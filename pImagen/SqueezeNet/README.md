<h1 align="center"> Implementación y entrenamiento (y quiza compresión) de una Red Neuronal Convolucional SqueezeNet </h1> 

<img src="a.jpg" alt="Hardware" width="500"/>

## Introduccion 

A lo largo de los años se ha logrado ir aumentando la precisión de las redes neuronales convolucionales con distintos tipos de arquitecturas a tal punto en el que hoy en día se pueden encontrar distintas arquitecturas que cumplan un nivel de precisión que se necesite.

Dado cierto nivel de precisión es muy útil tener una arquitectura de CNN (convolutional neural network) con menos parámetros. 



* Los FPGA a menudo tienen menos de 10 MBytes de memoria en chip y no tienen memoria o almacenamiento fuera del chip. Un modelo suficientemente pequeño podría almacenarse directamente en la FPGA, mientras que los fotogramas de vídeo se transmiten a través de la FPGA en tiempo real.

* Cuando se habla de entrenamiento distribuido, la carga de trabajo para entrenar el modelo se divide y se comparte entre varios procesadores llamados nodos de trabajo para acelerar y/o paralelizar el entrenamiento. En estos casos tener redes mas pequeñas ayuda a que este tipo de entrenamiento sea aun mas eficiente con la ventaja de que la performance de la red no se ve afectada.

* Empresas como Tesla copian periódicamente nuevos modelos de sus servidores a los productos de sus clientes (autos), al trabajar con redes mas pequeñas el gasto de exportar estos modelos se vería relativamente disminuido.
