#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lilaq:0.6.0" as lq
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 3",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
    Implementamos un algoritmo mediante la integración de $4/(x^2+1)$ de 0 a 1 para encontrar el valor $pi$. Comparamos los tiempos de ejecución de la implementación serial y en OpenMPI.
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Introducción
La constante $pi$ tiene un valor aproximado de $3.14159...$. Es un número trascendental. Esto quiere decir que no existe una ecuación polinomial para la cual $P(pi) = 0$. Entonces, debemos encontrar maneras creativas de aproximar el valor de esta constante.

Recordemos que
$
  d/(d t) arctan (t) = 1/(1+t^2),
$
por lo que
$
  integral_0^1 1/(1+t^2) d t = arctan(1) - arctan(0) = pi/4.
$
Entonces, una manera de aproximar el valor de $pi$ es calculando una aproximación a la integral
$
  integral_0^1 1/(1+t^2) d t.
$
Para aproximar esta integral, podemos utilizar diversos métodos numéricos. Por
simplicidad, usaremos el método del rectángulo. Es decir, dada la curva $y(t) =
1/(1+t^2)$, dividimos el intervalo $[0,1]$ en $N$ trozos de la misma medida. Suponemos que los límites de cada trozo es $x_i$ para $i = 0, ..., N$. Luego, formamos el rectángulo
$
  [x_i, x_(i+1)] times [min(f((x_i + x_(i+1))/2), 0),max(0, f((x_i + x_(i+1))/2))]
$
Es decir, un rectángulo con una altura del punto medio de los límites de la partición. Notemos que estos rectángulos también pueden ir hacia abajo del eje $x$.

#let x = lq.linspace(-2, 2)
#let y = x.map(x => 4 / (1 + x * x))

#figure(
  lq.diagram(
    grid: none,
    lq.plot(mark: none, x, y),
  ),
  caption: [Función $4/(1+x^2)$ en $[-2,2]$.],
)

= Implementaciones
Presentamos partes cruciales del código de ambas implementaciones.
== Secuencial
Para la versión secuencial, debemos recorrer cada uno de los rectángulos y
realizar la suma del área de cada uno.
#figure(
  ```cpp
  double step = (b - a) / static_cast<double>(N);
  double area = 0.0;
  double curr_mark = a;
  for (long i = 0; i < N; i++)
  {
    double mid = curr_mark + step / 2;
    area += step * f(mid);
    curr_mark += step;
  }
  return area;
  ```,
  caption: [Recorrido secuencial para la suma de áreas.],
)

=== Compilación del programa
Para compilar el programa, usamos

#figure(
  ```sh
  g++ integral_seq.cpp -o seq
  ```,
  caption: [Compilación de la implementación secuencial.],
)
=== Ejecución del programa
Para ejecutar el programa, usamos

#figure(
  ```sh
  ./seq <N>
  ```,
  caption: [Ejecución del programa. Los argumentos de línea corresponden a $N$:
    número de particiones del intervalo.],
)

=== Ejemplos

#figure(
  ```sh
  ./seq 1000
  ```,
  caption: [Ejemplo con $N = 1000$.],
)
El output de esta ejecución es
#figure(
  ```
  ===== SEQUENTIAL =====
  Execution time: 0.006615 ms
  Area: 3.14159273692313
  Error: 8.33333366756506e-08
  ```,
  caption: [Ejemplo con $N = 1000$.],
)

== OpenMPI
#figure(
  ```cpp
  double step = (b - a) / static_cast<double>(N);
  double global_area = 0.0;
  double area = 0.0;
  long start = taskid * N / numtasks;
  long end = (taskid + 1) * N / numtasks;
  double curr_mark = a + start * step;

  for (int i = start; i < end; i++)
  {
    double mid = curr_mark + step / 2;
    area += step * f(mid);
    curr_mark += step;
  }
  MPI_Reduce(&area, &global_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  return global_area;
  ```,
  caption: [Recorrido paralelo usando `Reduce` para la suma de áreas.],
)

=== Compilación del programa
Para compilar el programa, usamos

#figure(
  ```sh
  mpicxx integral_mpi.cpp -o mpi
  ```,
  caption: [Compilación de la implementación en OpenMPI.],
)
=== Ejecución del programa
Para ejecutar el programa, usamos

