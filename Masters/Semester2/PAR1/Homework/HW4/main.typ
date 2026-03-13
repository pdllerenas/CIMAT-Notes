#import "@preview/grape-suite:3.1.0": exercise
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 2",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
    Implementamos de manera secuencial y paralela usando OpenMPI una función de promedio en una placa 2D. Comparamos los tiempos de ejecución para distintos tamaños de malla. Encontramos que la implementación paralela supera a la secuencial para $N$ grande.
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Introducción
Imaginemos un dispositivo similar a las pistolas infrarrojas utilizadas para
medir la temperatura de una superficie. Este tipo de instrumento no mide un solo
punto, sino que puede capturar información térmica de toda una región.
Supongamos que dicha región se discretiza como una malla bidimensional de tamaño
$N times N$, donde cada celda contiene un valor de temperatura. El objetivo del
dispositivo es calcular rápidamente la temperatura promedio de toda la región a
partir de estos $N^2$ valores.

Para tamaños pequeños de $N$, este cálculo puede
realizarse fácilmente en una computadora utilizando un programa secuencial. Sin
embargo, cuando la resolución del sensor aumenta, por ejemplo con valores de $N =
1000$ o mayores, el número total de datos crece considerablemente, alcanzando
millones de mediciones. En este contexto, resulta natural recurrir a la
programación paralela para acelerar el cálculo. Una estrategia eficiente
consiste en distribuir el trabajo entre múltiples procesos que operan
simultáneamente. Para ello utilizaremos MPI (Message Passing Interface).

Supondremos que el calor de la superficie leído por la pistola se puede representar mediante la función $T : RR^2 -> RR$ dada por
$
  T(x,y) = 20(sin(4x) sin(3y)+0.3cos(6x) sin(5y) + 1).
$

= Implementaciones
Presentamos las partes cruciales de cada implementación. Suponemos que la región a analizar es $[x_min, x_max] times [y_min, y_max]$.
== Secuencial
En la implementación secuencial, debemos recorrer una malla bidimensional en la
manera usual, con un doble bucle.
=== Generación de malla bidimensional
Para recorrer los puntos de la malla, parametrizamos la discretización de la
placa. Esto se logra al calcular un tamaño de paso:
$
  "step"_x & = (x_max - x_min) / N, \
  "step"_y & = (y_max - y_min) / N.
$
Dados estos valores, recorremos la malla mediante el siguiente bucle:

#figure(
  ```cpp
  for (int i = 0; i < N; ++i)
  {
  	double curr_y = ymin + i * step_y;
  	for (int j = 0; j < N; ++j)
  	{
  		double curr_x = xmin + j * step_x;
  		sum += T(curr_x, curr_y);
  	}
  }
  ```,
  caption: [Recorrido de la malla bidimensional de manera secuencial.],
)

=== Promedio
Con la variable `sum` dentro del bucle anterior, podemos calcular el promedio mediante:
$
  "mean" = "sum"/(N times N)
$

=== Compilación del programa
Para compilar el programa, usamos

#figure(
  ```sh
  g++ 2d_mean_sequential.cpp -o mseq
  ```,
  caption: [Compilación de la implementación secuencial.],
)
=== Ejecución del programa
Para ejecutar el programa, usamos

#figure(
  ```sh
  ./mseq <N> <xmin> <xmax> <ymin> <ymax>
  ```,
  caption: [Ejecución del programa. Los argumentos de línea corresponden a $N$: tamaño de malla ($N times N$), y los demás corresponden a los límites de la placa bidimensional.],
)

=== Ejemplos

#figure(
  ```sh
  ./mseq 100 0.0 2.0 0.0 2.0
  ```,
  caption: [Ejemplo con $N = 100$ en la placa $[0,2] times [0,2]$.],
)
El output de esta ejecución es
#figure(
  ```
  Execution time: 0.653947 ms
  Average: 19.9731
  ```,
  caption: [Ejemplo con $N = 100$ en la placa $[0,2] times [0,2]$.],
)

#let data_seq = csv("seq.txt").slice(1)
#figure(
  table(
    stroke: none,
    columns: 3,
    table.hline(),
    table.header([*N*], [*time (ms)*], [*Average*]),
    table.hline(),
    ..data_seq.flatten(),
    table.hline(),
  ),
  caption: [Tiempos de ejecución y resultados para la implementación secuencial.],
)

