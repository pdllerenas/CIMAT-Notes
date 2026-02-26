#import "@preview/problemst:0.1.2": pset
#import "@preview/arborly:0.3.2": tree

#show: pset.with(
  class: "Programación y Estructuras de Datos II",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 1",
  date: datetime(year: 2026, month: 2, day: 18),
)
#set text(lang: "es")

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
    _después_ de la posición `pos` de la secuencia actual (si `pos = 0`, se inserta
    al inicio),
  3. `reverse(l,r)`: invierte el bloque $S[l..r]$,
  4. `cyclicShift(l,r,k)`: aplica un corrimiento cíclico al bloque $S[l..r]$ en
    $k$ posiciones a la derecha (con $k$ posiblemente mayor que el tamaño del
    bloque)

  En este problema no se pide código. Se pide:
  - Proponer una estructura de datos que soporte estas operaciones eficientemente
  - Explicar qué información agregada se debe mantener y cómo se actualiza
  - Dar la complejidad (tiempo y memoria) de cada operación
  - Ilustrar con un ejemplo pequeño ($n = 10$) una secuencia de al menos 8
    operaciones, mostrando el estado final de $S$
]
Proponemos usar un _treap implicito_, que consiste en un árbol binario de búsqueda (BST) que satisface la propiedad de heap.

=== Estructura
Un treap implícito acompañado de un stack de punteros a otros treaps. Cada nodo
contiene la siguiente información:
- valor (valor de la lista inicial),
- prioridad (aleatoria para tener un árbol balanceado en esperanza),
- nodo izquierdo y derecho,
- size (tamaño de sub-árbol)
- reversed (para saber cuando un segmento ha sido volteado),

Este tipo de treap tiene las operaciones `join` y `split`, que hacen uso del `size` como la posición implícitamente. Estas operaciones unen dos treaps implícitos, y lo parten dada una posición, respectivamente.
El stack de punteros mantendrá los arboles cortados por `cut`. De este mismo stack se
tomaran cuando se use `paste`.

=== Actualizaciones
- `cut`: se recorta el rango $(l,r)$ del treap mediante dos operaciones de `split`, que nos separa el treap en 3 secciones, lo que guardaremos en el stack, y otros 2 treaps que uniremos con `join`. Estos treaps ahora tienen
- `paste`: toma el primer treap del stack y usa `join` con el treap principal, con la posición dada. Quitamos el treap recientemente usado del stack.
- `reverse`: hacemos los `split` en los nodos frontera. El nodo base del rango a invertir tendra la bandera _reversed_ activada. Esto permitirá saber que el sub-árbol ha sido invertido, y que el nodo izquierdo corresponderá al nodo derecho y viceversa.
- `cyclicShift`: tomamos el bloque indicado mediante `split` y realizamos un recorrimiento dentro del mismo de manera cíclica. Luego, hacemos `join` para volver a unir el bloque con el treap principal.

=== Complejidad de operaciones
- Dado que el treap es balanceado (por las prioridades aleatorias), las operaciones base de la estructura `split` y `merge` son $O(h)$, donde $h$ es la altura del árbol.
- Por la definición de lo que hace `cut`, tenemos 3 llamadas $O(h) = O(log n)$, por lo que `cut` es $O(log n)$.
- Por la definición de lo que hace `paste`, solo usamos 1 `join`, por lo que esta operación es $O(log n)$.
- La operación `reverse` usa 2 split y 1 join. La actualización de la bandera solo se hace en el nodo padre, por lo que solo es $O(log n)$.
- La operación `cyclicShift` hace uso de 1 `merge` y 2 `split`, por lo que en total es $O(log n)$.

