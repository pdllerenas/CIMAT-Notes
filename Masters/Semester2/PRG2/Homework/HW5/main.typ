#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 5",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Problema 1

Tenemos un banco de $n$ problemas algorítmica $p_i$ para $i = 1, ..., n$.

Después de un trabajo de análisis profundo de estos problemas, has logrado
establecer una lista de $m$ reducciones de problema $q$ en problema $p$:
$
  p -> q
$
siginificando que, si tienes un algoritmo que permite resolver $p$, entonces
sabrás resolver $q$ con el mismo algoritmo.

Escribir un programa que determine el número mínimo $n_min$ de algoritmos que
escribir para que todos los problemas del banco de problemas se puedan
resolver, o directamente, o por la la reducción. Por ejemplo, si tienes tres
problemas 1, 2, 3 y que sabes que 3 se reduce a 1, entonces bastará tener 2
algoritmos para resolver los 3, y eso es el mínimo.

El programa deberá leer $m,n$ así como una lista de reducciones $p_i -> q_i$
donde $1 <= p_i, q_i <= n$. Deberá poder manejar hasta $10^5$ problemas
(identificados por enteros positivos) y $10^5$ especificaciones de reducciones.

#figure(
  $
    bold("Entrada")\
    "n m"\
    p_1 " " q_1\
    p_2 " " q_2\
    p_3 " " q_3\
    dots.v " " dots.v\
    p_m " " q_m\
    bold("Salida")\
    n_min
  $,
)

== Solución
Tenemos $n$ nodos que reperesentan problemas distintos. Construimos un grafo
dirigido donde las aristas representan las reducciones dadas. Al obtener este
grafo, generamos el grafo condensado que resulta a partir del grafo cociente
sobre la relación de equivalencia dada por las componentes fuertemente conexas.
Es decir, el resultado es un DAG. Cada nodo del DAG representa 1 algoritmo distinto.
Las aristas representan una reducción, que no necesariamente genera una equivalencia.
Entonces, buscamos los nodos que no tienen ancestro.
=== Ejemplo
Supongamos que tenemos la entrada
#figure(
  ```
    8 12
    1 5
    1 6
    5 6
    3 1
    3 8
    6 3
    6 8
    8 2
    2 8
    4 8
    8 7
    7 2
  ```,
)
Nos genera el siguiente grafo dirigido:

#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `1`, name: <1>),
    edge(<1>, "-|>", <5>, label-size: 8pt),
    edge(<1>, "-|>", <6>, label-size: 8pt),
    node((0, 1), `5`, name: <5>),
    edge(<5>, "-|>", <6>, label-size: 8pt),
    node((1, 1), `6`, name: <6>),
    edge(<6>, "-|>", <3>, label-size: 8pt),
    edge(<6>, "-|>", <8>, label-size: 8pt),
    node((1, 0), `3`, name: <3>),
    edge(<3>, "-|>", <1>, label-size: 8pt),
    edge(<3>, "-|>", <8>, label-size: 8pt),
    node((2, 1), `8`, name: <8>),
    edge(<8>, "-|>", <2>, label-size: 8pt),
    edge(<8>, "-|>", <7>, label-size: 8pt),
    node((2, 0), `4`, name: <4>),
    edge(<4>, "-|>", <8>, label-size: 8pt),
    node((3, 0), `7`, name: <7>),
    edge(<7>, "-|>", <2>, label-size: 8pt),
    node((3, 1), `2`, name: <2>),
    edge(<2>, "-|>", <8>, label-size: 8pt),
  ),
  caption: [Grafo dirigido inicial.],
)<graph:1>

El grafo condensado resultante es el siguiente:

#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `0`, name: <0>),
    edge(<0>, "-|>", <1>, label-size: 8pt),
    node((0, 1), `1`, name: <1>),
    edge(<2>, "-|>", <1>, label-size: 8pt),
    node((1, 0), `2`, name: <2>),
  ),
  caption: [Grafo condensado.],
)<graph:1>
Dado que solo hay 2 nodos sin ancestros, la respuesta es 2.

Para ejecutar y verificar nuestro programa con esta entrada, compilamos con
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
Luego, insertamos la entrada previamente expuesta.

= Problema 2

Re-escribe el algoritmo de ordenamiento topológico para poder recuperar la
lista de todos los ordenamientos topológicos posibles. El algoritmo leerá la
descripción de un DAG y dará la lista de los ordenamientos topológicos
posibles.

#figure(
  $
    bold("Entrada")\
    "n m"\
    i_1 " " e_1\
    i_2 " " e_2\
    i_3 " " e_3\
    dots.v " " dots.v\
    i_m " " e_m\
    bold("Salida")\
    "Impresión de " bold("todos") " los ordenamientos topológicos posibles"
  $,
)

== Solución
Para encontrar todos los ordenamientos topológicos, usaremos el algoritmo de
Kahn, que se basa en usar BFS. Buscamos los nodos sin ancestros, los agregamos
a una lista, y los borramos del grafo. Para obtener todos los ordenamientos,
utilizamos backtracking.

=== Ejemplo
Supongamos que tenemos el siguiente grafo dirigido:

#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `1`, name: <1>),
    edge(<1>, "-|>", <5>, label-size: 8pt),
    edge(<1>, "-|>", <6>, label-size: 8pt),
    node((0, 1), `5`, name: <5>),
    edge(<5>, "-|>", <6>, label-size: 8pt),
    node((1, 1), `6`, name: <6>),
    edge(<6>, "-|>", <8>, label-size: 8pt),
    node((1, 0), `3`, name: <3>),
    edge(<3>, "-|>", <1>, label-size: 8pt),
    edge(<3>, "-|>", <8>, label-size: 8pt),
    node((2, 1), `8`, name: <8>),
    edge(<8>, "-|>", <2>, label-size: 8pt),
    edge(<8>, "-|>", <7>, label-size: 8pt),
    node((2, 0), `4`, name: <4>),
    edge(<4>, "-|>", <8>, label-size: 8pt),
    node((3, 0), `7`, name: <7>),
    edge(<7>, "-|>", <2>, label-size: 8pt),
    node((3, 1), `2`, name: <2>),
  ),
  caption: [Grafo dirigido.],
)<graph:2>

Es decir, tenemos el input

#figure(
  ```
    8 10
    1 5
    1 6
    5 6
    3 1
    3 8
    6 8
    8 2
    4 8
    8 7
    7 2
  ```,
)
Esto nos genera los siguientes ordenamientos:
#figure(
  ```
  3 1 4 5 6 8 7 2
  3 1 5 4 6 8 7 2
  3 1 5 6 4 8 7 2
  3 4 1 5 6 8 7 2
  4 3 1 5 6 8 7 2
  ```,
)
