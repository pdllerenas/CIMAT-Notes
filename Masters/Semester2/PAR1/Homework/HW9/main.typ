#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lovelace:0.3.1": *
#import "@preview/cetz:0.5.0"
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 2",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],
  author: "Pedro D. Llerenas",

  show-solutions: false,
)
= Matrices en Octave
El siguiente código genera la gráfica de la matriz correspondiente.
#figure(
  ```m
  L = load('large.mat')
  spy(L.A)
  ```,
)

#grid(
  columns: 2,
  row-gutter: 5pt,
  figure(
    image("small_sparse.png"),
    caption: [
      Small sparse matrix.
    ],
  ),
  figure(
    image("heat_sparse.png"),
    caption: [
      Heat sparse matrix.
    ],
  ),
  grid.cell(
    colspan: 2,
    figure(
      image("large.png", width: 50%),
      caption: [
        Large sparse matrix.
      ],
    ),
  ),
)

= Gradiente Conjugado Precondicionado
El algoritmo de gradiente conjugado condicionado es el siguiente:
#figure(
  kind: "algorithm",
  supplement: [Algoritmo],

  pseudocode-list(booktabs: true, numbered-title: [Gradiente Conjugado Precondicionado])[
    + *Input* $(A, b, x_0, tau, N_max)$
    + $g_0 <- A x_0 - b$
    + $q_0 <- M^(-1) g_0$ ($M = "diag"(A)$ si es precondicionado de Jacobi)
    + $p_0 <- -q_0$
    + *while* $g_k != 0$ && $k < N_max$
      + $w <- A p_k$
      + $alpha_k <- (g^top_k q_k)/(p_k^top w)$
      + $x_(k+1) <- x_k + alpha_k p_k$
      + $g_(k+1) <- g_k + alpha_k w$
      + $q_(k+1) <- M^(-1)g_(k+1)$
      + $beta_(k) <- (g_(k+1)^top q_(k+1))/(g_k^top q_k)$
      + $p_(k+1) <- -q_(k+1)+beta_(k+1)p_k$
      + $k = k+1$
    + *end*
    + *return* $x_k$
  ],
) <cool>

Condensamos los problemas 2, 3 y 4 en un solo programa `p3.c`, que usa una
versión en `C` de los programas `read.cpp` y `save.cpp` proporcionados por el
Dr. Luis Blanco para la lectura y escritura de matrices.

Primero, se debe leer el archivo `.mat`, que es de tipo binario. El encabezado
contiene 5 enteros. La tabla @table:header resume los datos que encontramos en
el encabezado.

#figure(
  table(
    stroke: none,
    columns: 3,
    table.header([*Posición*], [*Descripción*], [*Dato*]),
    table.hline(),
    [0], table.vline(), [Tipo de matriz], table.vline(), [0-52],
    [1], [Número de renglones], [int],
    [2], [Número de columnas], [int],
    [3], [Reales o complejos], [0,1],
    [4], [Longitud del nombre], [0-64],
  ),
)<table:header>

Una vez extraído, debemos ver el tipo de matriz y decidir si utilizar el método
de gradiente conjugado para matrices esparzas o completas. Usualmente (como es en este caso),
el archivo `.mat` contiene los vectores `b` y `x` (en este orden) que describen el sistema
$
  A x = b.
$
Para leerlos, se utiliza el mismo encabezado @table:header.

Con los datos listos, usamos la función `conjugate_gradient_sparse` o
`conjugate_gradient`, según el valor 0 del encabezado. Para simplificar el
código, asumimos que el dato de entrada es un `double`.

En la implementación solo usamos variantes de las siguientes directivas:

#figure(
  ```c
  #pragma omp parallel for default(none) shared(A, x0, Ax0, N)
  #pragma omp parallel for reduction(+ : rr_next) default(none) shared(rk, zk, N)
  #pragma omp atomic
  ```,
)