=== Ejemplo
Supongamos que $S = [1,2,3,4,5,6,7,8,9,10]$ y que el treap implícito queda de la siguiente manera:
#figure(
  tree(
    vertical-gap: 1.2cm,
    horizontal-gap: 1cm,
  )[
    #table(
      columns: 2,
      [1.0], [6],
    )
    [#table(
      columns: 2,
      [0.9], [3],
    ) [#table(
      columns: 2,
      [0.7], [2],
    ) [#table(
      columns: 2,
      [0.5], [1],
    )][]][#table(
      columns: 2,
      [0.4], [5],
    ) [#table(
      columns: 2,
      [0.2], [4],
    )] []] ] [#table(
      columns: 2,
      [0.8], [9],
    )
    [#table(
      columns: 2,
      [0.6], [8],
    ) [#table(
      columns: 2,
      [0.3], [7],
    )][]] [#table(
      columns: 2,
      [0.5], [10],
    )]
    ]
  ],
)
1. `cut(3,5)`, separamos los primeros 5 elementos, luego hacemos split a los primeros 2 de este rango. El árbol resultante es $S = [1,2,6,7,8,9,10]$ y el stack queda con el bloque $[3,4,5]$. Visualmente:
#grid(
  columns: (1fr, 1fr),
  align: center,
  figure(
    table(
      rows: 3,
      columns: 1,
      [3],
      [4],
      [5],
    ),
    caption: "Stack",
  ),
  figure(
    tree(
      vertical-gap: 1.2cm,
      horizontal-gap: 1cm,
    )[
      #table(
        columns: 2,
        [1.0], [6],
      )
      [#table(
        columns: 2,
        [0.7], [2],
      ) [#table(
        columns: 2,
        [0.5], [1],
      ) ][] ] [#table(
        columns: 2,
        [0.8], [9],
      )
      [#table(
        columns: 2,
        [0.6], [8],
      ) [#table(
        columns: 2,
        [0.3], [7],
      )][]] [#table(
        columns: 2,
        [0.5], [10],
      )]
      ]
    ],
    caption: "Treap implícito después de cut(3,5)",
  ),
)

2. `reverse(2,4)`, nos da $S = [1,7,6,2,8,9,10]$.
3. `cyclicShift(3,6,2)` tiene como objetivo el bloque $[6,2,8,9]$, por lo que se convierte en $[8,9,6,2]$ nos da $S = [1,7,8,9,6,2,10]$.
4. `paste(4)`, insertamos el bloque del stack en la posición dada, por lo que $S = [1,7,8,9,3,4,5,6,2,10]$.
5. `reverse(3,7)`, nos da $S = [1,7,5,4,3,9,8,6,2,10]$.
6. `cut(1,3)`, nos da $S = [4,3,9,8,6,2,10]$ y el stack queda con $[1,7,5]$.
7. `paste(6)`, nos da $S = [4,3,9,8,6,2,1,7,5,10]$.
8. `cyclicShift(1,10,3)`, nos da $S = [7,5,10,4,3,9,8,6,2,1]$.


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

