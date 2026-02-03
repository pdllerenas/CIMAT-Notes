#import "@preview/adaptable-pset:0.2.0": *
#import "@preview/ctheorems:1.1.3": *
#show: thmrules.with(qed-symbol: $square$)

#let proof = thmproof("proof", "Proof")

// Feel free to omit any of the below, just set it to "" and it won't show
#let title = "Tarea 1"
#let author = "Pedro D. Llerenas"
#let collaborators = []
#let course-id = "PAR1: Cómputo Paralelo"
#let instructor = "Dr. Francisco J. Hernández López"
#let semester = "Enero-Julio 2026"
#let due-time = "Febrero 6"

#set text(lang: "es")

#show: homework.with(
  title: title,
  author: author,
  collaborators: collaborators,
  course-id: course-id,
  instructor: instructor,
  semester: semester,
  due-time: due-time,

  // Optional setting to change the paper size depending on region
  // (Defaults to A4)
  // paper-size: "us-letter",
)

// Numbering
#set enum(numbering: "a)")

#prob(title: "", color: rgb("#009999"))[
  Considere el siguiente código:
  #figure(
    ```cpp
    int N = 10;
    int x = 1;
    int y[N];
    omp_set_num_threads(5);
    #pragma omp parallel for firstprivate(x)
    for (int i = 0; i < N; i++) {
      y[i] = x + i;
      x = i;
    }
    ```,
  )
  1. ¿Cuál es el valor de $y$ al finalizar el ciclo `for`?
  2. ¿Cuál es el valor de $x$ al final del ciclo `for`?
]

1. Al finalizar el ciclo,

#figure(
  ```cpp
  y = [1, 1, 3, 5, 5, 9, 7, 13, 9, 17]
  ```,
)

Esto se debe a la manera en que los threads fueron asignados al `for`. La
siguiente tabla muestra la asignación:
#figure(
  table(
    columns: 2,
    table.header([*Thread*], [*i*]),
    $0$, $0, 1$,
    $1$, $2, 3$,
    $2$, $4, 5$,
    $3$, $6, 7$,
    $4$, $8, 9$,
  ),
)
Entonces, dado que usamos `firstprivate` en la directiva, el valor de $x$ es
inicializado a $x = 1$ para cada thread, y la privacidad hace que el
comportamiento sea bien definido, ya que cada thread tiene un estado
único del valor de $x$.

Entonces, dada esta partición, en el thread 0 tenemos
#figure(
  ```cpp
  // i = 0
  y[0] = x + 0; // 1 + 0
  x = 0;

  // i = 1
  y[1] = x + 1; // 0 + 1
  x = 1;
  ```,
  caption: [Asignaciones del thread 0],
)

#figure(
  ```cpp
  // i = 2
  y[2] = x + 2; // 1 + 2
  x = 2;

  // i = 3
  y[3] = x + 3; // 2 + 3
  x = 3;
  ```,
  caption: [asignaciones del thread 1],
)

#figure(
  ```cpp
  // i = 4
  y[4] = x + 4; // 1 + 4
  x = 4;

  // i = 5
  y[5] = x + 5; // 4 + 5
  x = 5;
  ```,
  caption: [Asignaciones del thread 2],
)

Las threads 3 y 4 tienen un comportamiento idéntico, resultando en la lista
previamente mencionada.

#prob(title: "", color: rgb("#009999"))[
  La suma de los elementos de un vector de tamaño $N$.
]
#figure(
  ```cpp
  #pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < N; i++) {
      sum += v[i];
    }
  ```,
)
Notemos que usamos `reduction` para resolver la _race condition_ en la variable `sum`.
#figure(
  ```
  g++ p2.cpp -fopenmp -o p2
  ```,
)
Ejemplos:

Sea
#let v = range(1, 11).map(i => i)
$
  v = #v
$
Entonces, su suma es
$
  sum_(i = 1)^10 i = (10 dot 11) / 2 = 55.
$
Al ejecutar el programa, el siguiente input
#figure(
  ```
  10
  1 2 3 4 5 6 7 8 9 10
  ```,
)
Genera el siguiente output
#figure(
  ```
  55
  ```,
)



#prob(title: "", color: rgb("#009999"))[
  La multiplicación de dos matrices cuadradas de tamaño $N times N$.
]

#figure(
  ```cpp
  #pragma omp parallel for
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        int sum = 0;
        for (int k = 0; k < N; k++) {
          sum += m1[N * i + k] * m2[N * k + j];
        }
        res[N * i + j] = sum;
      }
    }
  ```,
)
En este caso, usamos una sola instrucción de paralelización para el loop. Esto
nos paraleliza el loop de la variable `i`. Notemos que, a pesar de que tenemos
una suma dentro de los loops, no debemos usar `reduction`. Esto se debe a la
independencia de la variable `sum` entre cada `i`.

#figure(
  ```
  g++ p3.cpp -fopenmp -o p3
  ```,
)

Ejemplos:

Sean
$
  A = mat(12, 13; 31, 23), quad B = mat(123, 341; 423, 143).
$
Entonces, su producto es
$
  A dot B = mat(6975, 5951; 13542, 13860).
$
Al ejecutar el programa, el siguiente input
#figure(
  ```
  2
  12 13
  31 23
  123 341
  423 143
  ```,
)
Genera el siguiente output
#figure(
  ```
  6975 5951
  13542 13860
  ```,
)
