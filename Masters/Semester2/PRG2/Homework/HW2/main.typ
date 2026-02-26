#import "@preview/problemst:0.1.2": pset
#import "@preview/arborly:0.3.2": tree

#show: pset.with(
  class: "Programación y Estructuras de Datos II",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 2",
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

#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 1. Li Chao Tree
  #{
    set par(spacing: 1pt)
    line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  }
  === A) Extienda el código
  Extienda el código del Li-Chao Tree del enlace para incluir una inicialización que permita usarlo de manera más cómoda, justificando porque considera que su implementación puede ser considerada como más cómoda.
  La extensión debe incluir como mínimo:
  1. Una función (o constructor) para inicializar el rango de consulta $x in [x_L, x_R]$.
  2. Manejo seguro del estado inicial (antes de insertar líneas): definir qué devuelve query si no hay líneas, o usar una línea sentinela.
  3. Una implementación limpia para insertar y consultar:
    - `addLine(m, b)` inserta $y = m x + b$.
    - `query(x)` devuelve el mínimo (o máximo, según tu elección) en $x$.

  === B) Informe con 4 líneas en [1, 4]
  Presente un informe en que considere un caso con 4 líneas que se van a evaluar en el rango $[1, 4]$ y dibuje cómo irá quedando su Li Chao Tree después de cada inserción.
  El informe debe:
  1. Especificar las 5 líneas elegidas en forma $y = m x + b$ según su elección.
  2. Mostrar la subdivisión del dominio $[1, 4]$ usada por el árbol (por ejemplo $[1, 4] -> [1, 2]$ y $[3, 4]$,etc.).
  3. Tras cada inserción, dibuje el árbol indicando qué línea queda en cada nodo relevante.
  4. Explicar cada iteración del algoritmo de inserción:
    - punto medio donde se comparan líneas,
    - cuál línea “gana” en el medio,
    - a qué subintervalo recurre y por qué,
    - hasta finalizar el ciclo.
]

=== A)
La implementación en `LiChao.h` es más cómoda en comparación con la vinculada porque ahora está encapsulada en una clase. Los métodos se llaman directamente desde el objeto sin necesidad de pasar el objeto como argumento, y además las funciones `addLine` y `query` abstraen los otros argumentos que no le interesan al cliente. La inicialización con un rango fijo permite tener control sobre los valores en los cuales realizaremos las busquedas.

=== B)
1.
Elegimos las siguientes líneas:
$
  L_1: & y = 2x+1 \
  L_2: & y = -x+8 \
  L_3: & y = x+2 \
  L_4: & y = 3x-1 \
  L_5: & y = -2x+10 \
$
2.
El dominio $[1,4]$ se divide idénticamente a un Segment Tree. Es decir:
#figure(
  tree[$[1,4]$
    [$[1,2]$ [$[1,1]$] [$[2,2]$]]
    [$[3,4]$ [$[3,3]$] [$[4,4]$]]
  ],
  caption: [División inicial del árbol.],
)
3. En cada iteración, insertamos una línea nueva, haciendo la comparación de sus evaluaciones en los puntos medios de cada intervalo.

#figure(
  tree[$[1,4]:L_1$
    [$[1,2]$ [$[1,1]$] [$[2,2]$]]
    [$[3,4]$ [$[3,3]$] [$[4,4]$]]
  ],
  caption: [Primer iteración, se inserta $L_1$ a la raíz, ya que es la única línea.],
)

#figure(
  tree[$[1,4]:L_1$
    [$[1,2]$ [$[1,1]$] [$[2,2]$]]
    [$[3,4]:L_2$ [$[3,3]$] [$[4,4]$]]
  ],
  caption: [Segunda iteración. Realizamos la comparación $L_1$ y $L_2$ en el
  punto medio: $x=2$. $L_1(x=2) = 5 < 6 = L_2(x=2)$, por lo que gana $L_1$ por
  ser menor. En el extremo izquierdo, $L_1(x=1) = 3 < 7 = L_2(x=1)$ gana $L_1$,
  por lo que $L_2$ solamente es menor del lado derecho.
 ],
)

#figure(
  tree[$[1,4]:L_3$
    [$[1,2]: L_1$ [$[1,1]$] [$[2,2]$]]
    [$[3,4]:L_2$ [$[3,3]$] [$[4,4]$]]
  ],
  caption: [Tercera iteración. Realizamos la comparación $L_1$ y $L_3$ en el
  punto medio: $x=2$. $L_1(x=2) = 5 > 4 = L_3(x=2)$, por lo que gana $L_3$ por
  ser menor. En el extremo izquierdo, $L_1(x=1) = 3 = 3 = L_3(x=1)$ empatan, entonces $L_3$ queda en la
  raíz y movemos $L_1$ a la izquierda (o derecha, según la implementación que se
  use en el desempate; yo usé $<=$ para mandar a la izquierda).
 ],
)

#figure(
  tree[$[1,4]:L_3$
    [$[1,2]: L_4$ [$[1,1]$] [$[2,2]:L_1$]]
    [$[3,4]:L_2$ [$[3,3]$] [$[4,4]$]]
  ],
  caption: [Cuarta iteración. Realizamos la comparación $L_3$ y $L_4$ en el
  punto medio: $x=2$. $L_3(x=2) = 4 < 5 = L_4(x=2)$, por lo que gana $L_3$ por
  ser menor. En el extremo izquierdo, $L_3(x=1) = 3 > 2 = L_4(x=1)$ gana $L_4$,
  por lo que mandamos a $L_4$ al hijo izquierdo. Ahora, se compara $L_1$ y $L_4$
  en $x = 1$, donde gana $L_4$. En $x=2$ empatan, por lo que mandamos a $L_1$
  al hijo derecho.
  ],
)

#figure(
  tree[$[1,4]:L_3$
    [$[1,2]: L_4$ [$[1,1]:emptyset$] [$[2,2]:L_1$]]
    [$[3,4]:L_5$ [$[3,3]:emptyset$] [$[4,4]:L_2$]]
  ],
  caption: [Quinta iteración. Realizamos la comparación $L_3$ y $L_5$ en el
  punto medio: $x=2$. $L_3(x=2) = 4 < 6 = L_5(x=2)$, por lo que gana $L_3$ por
  ser menor. En el extremo izquierdo, $L_3(x=1) = 3 < 7 = L_5(x=1)$ gana $L_3$,
  por lo que mandamos a $L_5$ al hijo derecho. Ahora, se compara $L_2$ y $L_5$
  en $x = 3$, donde gana $L_5$. En $x=4$ gana $L_5$, por lo que mandamos a $L_2$
  al hijo derecho.
  ],
)

#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 2. `square-ends` con Li Chao Tree
  #{
    set par(spacing: 1pt)
    line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  }
  Resuelve el problema
  #align(center, link("https://csacademy.com/contest/archive/task/squared-ends/"))
]

#figure(
  image("image.png")
)