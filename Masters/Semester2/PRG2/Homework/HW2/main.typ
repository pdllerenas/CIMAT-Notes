#import "@preview/problemst:0.1.2": pset
#import "@preview/arborly:0.3.2": tree

#show: pset.with(
  class: "Programación y Estructuras de Datos II",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 2",
  date: datetime(year: 2026, month: 2, day: 18),
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

#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 2. `square-ends` con Li Chao Tree
  #{
    set par(spacing: 1pt)
    line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  }
  Resuelve el problema
  #align(center, link("https://csacademy.com/contest/archive/task/squared-ends/"))
]
