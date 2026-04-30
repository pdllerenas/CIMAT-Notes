#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/cetz:0.5.0"
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 1",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],
  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Uso de cláusulas
1. En el código `Prog_1.c`, primero se imprimen los procesadores e hilos disponibles en la máquina:

#figure(
  ```c
   printf ( "  Number of processors available = %d\n", omp_get_num_procs ( )  );
   printf ( "  Number of threads =              %d\n", omp_get_max_threads ( )  );
  ```,
)
Después, se comparan los tiempos de ejecución de 3 funciones. La primera, `int number_1( int n )`,
realiza una paralelización donde $n in NN$ es una variable compartida, y los indices $i, j, "number"$ son
privadas. El bucle se paraleliza mediante `reduction`, que realiza la suma
$
  sum_(i = 2)^n bb(1)_("isprime") = pi(n),
$
Es decir, la función que cuenta el número de primos hasta el número $n$.
#figure(
  ```c
  # pragma omp parallel \
    shared ( n ) \
    private ( i, j, number )

  # pragma omp for reduction ( + : total )
    for ( i = 2; i <= n; i++ )
    {
      number = 1;

      for ( j = 2; j < i; j++ )
      {
        if ( i % j == 0 )
        {
          number = 0;
          break;
        }
      }
      total = total + number;
    }
  ```,
)
Dado que no se especifica cómo dividir el rango, el compilador decide la
división. En el caso de GCC, se realiza una estática. Es decir, cada hilo
tiene una carga equitativa de $(n-1)/p$. Notemos que el bucle interior no es
paralelizado; cada hilo lo realiza de manera secuencial.

// #figure(
//   diagram(
//     node-corner-radius: 2pt,
//     {
//       let tint(c) = (stroke: c, fill: rgb(..c.components().slice(0, 3), 5%))
//       node(enclose: ((0, 0),), ..tint(blue), name: <one>)
//       node(enclose: ((1, 0),), ..tint(red), name: <two>)
//       node(enclose: ((2, 0),), ..tint(green), name: <three>)
//       node(enclose: ((4, 0),), ..tint(purple), name: <n>)
//       node((0, 0), [Thread 1])
//       node((1, 0), [Thread 2])
//       node((2, 0), [Thread 3])
//       node((3, 0), [...])
//       node((4, 0), [Thread n])

//       node(enclose: ((0, 3), (3, 2), (4, 3)), ..tint(rgb(0, 0, 0)), name: <n>)

//       edge((0.2, 1.8), (-0.2, 0), stroke: 0.5pt, "-|>")
//       edge((0.8, 1.81), (1.2, 0), stroke: 0.5pt, "-|>")
//       edge((1.4, 1.81), (2.4, 0), stroke: 0.5pt, "-|>")
//       edge((2.7, 1.81), (3.0, 0.7), stroke: 0.5pt, "-|>")
//       edge((3.7, 1.81), (4.4, 0), stroke: 0.5pt, "-|>")

//       edge((0.5, 1.81), (0.5, 3.19), stroke: 0.5pt)
//       edge((1.1, 1.81), (1.1, 3.19), stroke: 0.5pt)
//       edge((1.7, 1.81), (1.7, 3.19), stroke: 0.5pt)
//       edge((3.4, 1.81), (3.4, 3.19), stroke: 0.5pt)
//     },
//   ),
//   caption: [
//     División de trabajo, donde el bloque gris representa la carga completa de trabajo. La división es equitativa.
//   ],
// )

