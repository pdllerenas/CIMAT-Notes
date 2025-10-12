#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Métodos Numericos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 7",
  date: datetime.today(),
)

#question[
  Desarrolla un programa que resulva un sistema de ecuaciones de la forma
  $bold(A x) = bold(b)$ utilizando el *método QR*. (*10 puntos*)
]

El método QR se caracteriza por descomponer una matriz cuadrada $A$ de
dimensiones $m times m$ en el producto de dos matrices $Q R$, donde $Q$ es una
matriz ortonormal ) de tamaño $m times m$, y $R$ es una matriz triangular
superior de tamaño $m times m$.

Una vez realizada la descomposición, podemos utilizar el hecho de que $Q^(-1) =
Q^(T)$ para simplificar el sistema de ecuaciones lineales $bold(A x) = bold(b)$
de la siguiente manera:
#align(
  center,
  $bold(A x &= b) \
  bold(Q R x &= b) \
  bold(R x &= Q^T b)$,
)
Entonces, aplicamos un solucionador especifico para matrices triangulares, con
el vector constante siendo $bold(Q^T b)$.

#set math.vec(delim: "[")
#set math.mat(delim: "[")
=== Ejemplos:
- *Sistema de $3 times 3$*:
#align(
  center,
  $
    mat(
      3, -0.1, -0.2;
      -0.1, 7, -0.3;
      -0.2, -0.3, 10;
    )
    vec(
      x_1,
      x_2,
      x_3
    )
    =
    vec(
      7.85,
      -19.3,
      71.4
    ).
  $,
)

Ejecutando nuestro programa con
```
make run-p1 ARGS="ex/A1.txt ex/b1.txt"
```
Obtenemos la siguiente descomposición de $bold(A)$:
#align(center, grid(
  columns: 2,
  align: center + horizon,
  gutter: 20pt,
  [$bold(Q) = mat(
    -0.99723, -0.03022, -0.067905;
    0.03324, -0.99849, -0.043687;
    0.06648, 0.04582, -0.996735
  )$],
  [$bold(R) = mat(
    -3.00832,0.31246,0.854297;
    0.00000,-7.00016,0.763830;
    0.00000,0.00000,-9.940661;
  ).$],
))
Solucionando el sistema anteriormente mencionado, obtenemos
#align(
  center,
  $
  bold(x) = 
    vec(
      x_1,
      x_2,
      x_3
    )
    =
    vec(
      3.01157,
      -2.40863,
      7.12797,
    ).
  $,
)
Podemos validar con las tareas anteriores que, efectivamente, el sistema se resuelve correctamente.

- *Sistema de $125 times 125$*:
En este sistema, realizamos el mismo procedimiento. Para correr el programa, usamos
```
make run-p1 ARGS="ex/A2.txt ex/b2.txt"
```

dfkajsfdlk