Proponemos usar un _treap_, que consiste en un árbol binario de búsqueda (BST) que satisface la propiedad de heap. Las prioridades son generadas aleatoriamente, lo que garantiza que el árbol se mantenga balanceado en promedio, con una altura esperada de $O(log n)$. Las prioridades independientes identicamente distribuidas convierten el problema a uno de un árbol binario de búsqueda aleatorio, que tiene una altura esperada de $O(log n)$ (Referencia: https://en.wikipedia.org/wiki/Random_binary_tree).

=== Estructura
Un treap regular, donde cada nodo contiene la siguiente información:
- clave,
- peso (tamaño de sub-árbol),
- suma del sub-árbol,
- prioridad (aleatoria para tener un árbol balanceado en esperanza),
- nodo izquierdo y derecho,

=== Actualizaciones
- `add`: inserción usual en un treap, con la particularidad de que si la clave ya existe, solo se actualiza el peso y la suma del nodo.
- `remove`: similar a `add`, pero en lugar de insertar o actualizar, se disminuye el peso y la suma del nodo. Si el peso llega a 0, se elimina el nodo del treap.
- `rangeWeight`: se hace un `split` para separar el treap en tres partes: claves menores que $l$, claves entre $l$ y $r$, y claves mayores que $r$. La suma del sub-árbol del nodo raíz de la parte intermedia nos da el resultado. Luego se hace un `merge` para recombinar las partes.
- `weightedMedian`: se hace una búsqueda en el treap, comparando la suma del sub-árbol izquierdo con la mitad del peso total para decidir si se va a la izquierda, derecha o se queda en el nodo actual.
- `split`: búsqueda de un nodo dado su valor de clave, y se separa el treap en dos partes: claves menores o iguales a la clave dada, y claves mayores.
- `merge`: dados dos treaps $A$ y $B$, se hacen comparaciones de prioridades para decidir cómo unirlos, manteniendo la propiedad de BST y heap. Asumimos que las claves de $A$ son menores que las de $B$.

=== Complejidad de operaciones
- `add`: $O(log n)$ como una inserción usual en un treap.
- `remove`: $O(log n)$ como una eliminación usual en un treap.
- `rangeWeight`: $O(log n)$ por los splits y merge necesarios.
- `weightedMedian`: $O(log n)$ por la forma de búsqueda (escoger entre izquierda y derecha) en el treap.
- `split`: $O(log n)$, búsqueda de un nodo dado su valor de clave.
- `merge`: $O(log n)$, comparación de prioridades a lo más la suma de las alturas de los treaps, que son $O(log n)$ en esperanza.

=== Ejemplo
Supongamos que tenemos las siguientes claves y pesos:
#figure(
  table(
    columns: 2,
    align: center,
    table.header([*Clave*], [*Peso*]),
    [2], [3],
    [5], [1],
    [7], [4],
    [10], [2],
    [13], [6],
    [18], [2],
    [21], [5],
  ),
  caption: "Claves y pesos",
)
El peso total es $3 + 1 + 4 + 2 + 6 + 2 + 5 = #{3 + 1 + 4 + 2 + 6 + 2 + 5}$. La mitad del peso total es $11.5$. Para encontrar la mediana ponderada, veamos el peso acumulado de las claves ordenadas:
#figure(
  table(
    columns: 3,
    align: center,
    table.header([*Clave*], [*Peso*], [*Peso acumulado*]),
    [2], [3], [3],
    [5], [1], [4],
    [7], [4], [8],
    [10], [2], [10],
    [13], [6], [16],
    [18], [2], [18],
    [21], [5], [23],
  ),
  caption: "Claves y pesos",
)
La menor clave $m$ tal que el peso acumulado de todas las claves $<= m$ es al menos $11.5$ es $13$, ya que el peso acumulado hasta la clave $10$ es $10$, y el peso acumulado hasta la clave $13$ es $16$, que es mayor que $11.5$. Por lo tanto, `weightedMedian()` devuelve $13$.

== Parte 2: Programación en C++

#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 3. Consultas por rango con `add` y `cap` (3 puntos)
  #line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  Dado un arreglo $A$ de longitud $n$ (valores enteros), implementa una estructura que soporte las operaciones
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
Este problema parece resolverse con _segment tree beats_ (https://codeforces.com/blog/entry/57319) o segment tree con lazy propagation, pero no ninguno de los dos fue visto en clase (ni mencionada). En su lugar, propongo usar un _Fenwick tree_ que realice las operaciones de rango de manera ineficiente, actualizando cada posición del rango individualmente. Esto es $O(n)$ por operación, pero dado que el número de operaciones es "razonable", el programa debería correr en un tiempo aceptable.

- Con la implementación de _segment tree beats_, debemos guardar 2 mínimos por segmento, el mínimo y el segundo mínimo. Esto nos permite saber si el `cap` afecta a todo el segmento o solo a una parte de él. Si el `cap` es mayor o igual al mínimo del segmento, entonces no se necesita hacer nada. Si el `cap` es menor que el mínimo pero mayor o igual al segundo mínimo, entonces solo se necesita actualizar el mínimo del segmento. Si el `cap` es menor que el segundo mínimo, entonces se necesita hacer una propagación profunda para actualizar todos los elementos del segmento. Con nuestra implementación ingenua, cada `cap` podría afectar a todos los elementos del rango, lo que resultaría en una complejidad de $O(n)$ por operación.
- Por el comentario anterior, solo se hace propagación profunda cuando el `cap` es menor que el segundo mínimo del segmento. Entonces, las veces que hacemos esa operación costosa, se compensa con las veces que no.
- Si actualizamos todo el rango, la implementación naive realiza $O(n)$ operaciones, mientras que con segment tree beats, solo se haría una actualización del mínimo del segmento, lo que es $O(log n)$ amortizado.

=== Ejemplo
```
10 15
add 1 5 10
add 3 7 5
sum 1
sum 3
cap 3 3 12
sum 3
cap 1 5 11
sum 4
cap 3 3 9
sum 3
add 2 4 3
sum 2
cap 1 10 8
sum 4
sum 10
```

Tiene como output esperado:
```
10
15
12
11
9
13
8
0
```

#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
=== 4. Consultas aritméticas por rango con "horizonte" (4 puntos)
  #line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  Dado un arreglo estático $A$ de longitud $n$ (enteros positivos), implementa un programa que procese $q$ consultas de los siguientes tipos (los indices son 1-based):
  - Tipo 1: `1 L R`
    Imprime $gcd(A[L], A[L+1], ..., A[R])$.
  - Tipo 2: `2 L G`
    Encuentra el máximo indice $R$ (con $L <= R <= n$) tal que
    $
      gcd(A[L], A[L+1], ..., A[R]) equiv 0 (mod G)
    $
    Imprime ese $R$. Si no existe ningún $R$ que cumpla la condición, imprime $-1$.

  *Entrada*
  - La primera línea contiene un entero $n$ ($1 <= n <= 2 dot 10^5$), el tamaño del arreglo.
  - La segunda línea contiene $n$ enteros $A[i]$ ($1 <= A[i] <= 10^9$), los elementos del arreglo.
  - La tercera línea contiene un entero $q$ ($1 <= q <= 2 dot 10^5$), el número de consultas.
  - Las siguientes $q$ líneas contienen las consultas, cada una con el formato descrito anteriormente.
]