La primer línea es una paralelización simple de un bucle. La segunda realiza la
paralelización asegurando que la variable `rr_next`
no sea editada por dos procesos a la vez, ya que dentro del bucle se usará
`rr_next += ...`. Es decir, previene condiciones de carrera. La última de igual
manera se usa para evitar condiciones de carrera en una línea específica. En
nuestro caso, se usa para que al multiplicar las matrices esparzas por vectores,
no se edite el resultado de una casilla de manera simultanea.

== Compilación y ejecución
Para compilar, usamos
#figure(
  ```sh
  gcc p3.c utils/conjugate_gradient.c -fopenmp -O3 -lm -o p3
  ```,
)
Luego, ejecutamos con

#figure(
  ```sh
  ./p3 <file_name.mat> <output.mat> <threads>
  ```,
)

== Ejemplo
Por ejemplo, podemos correrlo con
#figure(
  ```sh
  ./p3 heat_full.mat heat_full_out.mat 4
  ```,
)
Lo cual nos imprime lo siguiente:

#figure(
  ```
  Number of threads set: 4
  format: 0 (full matrix)
  type:   0 (double)
  rows:  125
  cols:  125
  field: 0 (real)
  name:  A
  Method converged at 12 iterations.

  --- Results ---
  L2 Error Norm:       4.192996e-01
  L_infinity Error Norm: 9.827298e-02
  Time: 0.000656 s
  --------------------------
  ```,
)
Las primeras líneas describen la cantidad de hilo utilizados, lo que se leyó en el archivo, y las iteraciones que se necesitaron para la convergencia con tolerancia $tau = 1times 10^(-5)$. El apartado de resultados muestra el error con el vector $x$ que se encuentra en el archivo. Es decir, el error de la solución respecto a la solución real. Al final, el tiempo de ejecución con el uso de `omp_get_wtime`, que nos da la hora del reloj de pared.

== Resultados
#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header([*Matriz*], [*Error $L_2$*], [*Error $L_infinity$*], [*Iteraciones*], [*Tiempo (s)*]),
    table.hline(),
    [`small_sparse`], $1.8 times 10^(-17)$, $8.67 times 10^(-18)$, $10$, $0.0002$,
    [`heat_sparse`], $4.19 times 10^(-1)$, $9.82 times 10^(-2)$, $12$, $0.0003$,
    [`large`], $2.11 times 10^(-2)$, $8.84 times 10^(-4)$, $233$, $0.1186$,
    table.hline(),
  ),
  caption: [Resultados con 2 threads.],
)
#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header([*Matriz*], [*Error $L_2$*], [*Error $L_infinity$*], [*Iteraciones*], [*Tiempo (s)*]),
    table.hline(),
    [`small_sparse`], $2.17 times 10^(-17)$, $9.54 times 10^(-18)$, $10$, $0.0003$,
    [`heat_sparse`], $4.19 times 10^(-1)$, $9.82 times 10^(-2)$, $12$, $0.0004$,
    [`large`], $2.11 times 10^(-2)$, $8.84 times 10^(-4)$, $233$, $0.1059$,
    table.hline(),
  ),
  caption: [Resultados con 4 threads.],
)

#figure(
  table(
    stroke: none,
    columns: 5,
    table.hline(),
    table.header([*Matriz*], [*Error $L_2$*], [*Error $L_infinity$*], [*Iteraciones*], [*Tiempo (s)*]),
    table.hline(),
    [`small_sparse`], $1.90 times 10^(-17)$, $8.67 times 10^(-18)$, $10$, $0.0005$,
    [`heat_sparse`], $4.19 times 10^(-1)$, $9.82 times 10^(-2)$, $12$, $0.0006$,
    [`large`], $2.11 times 10^(-2)$, $8.84 times 10^(-4)$, $233$, $0.0976$,
    table.hline(),
  ),
  caption: [Resultados con 6 threads.],
)