#figure(
  cetz.canvas({
    import cetz.draw: *
    rect((1, -1), (7, 0), stroke: none, name: "all")
    content("all", [Workload])
    rect((0, 0), (1, 1), name: "r1", stroke: blue)
    rect((1, 0), (2, 1), name: "r2", stroke: blue)
    rect((2, 0), (3, 1), name: "r3", stroke: blue)

    rect((3, 0), (5, 1), stroke: none, name: "rectangle")
    content("rectangle", [...], color: blue)

    rect((5, 0), (6, 1), name: "r4", stroke: blue)
    rect((6, 0), (7, 1), name: "r5", stroke: blue)

    rect((1, 4), (7, 5), stroke: none, name: "allp")
    content("allp", [Threads])

    rect((0, 3), (1, 4), name: "t1", stroke: red)
    rect((1, 3), (2, 4), name: "t2", stroke: red)
    rect((2, 3), (3, 4), name: "t3", stroke: red)

    rect((3, 3), (5, 4), stroke: none, name: "rectanglep")
    content("rectanglep", [...], color: blue)
    rect((5, 3), (6, 4), name: "t4", stroke: red)
    rect((6, 3), (7, 4), name: "t5", stroke: red)

    set-style(mark: (symbol: ">>"))
    line("r1", "t1")
    line("r2", "t2")
    line("r3", "t3")
    line("r4", "t4")
    line("r5", "t5")
  }),
  caption: [
    La carga de trabajo se divide en el número de hilos. Cada hilo tiene la misma carga de trabajo.
  ],
)

La segunda función `int number_2(int n)` tiene la siguiente paralelización:
#figure(
  ```c
  # pragma omp parallel \
    shared ( n ) \
    private ( i, j, number )

  # pragma omp for reduction ( + : total ) schedule ( static, 100 )
    for ( i = 2; i <= n; i++ )
    {
      number = 1;

      for ( j = 2; j < i; j++ )
      {
        if ( i % j == 0 )
        {
          number = 0;
          break;
        }
      }
      total = total + number;
    }
  ```,
)

La única diferencia con el código anterior es el uso de `schedule (static, 100)`. Esto le dice al
compilador que queremos tamaños fijos en los bloques que se le reparten a cada hilo. Entonces, la carga
de trabajo de cada hilo no necesariamente es equitativa, como se observa en la @fig:2.

#figure(
  cetz.canvas({
    import cetz.draw: *
    rect((1, -1), (7, 0), stroke: none, name: "all")
    content("all", [Workload])
    rect((0, 0), (1, 1), name: "r1", stroke: blue)
    content("r1", [100])
    rect((1, 0), (2, 1), name: "r2", stroke: blue)
    content("r2", [100])
    rect((2, 0), (3, 1), name: "r3", stroke: blue)
    content("r3", [100])
    rect((5, 0), (6, 1), name: "r5", stroke: blue)
    content("r5", [100])

    rect((6, 0), (7, 1), name: "r6", stroke: blue)
    content("r6", [100])
    rect((7, 0), (8, 1), name: "r7", stroke: blue)
    content("r7", [100])

    rect((3, 0), (5, 1), stroke: none, name: "rectangle")
    content("rectangle", [...], color: blue)

    rect((1, 4), (7, 5), stroke: none, name: "allp")
    content("allp", [Threads])

    rect((0, 3), (1, 4), name: "t1", stroke: red)
    content("t1", [1])
    rect((1, 3), (2, 4), name: "t2", stroke: red)
    content("t2", [2])
    rect((2, 3), (3, 4), name: "t3", stroke: red)
    content("t3", [3])

    rect((3, 3), (5, 4), stroke: none, name: "rectanglep")
    content("rectanglep", [...], color: blue)
    rect((5, 3), (6, 4), name: "t5", stroke: red)
    content("t5", $n$)
    rect((6, 3), (7, 4), name: "t6", stroke: red)
    content("t6", [1])
    rect((7, 3), (8, 4), name: "t7", stroke: red)
    content("t7", [2])

    set-style(mark: (symbol: ">>"))
    line("r1", "t1")
    line("r2", "t2")
    line("r3", "t3")
    line("rectangle", "rectanglep")
    line("r5", "t5")
    line("r6", "t6")
    line("r7", "t7")
  }),
  caption: [
    La carga de trabajo es estática. En el ejemplo del código, 100. Una vez se acaben los hilos, se comienza a asignar más trabajo comenzando desde el primer hilo de nuevo, como se observa en la figura.
  ],
)<fig:2>

En la última función del programa, `int number_3(int n)`, tenemos solamente la modificación del uso de `dynamic` en la distribución de trabajo. Ahora, en vez de asignar trabajo de manera round-robin, se asigna
al primer hilo que se encuentre disponible. Así, nos aseguramos de que cada hilo esté trabajando. Entonces,
en la @fig:2, los hilos no se encuentran necesariamente en orden ascendente.

