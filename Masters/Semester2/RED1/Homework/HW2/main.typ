#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Programación de Redes I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 2",
  date: datetime(year: 2026, month: 2, day: 25),
)

#set text(lang: "es")
#question[
  Dada una imagen de entrada, obtenga la frecuencia de cada uno de sus pixeles usando 1, 2, 4 procesos.
  1. Determine el tiempo de ejecución para cada caso (ver ANEXO 1);
  2. ¿El tiempo de ejecución con 2 y 4 procesos es 2 y 4 veces menor que un solo proceso?
]

1.Para compilar y ejecutar el código, usamos
```
gcc fork_img_freq.c pgm_io.c -o fork_freq
./fork_freq images/lena_ascii.pgm 1
./fork_freq images/barbara_ascii.pgm 1
./fork_freq images/fractal_ascii.pgm 1
```
Donde el primer argumento es la image, y el segundo el numero de procesos a usar. La siguiente tabla contiene los tiempos de CPU según el número de procesos e imágenes usadas.
#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*N*], [*Lena*], [*Barbara*], [*Fractal*]),
    table.hline(),
    $1$, $0.000339 s$, $0.000311 s$, $0.003149 s$,
    $2$, $0.000392 s$, $0.000386 s$, $0.003490 s$,
    $4$, $0.000572 s$, $0.000601 s$, $0.003756 s$,
    table.hline(),
  ),
  caption: [Comparación de tiempos de cálculo de frecuencias en imagen.],
)
2. No. Parece incrementar, que puede ser debido al hecho de que al final de cada proceso hijo, el padre debe realizar la suma de cada elemento del arreglo del proceso hijo, haciendo que los otros hijos esperen.

#question[
  Implementar la suma y multiplicación de matrices 3D mediante $J$ procesos creados con la función *fork()*. Usar matrices cuadradas cuyas dimensiones y número de procesos son dados como línea de comando y presentar una tabla comparativa de los tiempos de ejecución.
]

Para compilar los programas, usamos
```
gcc matrix_product.c -o matrix_p
gcc matrix_sum.c -o matrix_s
```
Para ejecutar el producto de 2 matrices, usamos
```
./matrix_p <Dimension> <Procesos> <Semilla>
```
Por ejemplo:
```
./matrix_p 1000 1 101
```
Para la suma, usamos
```
./matrix_s <Dimension_1> <Dimension_2> <Dimension_3> <Procesos> <Semilla>
```
Por ejemplo,
```
./matrix_s 300 300 300 1 101
```
La siguiente tabla muestra algunos resultados obtenidos:

#figure(
  table(
    stroke: none,
    columns: 6,
    table.hline(),
    table.header([*N*], [*M*], [*K*], [*P = 1*], [*P = 2*], [*P = 4*]),
    table.hline(),
    $100$, $100$, $100$, $0.000278 s$, $0.000688 s$, $0.000824 s$,
    $300$, $300$, $300$, $0.001560 s$, $0.002993 s$, $0.006633 s$,
    $500$, $500$, $500$, $0.005981 s$, $0.011787 s$, $0.023188 s$,
    table.hline(),
  ),
  caption: [Comparación de tiempos para cálculo de suma de matrices.],
)
#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*N*], [*P = 1*], [*P = 2*], [*P = 4*]),
    table.hline(),
    $100$, $0.000127 s$, $0.000215 s$, $0.000363 s$,
    $300$, $0.000144 s$, $0.000220 s$, $0.000456 s$,
    $500$, $0.000173 s$, $0.000347 s$, $0.000541 s$,
    $1000$, $0.000218 s$, $0.000359 s$, $0.000666 s$,
    table.hline(),
  ),
  caption: [Comparación de tiempos para cálculo de producto de matrices.],
)

Por lo observado, usar `fork` añade un overhead significativo, lo que hace que
el rendimiento baje a comparación de un solo proceso.

#question[
  Dada una imagen de entrada, calcular el filtro de la media y mediana usando $N = {1, 2, 4, 9}$ procesos. Presentar una tabla comparativa con los tiempos de ejecución.
]

Para compilar el código, usamos
```
gcc filter.c kernel_io.c pgm_io.c convolution.c compare.c -o filter
```
Para ejecutarlo, usamos
```
./filter <input> <output> <filter> <window size> <processes>
```
Por ejemplo:
```
./filter images/lena_ascii.pgm images/lena_mean.pgm mean 3 1
```
Las siguientes tablas muestran los tiempos de ejecución

#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*N*], [*Lena*], [*Barbara*], [*Fractal*]),
    table.hline(),
    $1$, $0.000600 s$, $0.000668 s$, $0.005762 s$,
    $2$, $0.000807 s$, $0.000674 s$, $0.005940 s$,
    $4$, $0.000817 s$, $0.000829 s$, $0.006473 s$,
    $9$, $0.001381 s$, $0.001268 s$, $0.007644 s$,
    table.hline(),
  ),
  caption: [Comparativa de tiempos de ejecución para filtro de la media.],
)

#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*N*], [*Lena*], [*Barbara*], [*Fractal*]),
    table.hline(),
    $1$, $0.000444 s$, $0.000409 s$, $0.003214 s$,
    $2$, $0.000722 s$, $0.000521 s$, $0.003064 s$,
    $4$, $0.000765 s$, $0.000696 s$, $0.003526 s$,
    $9$, $0.001213 s$, $0.001129 s$, $0.003897 s$,
    table.hline(),
  ),
  caption: [Comparativa de tiempos de ejecución para filtro de la mediana.],
)