#figure(
  ```sh
  mpirun -n <processes> ./mpi <N>
  ```,
  caption: [Ejecución del programa. Los argumentos de línea corresponden a $N$:
    número de particiones del intervalo.],
)
=== Ejemplo

#figure(
  ```sh
  mpirun -n 4 ./mpi 1000
  ```,
  caption: [Ejemplo con $N = 1000$],
)

El output de esta ejecución es
#figure(
  ```
  ===== MPI_REDUCE =====
  Execution time: 0.143244 ms
  Area: 3.14159273692313
  Error: 8.33333331229369e-08
  ===== MPI_SEND =====
  Execution time: 0.005472 ms
  Area: 3.14159273692313
  Error: 8.33333335670261e-08
  ```,
  caption: [Ejemplo con $N = 1000$.],
)

= Análisis de resultados

#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header(
      [*$N$*], 
      [*Versión*], 
      [*Tiempo (ms)*], 
      [*Aproximación*], 
      [*Error*]
    ),
    table.hline(),
    
    // Group: N = 1,000
    [1,000], [Secuencial], [0.007800], [3.14159273692313], [8.33333366756506e-08],
    [], [Reduce], [0.013100], [3.14159273692313], [8.33333366756506e-08],
    [], [Send], [0.006300], [3.14159273692313], [8.33333366756506e-08],
    [], [AllGather], [0.008700], [3.14159273692313], [8.33333366756506e-08],
    
    table.hline(stroke: 0.5pt + gray),
    
    // Group: N = 10,000
    [10,000], [Secuencial], [0.065600], [3.14159265442323], [8.33440427783216e-10],
    [], [Reduce], [0.066100], [3.14159265442323], [8.33440427783216e-10],
    [], [Send], [0.094400], [3.14159265442323], [8.33440427783216e-10],
    [], [AllGather], [0.122000], [3.14159265442323], [8.33440427783216e-10],
    
    table.hline(stroke: 0.5pt + gray),
    
    // Group: N = 1,000,000
    [1,000,000], [Secuencial], [6.694400], [3.14159265359130], [1.50945922428036e-12],
    [], [Reduce], [5.887600], [3.14159265359130], [1.50945922428036e-12],
    [], [Send], [6.176702], [3.14159265359130], [1.50945922428036e-12],
    [], [AllGather], [7.837703], [3.14159265359130], [1.50945922428036e-12],
    
    table.hline(),
  ),
  caption: [Rendimiento comparativo entre ejecución secuencial y distintas versiones de MPI para 1 proceso.],
)

#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header([*$N$*], [*Versión*], [*Tiempo (ms)*], [*Aproximación*], [*Error*]),
    table.hline(),

    // Group: N = 1,000
    [1,000], [Reduce], [0.612202], [3.14159273692313], [8.33333366756506e-08],
    [], [Send], [0.005400], [3.14159273692313], [8.33333366756506e-08],
    [], [AllGather], [0.023200], [3.14159273692313], [8.33333366756506e-08],

    table.hline(stroke: 0.5pt + gray),

    // Group: N = 10,000
    [10,000], [Reduce], [0.112198], [3.14159265442317], [8.33380919829096e-10],
    [], [Send], [0.034699], [3.14159265442317], [8.33380919829096e-10],
    [], [AllGather], [0.035599], [3.14159265442317], [8.33380919829096e-10],

    table.hline(stroke: 0.5pt + gray),

    // Group: N = 1,000,000
    [1,000,000], [Reduce], [3.151810], [3.14159265358348], [6.30961949354969e-12],
    [], [Send], [3.909611], [3.14159265358348], [6.30961949354969e-12],
    [], [AllGather], [6.692720], [3.14159265358348], [6.30961949354969e-12],

    table.hline(),
  ),
  caption: [Rendimiento comparativo entre distintas versiones de MPI para 2 procesos.],
)