#figure(
  ```c
  # pragma omp parallel \
    shared ( n ) \
    private ( i, j, number )

  # pragma omp for reduction ( + : total ) schedule ( dynamic, 100 )
    for ( i = 2; i <= n; i++ )
    {
      number = 1;

      for ( j = 2; j < i; j++ )
      {
        if ( i % j == 0 )
        {
          number = 0;
          break;
        }
      }
      total = total + number;
    }
  ```,
)
2. El programa `Prog_2.c` tiene 4 regiones de interés.
#figure(
  ```c
  printf ( "  omp_get_max_threads ( ) = %d\n", omp_get_max_threads ( ) );
  printf ( "  omp_get_num_threads ( ) = %d\n", omp_get_num_threads ( ) );
  printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );
  printf ( "  omp_get_num_procs ( )   = %d\n", omp_get_num_procs ( ) );
  printf ( "  omp_get_wtime ( ) =       %g\n", omp_get_wtime ( ) );
  ```,
)
Primero, se ejecutan las lineas anteriores con todos los valores por default.
Lo obtenido en mi máquina es lo siguiente:

#figure(
  ```
   omp_get_max_threads ( ) = 8
   omp_get_num_threads ( ) = 1
   omp_get_thread_num ( ) =  0
   omp_get_num_procs ( )   = 8
   omp_get_wtime ( ) =       1.77735e+09
  ```,
  caption: [
    La primer linea indica el máximo número de hilos por procesador. La segunda indica los hilos que tenemos trabajando. La tercera el hilo en el que estamos. La cuarta el número de procesadores. La última es el tiempo transcurrido desde 01/01/1970.
  ],
)
La segunda región del programa tiene las mismas impresiones, pero utiliza la siguiente función antes de ejecutarlas:

#figure(
  ```c
  omp_set_num_threads ( 5 );
  ```,
  caption: [
    Esto le dice al compilador que prepare 5 hilos para nuestro programa.
  ],
)

#figure(
  ```
  omp_get_max_threads ( ) = 5
  omp_get_num_threads ( ) = 1
  omp_get_thread_num ( ) =  0
  omp_get_num_procs ( )   = 8
  omp_get_wtime ( ) =       1.77735e+09
  ```,
  caption: [
    Lo único distinto al previo ejemplo es el número de hilos. Ahora, con la llamada a la función, se settea a 5.
  ],
)
La tercera región es la siguiente:

#figure(
  ```c
    omp_set_num_threads ( 3 );
    printf ( "Output #3\n" );
  # pragma omp parallel
  {
    printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );

  # pragma omp single
    {
      printf ( "\n" );
      printf ( "  omp_get_max_threads ( ) = %d\n", omp_get_max_threads ( ) );
      printf ( "  omp_get_num_threads ( ) = %d\n", omp_get_num_threads ( ) );
      printf ( "  omp_get_thread_num ( ) =  %d\n", omp_get_thread_num ( ) );
      printf ( "  omp_get_num_procs ( )   = %d\n", omp_get_num_procs ( ) );
      printf ( "  omp_get_wtime ( ) =       %g\n", omp_get_wtime ( ) );
      printf ( "\n \n" );
    }
  }
  ```,
  caption: [
    Ahora, usamos 3 hilos. En la región paralelizada, el primer `printf` es ejecutado por todos los hilos. Luego, dado que usamos `single`, solo se ejecuta por un hilo, el primero que logre llegar a esa linea.
  ],
)

#figure(
  ```
    omp_get_thread_num ( ) =  0
    max_threads ( ) = 3
    omp_get_num_threads ( ) = 3
    omp_get_thread_num ( ) =  0
    omp_get_num_procs ( )   = 8
    omp_get_wtime ( ) =       1.77735e+09


    omp_get_thread_num ( ) =  2
    omp_get_thread_num ( ) =  1
  ```,
  caption: [
    Notemos que solamente el hilo 0 ejecuta el bloque en `single`.
  ],
)

