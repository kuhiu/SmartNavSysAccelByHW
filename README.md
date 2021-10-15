<h1 align="center"> Tesis </h1> 

## Links

* [Modulo - Procesamiento de imagenes](https://github.com/Fuschetto97/Tesis/tree/main/pImagen)
* [Modulo - Sistema de control difuso](https://github.com/Fuschetto97/Tesis/tree/main/FuzzyControl)
* [Zybo - Implementacion](https://github.com/Fuschetto97/Tesis/tree/main/Petalinux_Projects)

## Resuelto

0) ~~Comprar sensores de distancia~~ y escribir drivers.
1) ~~Quitar la red del framework y sus pesos, escribirla en python~~
2) ~~Escribir en C la red~~
3) ~~Movimiento del robot~~ (Tiempo=1 mes)
    * ~~motores y sus drivers)~~ 
    * ~~Comprar sensor para medir distancia recorrida y escribir driver.~~
    * ~~Para poder modificar su velocidad pwm en FPGA y su driver. (probar ILA si es necesario)~~
    * ~~Comprar brujula y escribir su driver. (para conocer la direccion a la que se desplaza)~~
4) ~~Comprar fuente dc-dc 5v al menos 1.5A para alimentar externamente la Zybo.~~
5) ~~Crear un archivo en donde se mantenga actualizada toda la informacion de los drivers, direccion del robot, velocidad del robot, distancia por recorrrer para llegar al target, sensores de distancia, etc. A PARTIR DE AQUI LLAMADAS VARIABLES DE ESTADO DEL ROBOT~~

## Pendientes v1 del robot

* El sistema de procesamiento de imagenes unicamente reconoce un tomate.
* El sistema de control difuso solamente controla la direccion a la cual se dirige el robot para evitar colisionar con el obstaculo.

5) A partir de la red neuronal escribir las variables de estado.
6) A partir de las variables de estado controlar los motores del robot.
7) Escribir el sistema de control difuso en python
8) Aplicaciones (sistema difuso + procesamiento de imagenes).

### Pendientes, proximas versiones del robot

1) Escribir el sistema de control difuso en C 
2) Desarrollo en la FPGA y su driver.
3) A partir de las variables de estado controlar la velocidad del robot.
4) El sistema de control difuso es capaz de dirigirse a un punto especifico evitando colisionar.
5) El robot es capaz de modificar su comportamiento al reconocer distintos obstaculos.

## Estado actual de la implementacion

<img src=./imagenes/estado2.jpeg height=500 width=350 alt="Hardware" /> <img src=./imagenes/estado3.jpeg alt="Hardware" />
<img src=./imagenes/estado4.jpeg height=500 width=500 alt="Hardware" />
<img src=./imagenes/estado5.jpeg scale=0.5 alt="Hardware" />

