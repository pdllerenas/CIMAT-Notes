#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/cetz:0.5.0": canvas, draw, tree
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 6",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Problema 1
Usando la idea del *pre-proceso de la cadena de caracteres en KMP* (para
determinar en cada posición los mayores prefijos que son también sufijos, i.e.
el arreglo resets de la clase), escribir un programa que determine el numero de
sub-cadenas distintas dentro de una cadena (o sea, el mismo problema que vimos
con la función z).

Por ejemplo, para el input:
$
  s = "BLABLA"
$
se espera el output siguiente
#figure(
  ```
  15
  ```,
)
Analizar su complejidad.
== Solución
Para solucionar el problema, realizaremos el preprocesamiento de la cadena al revés $p^(-1)$ de cada prefijo $p$ de $s$.
Notemos que esto funciona ya que los sufijos de $p^(-1)$ son prefijos de la cadena $p$. Por ejemplo:

$
  "BLAB " -> " BALB"
$
Y los sufijos de la palabra al revés son B, BL, BLA, BLAB. Entonces, el arreglo
`resets` que corresponde a $p^(-1)$ es
$
  K_4 = [0, 0, 0, 1],
$
el de LBALB es
$
  K_5 = [0, 0, 0, 1, 2],
$
y el de ALBALB
$
  K_6 = [0, 0, 0, 1, 2, 3].
$
En este caso, coinciden los de ambas cadenas. Notemos entonces que el valor máximo de cada arreglo
corresponde a la longitud máxima de sufijos que ya existe como sub-cadena antes del índice $i$.
Entonces, para cada $i$, el número de cadenas nuevas que se generan son $i + 1 - max(K_(i + 1))$.
En el ejemplo BLABLA, tenemos entonces
$
  1 + 2 + 3 + (4 - 1) + (5 - 2) + (6 - 3) = 15.
$
Para la implementación, recorremos la cadena caracter por caracter. Realizamos el pre-proceso para cada
prefijo al revés, calculamos su máximo, y lo sumamos al resultado final.
== Complejidad
La complejidad temporal es $O(n^2)$, ya que recorremos toda la cadena, y en cada iteración, realizamos una llamada a `kmpPreprocess`, que es
$O(n)$ en promedio. La complejidad espacial es $O(n)$, el tamaño del arreglo `resets`, que es a lo más $n$, por lo que lo podemos fijar al
tamaño máximo de $s$ o crearlo dinámicamente.
== Ejecución
Compilamos con
#figure(
  ```sh
  g++ p1.cpp -o p1
  ```,
)
Y ejecutamos con
#figure(
  ```sh
  ./p1
  ```,
)
El input es simplemente la cadena a la que queremos aplicarle el algoritmo. Por ejemplo,
#figure(
  ```
  BLABLA
  ```,
)
tiene como output
#figure(
  ```
  15
  ```,
)

= Problema 2
Usando alguno de los algoritmos vistos en las primeras sesiones (en particular,
KMP o función $z$), escribir un programa que reciba cadenas de caracteres
*S* (hasta $10^5$ caracteres) y determine el *palíndromo más corto* que se
pueda formar agregando caracteres al final de *S*.

Por ejemplo, con *S* siendo la siguiente cadena:

#figure(
  ```
  ARANAN
  ```,
)
el palíndromo más corto que se puede formar a partir de *S*, agregando caracteres, es

#figure(
  ```
  ARANANARA
  ```,
)

== Solución
Primero, encontramos el palíndromo más largo dentro de la cadena $S$. En el ejemplo, es ARA. Para hacer esto,
construimos la Longest Prefix Suffix de
$
  "reversed"(S) + | + S
$
Para nuestro ejemplo,
#figure(
  ```
  NANARA|ARANAN
  [0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 2, 3]
  ```,
)
Es decir, hemos realizado el siguiente matcheo:

#figure(
  ```
  ARANAN
     NANARA
  ```,
)
Entonces, el palíndromo más corto es
#figure(
  ```
  ARANANARA
  ```,
)
== Complejidad
Dado que solo necesitamos realizar el pre-proceso 1 vez, e invertimos la cadena
1 vez, la complejidad temporal es $O(N)$. La espacial es de $O(N)$, ya que
hicimos una copia de la cadena original.

== Ejecución

Compilamos con
#figure(
  ```sh
  g++ p2.cpp -o p2
  ```,
)
Y ejecutamos con
#figure(
  ```sh
  ./p2
  ```,
)
El input es simplemente la cadena a la que queremos aplicarle el algoritmo. Por ejemplo,
#figure(
  ```
  ARANAN
  ```,
)
tiene como output
#figure(
  ```
  ARANANARA
  ```,
)

