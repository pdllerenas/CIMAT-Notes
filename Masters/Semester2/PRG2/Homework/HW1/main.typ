#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Programación y Estructuras de Datos II",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 1",
  date: datetime(year: 2026, month: 2, day: 18),
)
#show: great-theorems-init
#let proof = proofblock()
#let mathcounter = rich-counter(identifier: "mathblocks", inherited_levels: 1)
#let theorem = mathblock(
  blocktitle: "Theorem",
  counter: mathcounter,
)

#show link: set text(fill: blue)
#show link: underline

#set math.equation(
  numbering: "(1)",
  supplement: none,
)

#set par(
  justify: true,
  leading: 0.52em,
)

#show ref: it => {
  // provide custom reference for equations
  if it.element != none and it.element.func() == math.equation {
    // optional: wrap inside link, so whole label is linked
    link(it.target)[(#it)]
  } else {
    it
  }
}

#set enum(
  full: true, // necessary to receive all numbers at once, so we can know which level we are at
  numbering: (..nums) => {
    let nums = nums.pos() // just positional args
    let num = nums.last() // just the current level’s number
    let level = nums.len() // level is the amount of numbers available

    // format for current level (or stop at i. If going too deep)
    let format = ("a)", "i)", "i)").at(calc.min(2, level - 1))
    let result = numbering(format, num) // formatted number
    if level < 3 {
      // first 2 levels for example
      strong(result)
    } else {
      result
    }
  },
)

== Parte 1: Resolución escrita
#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 1. Secuencia editable con operaciones avanzadas *(1.5 puntos)*
  #{
  set par(spacing: 1pt)
  line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  }
  Se mantiene una secuencia $S$ de longitud $n$ (inicialmente $S = [1,2,...,n]$).
  Se deben soportar las operaciones siguientes:
  1. `cut(l,r)`: extrae el bloque contiguo $S[l..r]$ y lo coloca en una pila
    (LIFO) de bloques,
  2. `paste(pos)`: toma el bloque del tope de la pila y lo inserta inmediatamente
    _después_ de la posición `pos` de la scuencia actual (si `pos = 0`, se inserta
    al inicio),
  3. `reverse(l,r)`: invierte el bloque $S[l..r]$,
  4. `cyclicShift(l,r,k)`: aplica un corrimiento cíclico al bloque $S[l..r]$ en
    $k$ posiciones a la derecha (con $k$ posibemente mayor que el tamaño del
    bloque)

  En este problema no se pide código. Se pide:
  - Proponer una estructura de datos que soporte estas operaciones eficientemente
  - Explicar qué información agregada se debe mantener y cómo se actualiza
  - Dar la complejidad (tiempo y memoria) de cada operación
  - Ilustrar con un ejemplo pequeño ($n = 10$) una secuencia de al menos 8
    operaciones, mostrando el estado final de $S$
]



#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 2. Colección ordenada con partir/mezclar y medianas ponderadas (1.5puntos)
  #line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  Se mantiene una colección de pares $(x, w)$ donde $x$ es una clave entera y
  $w$ un peso positivo. Pueden existir múltiples elementos con la misma $x$
  (los pesos se _acumulan_ por clave).

  Operaciones:
  1. `add(x,w)` y `remove(x,w)` (garantiza que nunca se elimina más peso del
    que existe)
  2. `rangeWeight(l,r)`: devuelve $sum w$ de todas las claves $x in [l,r]$,
  3. `weightedMedian()`: devuelve la menor clave $m$ tal que el peso acumulado
    de todas las claves $<= m$ es al menos la mitad del peso total,
  4. `split(x)`: produce dos estructuras $A$ (claves $<= x)$ y $B$ (claves $>
    x$),
  5. `merge(A,B)`: dado que toda clave en $A$ es menor que cualquier clave en
    $B$, produce una estructura única

  Se pide:
  - Diseñar una estructura que soporte las operaciones anteriores en tiempo
    esperado logarítmico,
  - Justificar brevemente por qué la altura esperada se mantiene pequeña,
  - Mostrar (a mano) el resultado de weightedMedian() en un ejemplo con al
    menos 7 claves distintas y pesos variados,
]

== Parte 2: Programación en C++

=== Colección ordenada con partir/mezclar y medianas ponderadas (1.5 puntos)
#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 3. Consultas por rango con `add` y `cap` (3 puntos)
  #line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  Dado un arreglo $A$ de longitud $n$ (valores enteros), implementa una estrucura que soporte las operaciones
  1. `add(l,r,v)`: para todo $i in [l,r]$, $A[i] <- A[i] + v$,
  2. `cap(l,r,x)`: para todo $i in [l,r]$, $A[i] <- min(A[i], x)$,
  3. `sum(l,r)`: devuelve $sum_(i=l)^r A[i]$.

  Requisitos:
  - Para sus pruebas considere una entrada y un número de consultas razonables,
  - Debe pasar casos adversarios razonables (mezcla de muchas caps y adds),
  - Entrega: `nombre_apellido_tarea_01_problema_03.cpp`.

  En su reporte discuta lo siguiente: La operación `cap(l,r,x)` puede forzar
  cambios en muchos elementos. Aun así, existe un método para evitar bajar a
  hojas en la mayoría de los casos.
  - Describe qué información adicional debes mantener por segmento para
    detectar cuándo `cap` modifica sólo un subconjunto “controlable” de
    posiciones
  - Da un argumento amortizado (intuitivo pero preciso) de por qué el número
    total de veces que un segmento requiere propagación profunda es acotado
  - Menciona un caso donde una implementación naive caería a $Theta(n)$ por operación
]

