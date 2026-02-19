#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Cómputo Paralelo I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 2",
  date: datetime(year: 2026, month: 2, day: 6),
)

#set text(lang: "es")
#question[
  Dada la siguiente función de error:
  $
    "Error"(I_1, I_2) = sum_(r=1)^N sum_(c = 1)^M (I_1(r,c) - I_2(r,c))^2,
  $
  donde $I_1$ e $I_2$ son matrices de tamaño $N times M$ que contienen valores de punto flotante de $32$ bits (float), paraleliza la función usando:
  1. OpenMP
  2. CUDA con memoria global
  3. CUDA con memoria compartida
  4. Evaluar el tiempo de procesamiento y _speedup_ entre las versiones Serial y Parelelo usando OpenMP, Paralelo usando CUDA con memoria global, y Paralelo usando CUDA con memoria compartida, para diferentes tamaños de matrices (por ejemplo, $N = M = 512$, $N = M = 1024$, $N = M = 2048$, $N = M = 4096$, $N = M = 8192$).
]

== Implementaciones
=== Secuencial
#show figure: set align(start)
Para el codigo secuencial, realizamos una implementación directa de la función
de error, iterando sobre cada elemento de las matrices y acumulando el
resultado.
#figure(
  ```cpp
    auto start = std::chrono::high_resolution_clock::now();

    for (int repeat = 0; repeat < REPEAT; ++repeat)
    {
      for (int i = 0; i < N * M; ++i)
        E += (A[i] - B[i]) * (A[i] - B[i]);
    }

    auto end = std::chrono::high_resolution_clock::now();
  ```,
  caption: [Código secuencial para calcular el error entre dos matrices. Realizamos un bucle adicional para repetir el cálculo varias veces y obtener tiempos de ejecución más estables.],
)

La compilacion del codigo se realiza con el siguiente comando:
```bash
g++ -o error_seq error_seq.cpp
```
Al ejecutar el programa, se le pide al usuario insertar las dimensiones de las matrices. En nuestro caso, se probaron con matrices de tamaño $512 times 512$, $1024 times 1024$, $2048 times 2048$, $4096 times 4096$, y $8192 times 8192$. Luego, se pide un numero de semilla, que solo se utiliza para generar matrices de entrada aleatorias, y se muestra el resultado del error calculado y el tiempo de ejecución.


=== OpenMP
Para la versión con OpenMP, utilizamos una directiva `#pragma omp parallel for reduction(+ : E)`. Esto permite que múltiples hilos calculen partes
de la suma de manera concurrente, y la cláusula `reduction` asegura que la
variable `E` se actualice correctamente sin condiciones de carrera#footnote[Una
  condicion de carrera consiste en varios hilos accediendo a la misma variable sin
  sincronización, causando un posible resultado erroneo al finaliar el calculo.].

#figure(
  ```cpp
      auto start = omp_get_wtime();

      // reduction clause to safely accumulate E across threads
  #pragma omp parallel for reduction(+ : E)
      for (int i = 0; i < N * M; ++i)
        E += (A[i] - B[i]) * (A[i] - B[i]);

      auto end = omp_get_wtime();
  ```,
  caption: [Código OpenMP para calcular el error entre dos matrices. Usamos la función de tiempo `omp_get_wtime()` para medir el tiempo de ejecución en vez de `chrono`, ya que una mide el tiempo de CPU y la otra el tiempo de pared, lo que es más relevante para programas paralelos.],
)
La compilacion del codigo se realiza con el siguiente comando:
```bash
g++ -fopenmp -o error_omp error_omp.cpp
```
Al ejecutar el programa, se le pide al usuario insertar las dimensiones de las matrices. En nuestro caso, se probaron con matrices de tamaño $512 times 512$, $1024 times 1024$, $2048 times 2048$, $4096 times 4096$, y $8192 times 8192$. Luego, se pide un numero de semilla, que solo se utiliza para generar matrices de entrada aleatorias, y se muestra el resultado del error calculado y el tiempo de ejecución.

=== CUDA con memoria global y compartida
En la implementación con CUDA, se lanzan kernels que calculan partes de la suma
en paralelo. Para la versión con memoria global, cada hilo accede directamente a
los datos en la memoria global de la GPU, mientras que para la versión con
memoria compartida, los datos se cargan primero en la memoria compartida de cada
bloque antes de realizar el cálculo, lo que puede mejorar el rendimiento al
reducir el número de accesos a la memoria global.

La compilacion del codigo se realiza con el siguiente comando:
```bash
nvcc -o error_cuda_global error_cuda_global.cu
nvcc -o error_cuda_shared error_cuda_shared.cu
```
(*Nota*: yo use el compilador de Visual Studio en vez de la compilacion con `nvcc`, pero el comando deberia funcionar).

Al ejecutar el programa, se le pide al usuario insertar las dimensiones de las matrices. En nuestro caso, se probaron con matrices de tamaño $512 times 512$, $1024 times 1024$, $2048 times 2048$, $4096 times 4096$, y $8192 times 8192$. Luego, se pide un numero de semilla, que solo se utiliza para generar matrices de entrada aleatorias, y se muestra el resultado del error calculado y el tiempo de ejecución.

#{
  set text(
    font: "New Computer Modern",
    size: 8pt,
  )
  figure(
    table(
      align: center,
      columns: 8,
      table.header(
        [Size ($N times M$)],
        "Serial (ms)",
        "OpenMP (ms)",
        "CUDA Global (ms)",
        "CUDA Shared (ms)",
        "Speedup OpenMP",
        "Speedup CUDA Global",
        "Speedup CUDA Shared",
      ),
      $512 times 512$,
      [ 2.27 ms],
      [ 2 ms],
      [ 0.79 ms],
      [ 0.58 ms],
      [ #{ calc.round(2.27 / 2.0, digits: 2) }],
      [ #{ calc.round(2.27 / 0.79, digits: 2) }],
      [ #{ calc.round(2.27 / 0.58, digits: 2) } ],

      $1024 times 1024$,
      [ 9.51 ms],
      [ 5 ms],
      [ 2.68 ms],
      [ 1.96 ms],
      [ #{ calc.round(9.51 / 5.0, digits: 2) }],
      [ #{ calc.round(9.51 / 2.68, digits: 2) }],
      [ #{ calc.round(9.51 / 1.96, digits: 2) } ],

      $2048 times 2048$,
      [ 37.11 ms],
      [ 11 ms],
      [ 10.43 ms],
      [ 7.44 ms],
      [ #{ calc.round(37.11 / 11.0, digits: 2) }],
      [ #{ calc.round(37.11 / 10.43, digits: 2) }],
      [ #{ calc.round(37.11 / 7.44, digits: 2) } ],

      $4096 times 4096$,
      [ 147.28 ms],
      [ 44 ms],
      [ 36.90 ms],
      [ 23.90 ms],
      [ #{ calc.round(147.28 / 44.0, digits: 2) }],
      [ #{ calc.round(147.28 / 36.90, digits: 2) }],
      [ #{ calc.round(147.28 / 23.90, digits: 2) } ],

      $8192 times 8192$,
      [ 590.43 ms],
      [ 161 ms],
      [ 161.24 ms],
      [ 103.81 ms],
      [ #{ calc.round(590.43 / 161, digits: 2) }],
      [ #{ calc.round(590.43 / 161.24, digits: 2) }],
      [ #{ calc.round(590.43 / 103.81, digits: 2) }],
    ),
    caption: [Tiempos de ejecución y speedup para cada programa para matrices de entradas aleatorias.],
  )
}