#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header([*$N$*], [*Versión*], [*Tiempo (ms)*], [*Aproximación*], [*Error*]),
    table.hline(),

    // Group: N = 1,000
    [1,000], [Reduce], [0.045927], [3.14159273692313], [8.33333331229369e-08],
    [], [Send], [0.005472], [3.14159273692313], [8.33333335670261e-08],
    [], [AllGather], [0.009533], [3.14159273692313], [8.33333335670261e-08],

    table.hline(stroke: 0.5pt + gray),

    // Group: N = 10,000
    [10,000], [Reduce], [0.100905], [3.14159265442316], [8.33363156260702e-10],
    [], [Send], [0.028338], [3.14159265442316], [8.33362712171493e-10],
    [], [AllGather], [0.028876], [3.14159265442316], [8.33362712171493e-10],

    table.hline(stroke: 0.5pt + gray),

    // Group: N = 1,000,000
    [1,000,000], [Reduce], [1.624320], [3.14159265358765], [2.14450679436595e-12],
    [], [Send], [1.734390], [3.14159265358765], [2.14450679436595e-12],
    [], [AllGather], [1.664277], [3.14159265358765], [2.14450679436595e-12],

    table.hline(),
  ),
  caption: [Rendimiento comparativo entre distintas versiones de MPI para 4 procesos.],
)

#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header([*$N$*], [*Versión*], [*Tiempo (ms)*], [*Aproximación*], [*Error*]),
    table.hline(),

    // Group: N = 1,000
    [1,000], [Reduce], [0.185], [3.14159273692313], [8.33333335670261e-08],
    [], [Send], [0.004900], [3.14159273692313], [8.33333331229369e-08],
    [], [AllGather], [0.008900], [3.14159273692313], [8.33333331229369e-08],

    table.hline(stroke: 0.5pt + gray),

    // Group: N = 10,000
    [10,000], [Reduce], [0.849599], [3.14159265442314], [8.33347613138358e-10],
    [], [Send], [0.060000], [3.14159265442314], [8.33348057227568e-10],
    [], [AllGather], [0.021400], [3.14159265442314], [8.33348057227568e-10],

    table.hline(stroke: 0.5pt + gray),

    // Group: N = 1,000,000
    [1,000,000], [Reduce], [1.998700], [3.14159265358870], [1.09512399149025e-12],
    [], [Send], [2.209100], [3.14159265358870], [1.09467990228040e-12],
    [], [AllGather], [1.258800], [3.14159265358870], [1.09467990228040e-12],

    table.hline(),
  ),
  caption: [Rendimiento comparativo entre distintas versiones de MPI para 8 procesos.],
)

Por lo observado, usar demasiados procesos ralentiza el tiempo de ejecución. Esto puede ser por la cantidad de comunicación que debe hacerse para un problema simple como este.

Podemos observar que los errores difieren entre cada versión. Esto es completamente esperado, ya que la precisión de punto flotante hace que los resultados sean distintos. Para la versión secuencial, los valores se suman de la siguiente manera:
#figure(
  ```
  (((R1 + R2) + R3) + R4 + ...)
  ```,
  caption: [Suma de áreas de rectángulos en la versión secuencial.],
)

#figure(
  ```
  (A1 + A2) + (A3 + A4) + ...
  ```,
  caption: [Suma de áreas de rectángulos en la versión Reduce. La suma se hace en manera de árbol según los procesos. Cada $A$ representa el área que cubre cada proceso. Entonces, el error de punto flotante es distinto al secuencia, ya que no se sumaron de manera lineal.],
)

#figure(
  ```
  (((A1 + A2) + A3) + A4 + ...)
  ```,
  caption: [Suma de áreas de rectángulos en la versión Send. Notemos que aquí difiere de la versión secuencial porque las $A$ representan las áreas obtenidas de cada proceso.],
)

#figure(
  ```
  (((A1 + A2) + A3) + A4 + ...)
  ```,
  caption: [Suma de áreas de rectángulos en la versión Allgather. La suma es igual al Send, por lo que esperamos un error idéntico.],
)

= Conclusiones
Cada ejecución resultó en un error distinto. Esto se explica por el orden en el
que se sumaron las áreas de cada rectángulo. Los tiempos obtenidos indican que
las versiones paralelas tienen un mejor rendimiento que la secuencial. Tanto
`Reduce` como `Send` con `Receive` y `Allgather` tienen un desempeño similar para estos
valores. Sin embargo, la complejidad de `Reduce` es de $O(log N)$, ya que la
suma se hace a manera de árbol. Esta diferencia se notaría más con el uso de un
mayor número de procesos.