== Paralela (OpenMPI)
=== Generación de malla bidimensional
Al igual que en el caso secuencial, definimos el tamaño de paso:
$
  "step"_x & = (x_max - x_min) / N, \
  "step"_y & = (y_max - y_min) / N.
$
Luego, debemos dividir nuestra malla en $P$ procesos. Para hacer esto, le asignaremos a cada proceso una cantidad de filas de la matriz. Es decir:
$
  "slabs" & = N / P.
$
Si la division no es entera, le asignamos las filas faltantes al último proceso. Con esta cantidad de slabs. Cada proceso comienza y termina en una fila distinta. Entonces, definimos las siguientes variables locales que dependen del número de proceso:
#figure(
  ```
  	start = taskid * slabs;
  	end = start + slabs;
  ```,
  caption: [Límites de cada proceso.],
)
Dados estos límites, definimos el bucle que calcula la suma local de estas filas:

#figure(
  ```cpp
  for (int j = start; j < end; ++j)
  {
  	double curr_y = ymin + j * step_y;

  	for (int i = 0; i < N; ++i)
  	{
  		double curr_x = xmin + i * step_x;
  		sum += T(curr_x, curr_y);
  	}
  }
  ```,
  caption: [Límites de cada proceso.],
)
Hasta este momento, cada proceso ha calculado `sum` de manera privada. Para
juntar todas las secciones de la malla calculada, debemos usar `MPI_Reduce`.
Esto nos permite sincronizar todos los procesos, de tal manera que la suma
global coincida con la suma de las sumas locales.

#figure(
  ```cpp
  MPI_Reduce(&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM,0, MPI_COMM_WORLD);
  ```,
  caption: [Sincronización de procesos. Se realiza la suma (`MPI_SUM`) de los
    elementos locales (1 elemento) `sum` hacia la variable `global_sum` en el proceso 0. El tipo
    es `double`, por lo que le indicamos a la función `MPI_DOUBLE`.
    `MPI_COMM_WORLD` indica donde buscar las variables. En este caso, en los
    procesos hijos creados.],
)

=== Compilación del programa
Para compilar el programa, usamos

#figure(
  ```sh
  mpicxx 2d_mean_mpi.cpp -o mmpi
  ```,
  caption: [Compilación mediante MPI.],
)
=== Ejecución del programa
Para ejecutar el programa, usamos

#figure(
  ```sh
  mpirun -n <P> ./mmpi <N> <xmin> <xmax> <ymin> <ymax>
  ```,
  caption: [Ejecución de la implementación en OpenMPI. `<P>` representa los procesos a usar dentro del programa. Los demás argumentos son iguales a la versión secuencial.],
)

=== Ejemplos

#figure(
  ```sh
  mpirun -n 4 ./mmpi 100 0.0 2.0 0.0 2.0
  ```,
  caption: [Ejemplo con 4 procesos, $N = 100$ y la placa $[0,2] times [0,2]$.],
)

#figure(
  ```
  Execution time: 1.96475 ms
  Average: 19.9731
  ```,
  caption: [Ejemplo con 4 procesos, $N = 100$ y la placa $[0,2] times [0,2]$.],
)

#figure(
  table(
    stroke: none,
    columns: 3,
    table.hline(),
    table.header([*N*], [*time (ms)*], [*Average*]),
    table.hline(),
    [1], [0.190244], [20],
    [10], [0.042209], [19.9983],
    [100], [0.420061], [19.9731],
    [1000], [25.4098], [19.97],
    [10000], [2233.42], [19.9697],
    table.hline(),
  ),
  caption: [Tiempos de ejecución con 4 procesos.],
)


#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*N*], [*Secuencial*], [*OpenMPI*], [*Speedup*]),
    table.hline(),
    [1],[0.00700],[0.190244], [#{calc.round(0.00700/0.190244, digits: 3)}],
    [10], [0.0119],[0.042209], [#{calc.round(0.0119/0.042209, digits: 3)}],
    [100],[0.648309], [0.420061], [#{calc.round(0.648309/0.420061, digits: 3)}],
    [1000],[62.2854], [25.4098], [#{calc.round(62.2854/25.4098, digits: 3)}],
    [10000], [6276.06], [2233.42], [#{calc.round(6276.06/2233.42, digits: 3)}],
    table.hline(),
  ),
  caption: [Speedup de OpenMPI],
)