La última región mide el tiempo transcurrido en un `sleep(5)`:

#figure(
  ```c
    t1 = omp_get_wtime ( );
    sleep ( 5 );
    t2 = omp_get_wtime ( );
    printf ( "  omp_get_wtime estimates duration of sleep(5) as %g seconds.\n", t2 - t1 );
  ```,
)
Esto nos imprime:

#figure(
  ```
  omp_get_wtime estimates duration of sleep(5) as 5.011 seconds.
  ```,
)
Esto nos dice que hay error de magnitud $10^(-2)$ en la medición del tiempo con
las funciones de OpenMP.

= Métodos de ordenamiento
Para generar los número aleatorios, usamos `rand()` y usamos la operación módulo para mantenerlos en un rango. Aunque la distribución de estos no es uniforme, basta para los fines del ejercicio.

#figure(
  ```c
  fprintf(fp, "SIZE %d\n", N);
  for (int i = 0; i < N; i++)
  {
    fprintf(fp, "%d\n", (rand() % M));
  }
  ```,
  caption: [
    Escribe $N$ números aleatorios al archivo al que apunta `fp`.
  ],
)

#figure(
  ```c
  int *SortFile(char *filename, void (*sort)(int *A, int N))
  {
    FILE *fp = fopen(filename, "r");

    // read header
    read = getline(&line, &len, fp);
    sscanf(line, "SIZE %d", &size);

    int *arr = (int *)malloc(size * sizeof(int));

    while ((read = getline(&line, &len, fp)) != -1 && k < size)
    {
      arr[k++] = atoi(line);
    }
    sort(arr, size);

    free(line);
    fclose(fp);

    return arr;
  }
  ```,
  caption: [
    Dado el nombre de un archivo con formato de tamaño y los dígitos separados
    en líneas, se crea un arreglo ordenado de dichos números. Se usa un
    apuntador a una función para mantener el algoritmo de ordenación abstracto.
  ],
)

#figure(
  ```c
  t0 = clock();
  int *arr1 = SortFile(filename, BubbleSort);
  tf = clock();
  cpu_time_used = ((double)(tf - t0)) / CLOCKS_PER_SEC;
  printf("Bubble sort time: %lf s\n", cpu_time_used);
  ```,
  caption: [Uso de `BubbleSort` y `SortFile` para ordenar los números dentro del archivo `filename`, y se imprime el tiempo de ejecución.],
)

== Compilación y ejecución
Para compilar, usamos
#figure(
  ```sh
  gcc p2.c sort.c -o p2
  ```,
)
Para ejecutar, se esperan 3 argumentos, el tamaño del arreglo, el número máximo
en los números aleatorios, y el archivo de salida.
#figure(
  ```sh
  ./p2 100000 500 out.txt
  ```,
)
== Resultados

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header([*Algoritmo*], [*Tiempo (s)*]),
    table.hline(),
    [Bubble], [26.282665],
    [OddEven], [19.765198],
    [Insertion], [6.049853],
    table.hline(),
  ),
  caption: [
    A pesar de que los 3 algoritmos son $O(n^2)$, la constante de cada uno cambia drásticamente el tiempo final.
  ],
)

= Ecuación de Poisson 2D
Consideremos el siguiente problema:
$
  (partial^2)/(partial x^2) u + (partial^2)/(partial y^2) u = f(x,y) quad "en " Omega = [0,1] times [0,1],\
  u(x,y) = g(x,y) " en " partial Omega.
$
con
$
  f(x,y) = -2sin x cos y
$
La solución analítica es
$
  u(x,y) = sin x cos y.
$
== Jacobi
Para la solución usando Jacobi, realizamos la actualización mediante la siguiente fórmula:
$
  u^(k+1)_(i,j) = -1/4 h^2 f_(i,j) + 1/4 (u^k_(i+1, j) + u^k_(i-1,j)+u^k_(i,j+1)+u^k_(i,j-1))