Para este problema, dado que el arreglo es estático, usaremos una _sparse table_ para responder las consultas de tipo 1 en tiempo constante. Para las consultas de tipo 2, haremos una búsqueda binaria sobre el rango $[L, n]$ y usaremos la sparse table para calcular el gcd del rango $[L, R]$ en cada paso de la búsqueda binaria. Esto nos permitirá encontrar el máximo índice $R$ que cumple la condición dada en tiempo logarítmico.

La manera ingenua de realizar la consulta tipo 2 seria calcular el $gcd$ de cada rango $[L, R]$ para $R$ desde $L$ hasta $n$, lo que resultaría en una complejidad de $O(n)$ por consulta.

El algoritmo regresa correctamente el resultado de la consulta tipo 1 gracias a la construcción de la sparse table, que permite obtener el $gcd$ de cualquier rango en tiempo constante. Para la consulta tipo 2, la búsqueda binaria garantiza que encontraremos el máximo índice $R$ que cumple la condición dada, y el uso de la sparse table para calcular el $gcd$ en cada paso de la búsqueda binaria asegura que cada consulta se resuelva en tiempo constante, y por lo tanto de la consulta es logarítmica.

Como entrada, podemos usar el siguiente ejemplo:
```
7
12 18 6 9 3 15 10
6
1 0 2
1 1 4
2 0 6
2 1 3
2 5 5
2 6 2
```

Tiene como output esperado:
```
6
3
3
6
7
7
```
Esto es porque:
- Para la consulta `1 0 2`, el $gcd$ de $[12, 18, 6]$ es $6$.
- Para la consulta `1 1 4`, el $gcd$ de $[18, 6, 9, 3]$ es $3$.
- Para la consulta `2 0 6`, el máximo índice $R$ tal que $gcd(A[0], A[1], ..., A[R])$ es múltiplo de $6$ es $3$, ya que $gcd(12, 18, 6, 9) = 3$ no es múltiplo de $6$, pero $gcd(12, 18, 6) = 6$ sí lo es.
- Para la consulta `2 1 3`, el máximo índice $R$ tal que $gcd(A[1], A[2], ..., A[R])$ es múltiplo de $3$ es $6$, ya que $gcd(18, 6, 9, 3, 15) = 3$ es múltiplo de $3$, pero $gcd(18, 6, 9, 3, 15, 10) = 1$ no lo es.
- Para la consulta `2 5 5`, el máximo índice $R$ tal que $gcd(A[5], A[6])$ es múltiplo de $5$ es $7$, ya que $gcd(15, 10) = 5$.
- Para la consulta `2 6 2`, el máximo índice $R$ tal que $gcd(A[6])$ es múltiplo de $2$ es $7$, ya que $gcd(10) = 10$ es múltiplo de $2$.
