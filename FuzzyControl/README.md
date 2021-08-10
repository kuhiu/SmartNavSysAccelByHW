## Resolucion de problemas con logica difusa

En este trabajo se resuelve un problema de planificación de movimiento utilizando la técnica de control difuso para un móvil con tres sensores de distancia y libertad de movimiento. Tanto la simulación del problema como la resolución de la lógica difusa se encuentran implementados en matlab.

### Introduccion 

Los sistemas de control difuso se utilizan en una amplia variedad de aplicaciones, que van desde el control de electrodomésticos, por ejemplo los lavarropas, hasta aplicaciones críticas como es el control de trenes y subterráneos. A diferencia de los sistemas de control clásico, en los sistemas de control difuso el comportamiento se basa en reglas definidas en lenguaje natural. Además permite agregar múltiples variables de entrada y salida sin complejizar demasiado su resolución. Otra ventaja del control difuso es la suavidad de las variaciones en las salidas frente a cambios bruscos en las entradas. El problema resuelto mediante esta técnica es la planificación de movimiento de un móvil que tiene como entradas las distancias medidas por tres sensores y el sistema debe resolver los movimientos a realizar para llegar a su destino evadiendo los obstáculos. El control difuso nos permite trabajar de manera sencilla con la combinación de las entradas para definir las decisiones del móvil.

### Antecedentes

Tradicionalmente la robótica estuvo centrada en sectores industriales manufactureros orientados a la producción masiva. A mediados de la década del 60 se introducen robots manipuladores en la industria debido a las posibilidades que estos ofrecían [1]. 
Normalmente los robots desarrollaban tareas repetitivas. Estas tareas exigían tomar algunas piezas y reubicarlas en otra área a la cual el robot manipulador sea capaz de llegar con la máxima extensión de su articulación, es decir su área de acción estaba limitado al largo de su articulación.
Una mejora frente a esta limitación se dió al desarrollar un vehículo móvil sobre rieles para que el robot tenga mayor alcance. Así es como a mediados de la década del 80 aparecieron los primeros vehículos guiados automáticamente (AGV's) en la industria.
Fuera del entorno industrial, en donde se imposibilita estructurar el entorno, se les dotó a los robots un mayor grado de inteligencia y capacidad para poder
desenvolverse.
Uno de los desafíos más grandes en la aplicación de robots es la navegación en entornos desconocidos abarrotados de obstáculos. La navegación se vuelve aún más compleja cuando no se conoce la ubicación de estos a priori.
Para mover un robot en un entorno definido con cierto grado de autonomía es necesario que este sea capaz de la detección y evitación autónoma de obstáculos y/o del reconocimiento de estos para poder actuar.
La lógica difusa fue investigada a mediados de los años sesenta en la Universidad de Berkeley (California) y trae una solución a este problema.

### Bases Teoricas

Los sistemas de control en general se encargan de evaluar las variables de entrada, realizar un proceso en base a un comportamiento deseado para poder modificar las salidas y en definitiva actuar sobre la planta. En el caso del control difuso las variables se evalúan utilizando lógica difusa y el proceso se realiza en base a un conjunto de reglas.


#### Logica difusa

Las variables que intervienen en las reglas no son verdaderas o falsas, sino que el grado de pertenencia a un conjunto está dado por una función que varía entre 0 y 1 en todo el rango de la variable también llamado universo de discurso [2].

#### Sistemas Basados en Reglas

El conjunto de reglas se definen en lenguaje natural, según el grado de pertenencia de sus variables será el grado de verdad de la regla. El comportamiento de la salida dependerá de las reglas con mayor grado de verdad.

SI antecedente ENTONCES consecuente
SI condición ENTONCES acción

#### Control Difuso
Se puede descomponer el proceso de control difuso en tres etapas principales.

##### <div style="text-align:center"> 1) Fuzzificación 

Dados los valores de entrada se calcula el grado de pertenencia a cada uno de los conjuntos difusos considerados, mediante las funciones características asociadas a estos conjuntos difusos.

<p align="center">
  <img src=./imagenes/fpertenencia.png alt="Hardware" />
</p>

##### <div style="text-align:center"> 2) Evaluación de reglas

Relaciona los conjuntos de entrada (las cuales son conjuntos difusos, grados de pertenencia) mediante mecanismos de inferencia (reglas) con las salidas (las cuales también son conjuntos difusos, asociados a una o varias variables de salida).

##### <div style="text-align:center"> 3) Defuzzificación

A partir del conjunto difuso obtenido en el mecanismo de inferencia y mediante métodos matemáticos de desdifusión, se obtiene un valor concreto de la/s variable/s de salida.

<p align="center">
<img src=./imagenes/etapas.png alt="Hardware" />
</p>

#### Estado del arte

En 2006 se desarrolló un sistema difuso para la navegación en entornos densamente abarrotados de obstáculos. En donde utilizaron lógica de preferencias para poder arbitrar o fusionar las reacción de los comportamientos individuales difusos.
“El problema más difícil al aplicar sistemas de control de navegación basados en el comportamiento reactivo difuso es el de arbitrar o fusionar las reacciones de los comportamientos individuales, que se aborda aquí mediante el uso de la lógica de preferencias.” [3]

<p align="center">
<img src=./imagenes/state1.png alt="Hardware" />
</p>

Danial y Babak [4] dividieron la navegación en dos partes. La primera compuesta en comportamientos básicos: lograr metas, evitación de obstáculos y seguimiento de muros. La segunda, una capa de supervisión responsable de la selección de las acciones (elección de comportamientos según el contexto) en donde seleccionan un comportamiento de la lista de estos en función de su prioridad.

<p align="center">
<img src=./imagenes/state2.png alt="Hardware" />
</p>

En 2016 se desarrolló un sistema de control difuso con la ventaja de que solo necesitaba un único controlador difuso para la navegación y la evasión de obstáculos para un robot [5].

<p align="center">
<img src=./imagenes/state3.png />
</p>

<p align="center">
<img src=./imagenes/state4.png  />
</p>