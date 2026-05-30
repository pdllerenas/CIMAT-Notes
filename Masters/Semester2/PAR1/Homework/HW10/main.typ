#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lovelace:0.3.1": *
#import "@preview/cetz:0.5.0"
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 3",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],
  author: "Pedro D. Llerenas",

  show-solutions: false,
)
= Gradiente Conjugado Mejorado
El algoritmo de gradiente conjugado mejorado es el siguiente:
#figure(
  kind: "algorithm",
  supplement: [Algoritmo],

  pseudocode-list(booktabs: true, numbered-title: [Gradiente Conjugado Mejorado])[
    + *Input* $(A, b, x_0, tau, N_max)$
    + $g_0 <- A x_0 - b$
    + $p_0 <- -g_0$
    + *while* $g_k != 0$ && $k <= "rank"(A)$ && $norm(g_k) > epsilon$
      + $w <- A p_k$
      + $alpha_k <- (g^top_k g_k)/(p_k^top w)$
      + $x_(k+1) <- x_k + alpha_k p_k$
      + $g_(k+1) <- g_k + alpha_k w$
      + $beta_(k) <- (g_(k+1)^top g_(k+1))/(g_k^top g_k)$
      + $p_(k+1) <- -g_(k+1)+beta_(k+1)p_k$
      + $k = k+1$
    + *end*
    + *return* $x_k$
  ],
)

= Implementación en MPI (Condiciones de Dirichlet)
Para implementar el algoritmo en MPI, dividimos la carga de trabajo en 4 procesos
esclavos, mientras que el nodo maestro se encarga de recibir
los resultados de cada esclavo para unificarlo.

Primero, separamos el ambiente de comunicación del maestro y los esclavos:
#figure(
  ```c
  MPI_Comm worker_comm;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &worker_comm);
  ```,
  caption: [Definir una nueva región para los trabajadores. Esto nos permite
    ignorar el nodo maestro sin condicionar el rango del proceso. `MPI_Comm_split`
    crea un nuevo comunicador dependiendo del color. En este caso, los esclavos
    son de un color, y el maestro de otro.],
)

Cada proceso esclavo tomará una porción de los nodos totales. Dado que cada nodo
depende de sus vecinos, algunos nodos quedan sin su vecino al realizar la división.
Por ello, tenemos un nodo fantasma que funge como el comunicador entre los procesos
asignados a regiones adyacentes.

#figure(
  ```c
      MPI_Scatterv(u_prev, sendcounts, displs, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  ```,
  caption: [El nodo maestro ejecuta esta línea, que le informa a los esclavos su rango de trabajo y la aproximación inicial `u_prev`.],
)

Luego, definimos las condiciones de frontera de Dirichlet.
#figure(
  ```c
    for (int i = 0; i < N; i++)
    {
      double x = i * dx;
      u_prev[i] = 4.0 * x - 4.0 * x * x;
    }
  ```,
  caption: [Condiciones de Dirichlet.],
)

#figure(
  ```c
  MPI_Sendrecv(&u[local_N - 1], 1, MPI_DOUBLE, right_neighbor, 0, &left_ghost, 1, MPI_DOUBLE, left_neighbor, 0, comm, MPI_STATUS_IGNORE);
  MPI_Sendrecv(&u[0], 1, MPI_DOUBLE, left_neighbor, 1, &right_ghost, 1, MPI_DOUBLE, right_neighbor, 1, comm, MPI_STATUS_IGNORE);
  ```,
  caption: [Intercambio de información entre procesos adyacentes para los nodos fantasma. `MPI_Sendrecv` envía y recibe información. En este caso, se envía el primer y último nodo a los vecinos adyacentes, y se reciben los nodos fantasma de los nodos adyacentes.],
)

Dado que cada proceso trabaja de manera independiente, se deben combinar ciertos cálculos, como puede ser una norma. Esto se resuelve mediante
`MPI_AllReduce`.
#figure(
  ```c
  MPI_Allreduce(&local_rr, &global_rr, 1, MPI_DOUBLE, MPI_SUM, comm);
  ```,
  caption: [Este comando nos permite calcular la suma global a partir de las locales sin condiciones de carrera.],
)

#figure(
  ```c
      MPI_Gatherv(NULL, 0, MPI_DOUBLE, u_prev, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      for (int i = 0; i < N; i++)
      {
        fprintf(fout, "%f%s", u_prev[i], (i == N - 1) ? "" : ",");
      }
  ```,
  caption: [Una vez todos los esclavos hayan terminado, se imprimen los resultados de la iteración. En este caso, `MPI_Gatherv` funge como un bloqueo para asegurarnos que todos los procesos hayan terminado.],
)

Los esclavos son los que corren el algoritmo de gradiente conjugado, de la siguiente manera:

#figure(
  ```c
      MPI_Scatterv(NULL, NULL, NULL, MPI_DOUBLE, local_u, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      conjugate_gradient_3diag_MPI(diag, off_diag, local_u, local_N, TOL, 1000, worker_comm);
      MPI_Gatherv(local_u, local_N, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  ```,
  caption: [Cada nodo recibe su rango de trabajo desde el nodo maestro mediante
    `MPI_Scatterv`, donde `0` representa el nodo maestro, y recibe los valores de
    `local_u`, de tamaño `local_N`. Luego se corre el gradiente conjugado con dichos
    parámetros, y se envía el valor de los resultados locales del vector
    `local_u`, con un tamaño de `local_N`.],
)
= Compilación y ejecución
== Secuencial
Para compilar la versión secuencial, usamos
#figure(
  ```sh
  gcc seq.c utils/conjugate_gradient.c -o seq
  ```,
)
Y para ejecutar, usamos
#figure(
  ```sh
  ./seq <nodos> <lambda>
  ```,
)

Por ejemplo,
#figure(
  ```sh
  ./seq 62 1.0
  ```,
)
Escribe al archivo `results.csv`, donde contiene la información de la ecuación
de calor en cada nodo, para cada tiempo.
== MPI
Para compilar la versión MPI, usamos

#figure(
  ```sh
  mpicxx mpi.c utils/conjugate_gradient.c -D USE_MPI -o mpi
  ```,
)
Y ejecutamos con
#figure(
  ```sh
  mpiexec -n 5 ./mpi 102 1.0
  ```,
)
De igual manera, esto nos escribe a `results.csv`.

= Resultados
#figure(
  image(
    "heat_plot_62.png",
  ),
  caption: [Mapa de calor de la simulación con las condiciones de Dirichlet, 62 nodos.],
)

#figure(
  image(
    "heat_plot_102.png",
  ),
  caption: [Mapa de calor de la simulación con las condiciones de Dirichlet, 102 nodos.],
)


#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*Versión*], [*Nodos*], [*Tiempo (s)*], [*Speedup*]),
    table.hline(),
    [Secuencial], $62$, $0.242$, $1$,
    [Secuencial], $102$, $0.433$, $1$,
    [MPI], $62$, $0.075$, $3.22$,
    [MPI], $102$, $0.238$, $1.81$,
    table.hline(),
  ),
  caption: [Comparativa de tiempos con $lambda = 1.0$],
)

= Conclusiones
Observamos un speedup considerable para la implementación en MPI. Ambos proveen
resultados idénticos, por lo que concluimos que la versión MPI logra superar a
la versión secuencia.