= Problema 3
Resolver el siguiente problema de Codeforces: https://codeforces.com/problemset/problem/471/D

== Solución
El problema consiste en encontrar las ocurrencias de una figura en otra. Es
decir, si una persona tiene una figura hecha de bloques:
#figure(
  canvas({
    import draw: *

    let fill-color = rgb("c4c4c4")
    let box-stroke = stroke(black + 0.5pt)
    let axis-stroke = stroke(black + 1pt)

    let left-x = -7
    for (i, h) in ((0, 3), (1, 4), (2, 4), (3, 3), (4, 2)) {
      for y in range(1, h + 1) {
        rect((left-x + i, y), (left-x + i + 1, y + 1), fill: fill-color)
      }
    }
  }),
  caption: [Ejemplo de construcción de bloques representado por `3 4 4 3 2`.],
)

Si tenemos otra estructura

#figure(
  canvas({
    import draw: *

    let fill-color = rgb("c4c4c4")
    let box-stroke = stroke(black + 0.5pt)
    let axis-stroke = stroke(black + 1pt)

    for x in range(1, 9) {
      rect((x, -1), (x + 1, 0))
    }
    for x in range(1, 9) {
      rect((x, 0), (x + 1, 1))
    }
    for x in (1, 7, 8) {
      rect((x, 1), (x + 1, 2))
    }

    let mid-x = 2
    for (i, h) in ((0, 3), (1, 4), (2, 4), (3, 3), (4, 2)) {
      for y in range(1, h + 1) {
        rect((mid-x + i, y), (mid-x + i + 1, y + 1), fill: fill-color)
      }
    }

    let right-x = 9
    for (i, h) in ((0, 3), (1, 4), (2, 4), (3, 3), (4, 2)) {
      for y in range(-1, h - 1) {
        rect((right-x + i, y), (right-x + i + 1, y + 1), fill: fill-color)
      }
    }
    line((0.5, 0), (14.2, 0)) // Eje X
    line((1, -1.2), (1, 5.5)) // Eje Y

    for y in (-1, 1, 2, 3, 4, 5) {
      content((0.5, y), [#y])

      // Determinar hasta qué columna llega la línea punteada
      let end-x = 1
      if y == 5 { end-x = 3 } else if y == 4 or y == 3 { end-x = 2 }

      line((0.6, y), (end-x, y), stroke: (paint: black, dash: "dashed", thickness: 0.5pt))
    }
    // El número 0 del origen
    content((0.5, 0), [0], anchor: "east")

    // 7. Números del Eje X (dibujados sobre las cajas inferiores)
    for x in range(1, 14) {
      content((x + 0.5, -1.5), [#x])
    }
  }),
  caption: [Ejemplo de construcción alterna, donde encontramos 2 patrones
    similares al de la construcción anterior. Este ejemplo corresponde al input `2 4 5 5 4 3 2 2 2 3 3 2 1`.],
)
Podemos mover la estructura original hacia arriba o abajo, como podemos observar en
los recuadros sombreados, la estructura original es comienza originalmente en 0,
pero _enterramos_ la estructura original para que coincidiera con la forma de
ésta.

Para solucionar el problema, debemos observar cómo cambian las alturas de las
estructuras. Es decir, si la altura de la estructura de longitud $N$ en cada $x_i$ es dada por
$h_i$, queremos comparar los arreglos

$
  [h_2 - h_1, dots, h_(i+1) - h_i, dots, h_N - h_(N-1)].
$
de cada estructura. Esto funciona porque las coincidencias entre estos arreglos
nos dicen que tienen la misma forma de _picos_.
== Complejidad
Sea $m$ la longitud de la primer estructura. Sea $n$ la de la segunda estructura.
El cálculo de los arreglos de diferencias es $O(n + m)$. Realizar el pre-proceso es $O(m)$. Realizar el algoritmo KMP es $O(n+m)$, por lo que la complejidad temporal final es $O(n+m)$. La complejidad espacial es $O(n + m)$, por los arreglos de diferencias.
== Ejecución
Compilamos con
#figure(
  ```sh
  g++ p3.cpp -o p3
  ```,
)
Y ejecutamos con
#figure(
  ```sh
  ./p3
  ```,
)
Luego, input esperado es de la forma
#figure(
  ```
  n m
  i_1 ... i_n
  j_1 ... j_m
  ```,
)
Por ejemplo,

#figure(
  ```
  13 5
  2 4 5 5 4 3 2 2 2 3 3 2 1
  3 4 4 3 2
  ```,
)
Genera el output

#figure(
  ```
  2
  ```,
)