$
En código, lo implementamos de la siguiente manera:
#figure(
  ```c
  #pragma omp parallel for shared(u, u_new, N) reduction(max : max_diff)
      for (int i = 1; i < N - 1; i++)
      {
        for (int j = 1; j < N - 1; j++)
        {
          u_new[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)]);

          // norm infinity
          double diff = fabs(u_new[IDX(i, j, N)] - u[IDX(i, j, N)]);
          if (diff > max_diff)
            max_diff = diff;
        }
      }
  ```,
  caption: [
    Iteraciones de Jacobi con $norm(dot)_infinity$.
  ],
)

== Gauss-Seidel
Para solucionar el problema utilizando Gauss-Seidel, usamos la siguiente fórmula:
$
  u^(k+1)_(i,j) = -1/4 h^2 f_(i,j) + 1/4 (u^(k+1)_(i+1, j) + u^(k+1)_(i-1,j)+u^(k+1)_(i,j+1)+u^(k+1)_(i,j-1))
$
Notemos que en este caso, debemos tomar los valores de la iteración actual. Entonces, la paralelización no es trivial como en el caso de Jacobi. En su lugar, dividimos la cuadrícula generada en 2 partes:

#figure(
  grid(
    fill: (x, y) => rgb(
      if (calc.odd(x + y) and x > 0 and y < 8 and x < 8 and y > 0) { "000E" } else if (
        x == 0 or y == 0 or y == 8 or x == 8
      ) { "f80F" } else { "800F" },
    ),
    columns: (3em,) * 9,
    rows: 3em,
    align: center + horizon,

    ..(grid.cell(y: 8)[],),
  ),
  caption: [
    Cuadrícula $9 times 9$, donde los valores #text([naranja], fill: rgb("f80F")) representan los valores iniciales de la frontera. Los recuadros #text([negros], fill: rgb("000E")) y #text([rojos], fill: rgb("800F")) son separados para que en las iteraciones pares (o impares) de Gauss-Seidel, cada cuadro rojo solo interactúe con los valores de los cuadros negros, mientras que en las iteraciones impares (o pares), los cuadros negros solo interactúen con los valores de los cuadros rojos.
  ],
)
Con esta división, no nos preocupamos por condiciones de carrera al actualizar un valor $u^(k+1)_(i,j)$, ya que las actualizaciones de los cuadros negros y rojos se hacen por separado.

#figure(
  ```c
      // even squares
  #pragma omp parallel for shared(u, N) reduction(+ : l2_sum)
      for (int i = 1; i < N - 1; i++)
      {
        for (int j = 1; j < N - 1; j++)
        {
          if ((i + j) % 2 == 0)
          {
            double x = i * h, y = j * h;
            double f_term = h * h * f(x, y);

            double old_val = u[IDX(i, j, N)];
            u[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

            double diff = fabs(u[IDX(i, j, N)] - old_val);
            l2_sum += diff * diff;
          }
        }
      }

      // odd squares
  #pragma omp parallel for shared(u, N) reduction(+ : l2_sum)
      for (int i = 1; i < N - 1; i++)
      {
        for (int j = 1; j < N - 1; j++)
        {
          if ((i + j) % 2 != 0)
          {
            double x = i * h, y = j * h;
            double f_term = h * h * f(x, y);

            double old_val = u[IDX(i, j, N)];
            u[IDX(i, j, N)] = 0.25 * (u[IDX(i - 1, j, N)] + u[IDX(i + 1, j, N)] + u[IDX(i, j - 1, N)] + u[IDX(i, j + 1, N)] - f_term);

            double diff = fabs(u[IDX(i, j, N)] - old_val);
            l2_sum += diff * diff;
          }
        }
      }
  ```,
  caption: [
    Gauss-Seidel usando la norma $norm(dot)_2$. Notemos que aquí aún no se realiza el escalado por el área de los cuadros ($h^2$).
  ],
)

