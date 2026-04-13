#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lilaq:0.6.0" as lq
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 5",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
    Visualizamos la propagación de calor en una placa bidimensional en ParaView.
    Comparamos los resultados en cada iteración para validad la correctitud de la
    implementación en paralelo.
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Introducción
En la tarea anterior, presentamos resultados donde se seguía el comportamiento
de una celda en partícular en la placa bidimensional. Ahora, visualizaremos la
placa entera utilizando #link("https://www.paraview.org/")[ParaView].

= Cambios al código OpenMPI
En la tarea anterior, presentamos el código en paralelo que nos permitía seguir
el valor de una celda específica. Para poder ver toda la placa, debemos
enviar el valor que tiene cada celda al proceso principal. Es decir, debemos
utilizar `MPI_Send` para cada proceso hijo. Esto lo haremos un total de 20 veces,
sin importar el número de iteraciones (asumiendo que tenemos al menos 20 iteraciones).
Una vez que el proceso maestro reciba los valores de cada hijo, se imprimen los resultados
a un archivo `.vkt`. La implementación de `save_paraview` para la escritura de estos archivos le pertenece al
Dr. Ángel Uh Zapata.

#figure(
  ```cpp
          std::vector<double> recv_buffer(N * local_width);
          for (int p = 1; p < size; p++) {
            MPI_Recv(recv_buffer.data(), N * local_width, MPI_DOUBLE, p, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int p_start_col = p * col_size;
            for (int j = 0; j < N; j++) {
              for (int i = 0; i < col_size; i++) {
                int global_col = p_start_col + (i - 1);
                global_U[global_col][j] = recv_buffer[i + j * local_width];
              }
            }
          }
          save_paraview(N, N, x, y, (double)iter, global_U, iter / print_freq, "output");
  ```,
  caption: [Código agregado a la implementación previa en paralelo.],
)

Si el nodo es un hijo, simplemente enviamos los valores de las celdas al nodo maestro:

#figure(
  ```cpp
        MPI_Send(u.data(), N * local_width, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  ```,
  caption: [Código agregado a la implementación previa en paralelo.],
)
= Visualizaciones
Las siguientes imagenes son resultado de una simulación con $N = 1000$, hasta
$500,000$ iteraciones.

#grid(
  columns: 2,
  figure(
    image("2d-0.png"),
    caption: [Estado inicial.],
  ),
  figure(
    image("2d-1.png"),
    caption: [Estado final.],
  ),
)

#grid(
  columns: 2,
  figure(
    image("3d-00.png"),
    caption: [Estado inicial.],
  ),
  figure(
    image("3d-01.png"),
    caption: [Estado final.],
  ),

  figure(
    image("3d-11.png"),
    caption: [Estado final.],
  ),
)


= Conclusiones
El estado inicial (aunque muy difícil de notar) es un punto con valor de 100.
En las siguientes iteraciones se propaga el calor, lentamente hasta alcanzar el
estado final. Notemos que, a pesar de que el centro tiene una temperatura constante,
la placa no se estabiliza en el valor constante. Esto se debe a que hay un factor
de enfriamiento $1 - 4 alpha = 0.6$.
