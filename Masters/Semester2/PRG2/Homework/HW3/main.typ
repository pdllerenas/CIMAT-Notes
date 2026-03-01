#import "@preview/problemst:0.1.2": pset
#import "@preview/arborly:0.3.2": tree

#show: pset.with(
  class: "Programación y Estructuras de Datos II",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 2",
  date: datetime(year: 2026, month: 3, day: 5 ),
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
  === 1. Quadtree: capacidad óptima por límites superior e inferior (10 pts)
  #{
    set par(spacing: 1pt)
    line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  }
  Considera un _point-region quadtree_ sobre el rectangulo
  $
    Sigma = [x_min, x_max] times [y_min, y_max], quad quad x_min < x_max, quad y_min < y_max.
  $
  Cada hoja almacena hasta $C$ puntos. Si al insertar un punto una hoja excede $C$, el nodo se subdivide en cuatro hijos y sus puntos se redistribuyen.

  *Datos de prueba (generados por ti).* Construye el quadtree con capacidad C y define:
  - $S(C)$: número total de subdivisiones realizadas (nodos internos creados por overflow).
  - $D(C)$: profundidad máxima del árbol (raíz en profundidad 0).

  *Capacidades extremas.* Con las métricas anteriores, define:
  - *Capacidad máxima* $C_max$: el mayor entero $C>= 1$ tal que $S(C) >= 4$.
  - *Capacidad mínima* $C_min$: el menor entero $C>=1$ tal que $D(C) <= n -1$.

  La interpretation buscada es:
  - si $C$ es demasiado grande, el árbol casi no subdivide (mala resolution espacial);
  - si $C$ es demasiado pequeño, puede haver sobre-subdivisión (profundidades excesivas).

  *Tarea.* Calcula $C_min$ y $C_max$ para la instancia generada. Tu método debe ser eficiente para $n$ grande: se espera razonamiento algorítmico (por ejemplo, monotonicidad + búsqueda binaria, o una estrategia equivalente bien justificada).
]