== Compilación y ejecución
Podemos compilarlo de tal manera que el código producido sea serial o paralelo. Para la versión serial, usar
#figure(
  ```sh
  gcc -O3 p3.c -o p3
  ```,
)
Para la versión paralela, usar
#figure(
  ```sh
  gcc -O3 -fopenmp p3.c -o p3_mp
  ```,
)
Dado que la actividad solo pide analizar la versión paralela, solo usaremos la segunda. Para ejecutarla, usamos
#figure(
  ```sh
  ./p3_mp <discretización> <hilos>
  ```,
)
Por ejemplo,
#figure(
  ```sh
  ./p3_mp 4 8
  ```,
)
nos genera los resultados de hacer una malla $4 times 4$, usando 8 hilos.

== Resultados
#grid(
  figure(
    table(
      stroke: none,
      columns: (3fr, 8fr, 7fr),
      [], [_Jacobi_], [_Gauss-Seidel_],
    ),
  ),
  figure(
    table(
      columns: 9,
      stroke: none,
      table.hline(),
      table.header(
        [*Discretización*],
        [*Iteraciones*],
        $bold(L_2)$,
        $bold(L_infinity)$,
        [*Tiempo (s)*],
        [*Iteraciones*],
        $bold(L_2)$,
        $bold(L_infinity)$,
        [*Tiempo (s)*],
      ),
      table.hline(),
      // Grid, Jacobi (Iter, L2, Linf, Time), Gauss-Seidel (Iter, L2, Linf, Time)
      $bold(4 times 4)$, [15], [0.000008], [0.000007], [0.002], [8], [0.000005], [0.000010], [0.001],
      $bold(8 times 8)$, [84], [0.000009], [0.000009], [0.006], [46], [0.000010], [0.000009], [0.008],
      $bold(16 times 16)$, [328], [0.000010], [0.000010], [0.020], [180], [0.000010], [0.000010], [0.020],
      $bold(32 times 32)$, [1129], [0.000010], [0.000010], [0.064], [633], [0.000010], [0.000010], [0.072],
      $bold(64 times 64)$, [3531], [0.000010], [0.000010], [0.232], [2045], [0.000010], [0.000010], [0.287],
      $bold(256 times 256)$, [10000], [0.000012], [0.000026], [5.229], [10000], [0.000011], [0.000022], [5.127],
      table.hline(),
    ),
    caption: [Resultados usando 4 hilos.],
  ),
)
#grid(
  figure(
    table(
      stroke: none,
      columns: (3fr, 8fr, 7fr),
      [], [_Jacobi_], [_Gauss-Seidel_],
    ),
  ),
  figure(
    table(
      columns: 9,
      stroke: none,
      table.hline(),
      table.header(
        [*Discretización*],
        [*Iteraciones*],
        $bold(L_2)$,
        $bold(L_infinity)$,
        [*Tiempo (s)*],
        [*Iteraciones*],
        $bold(L_2)$,
        $bold(L_infinity)$,
        [*Tiempo (s)*],
      ),
      table.hline(),
      $bold(4 times 4)$, [15], [0.000008], [0.000007], [0.001], [8], [0.000005], [0.000010], [0.001],
      $bold(8 times 8)$, [84], [0.000009], [0.000009], [0.007], [46], [0.000010], [0.000009], [0.007],
      $bold(16 times 16)$, [328], [0.000010], [0.000010], [0.026], [180], [0.000010], [0.000010], [0.026],
      $bold(32 times 32)$, [1129], [0.000010], [0.000010], [0.077], [633], [0.000010], [0.000010], [0.087],
      $bold(64 times 64)$, [3531], [0.000010], [0.000010], [0.254], [2045], [0.000010], [0.000010], [0.289],
      $bold(256 times 256)$, [10000], [0.000012], [0.000026], [2.320], [10000], [0.000011], [0.000022], [4.197],
      table.hline(),
    ),
    caption: [
      Resultados usando 8 hilos.
    ],
  ),
)
== Conclusiones
Se observa una gran mejora para discretizaciones más pequeñas al usar más hilos.
Además, notamos que en general, el algoritmo de Gauss-Seidel logra converger más
rápido (como sugiere la teoría). Aunque esto sea cierto, el tiempo de ejecución
no es en general menor. Esto se debe a que el algoritmo no es tan _altamente
paralelizable_, como lo es el de Jacobi. También notamos que la norma $L_2$
converge más rápido para mallados más precisos.
