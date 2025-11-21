#import "@preview/upb-cn-templates:0.2.0": code, upb-cn-report
#import "@preview/timeliney:0.2.0"

#show: upb-cn-report.with(
  title: "Propuesta de proyecto: Conecta 4",
  author: "Pedro D. Llerenas",
  left-header: "Programacion y Algoritmos I",
)

= Introducción <sec:introduction>

Conecta 4 @enwiki:1317722271 es un juego en el cual los jugadores eligen un color y se toman
turnos soltando fichas del color elegido en un tablero vertical con 6 filas y 7
columnas. Las piezas caen una sobre otra en forma de pila, ocupando el lugar
mas hondo. El primer jugador en juntar 4 fichas consecutivas en diagonal,
horizontal o vertical gana.

Conecta 4 es un _juego resuelto_ @enwiki:1310028810, esto quiere decir que desde cualquier
posición se puede predecir el ganador (o empate), si ambos jugadores juegan
perfectamente. Más aún, si el primer jugador juega perfectamente, tiene
garantizado ganar @Allen_ConnectFour_Web.

= Objetivos <sec:objectives>

El objetivo principal es crear un _bot_ que logre evaluar una posición dada y,
mediante el algoritmo _mini-max_ @enwiki:1320676492, que consiste en minimizar la puntuación del
oponente, maximizando la propia, determine la columna que maximiza su
probabilidad de ganar. Podremos interactuar con el _bot_ mediante la terminal,
donde se mostrará el tablero.

= Herramientas <sec:tools>

- Orientación a objetos: se utilizarán clases para representar el estado del
  tablero, y los métodos de dicha clase actuarán sobre ese estado.

- Estructuras de datos y algoritmos: para diseñar el algoritmo _mini-max_, se
  utilizaran arboles de búsqueda y una pila para determinar el mejor
  movimiento, evaluando varios movimientos al futuro. También se puede utilizar la
  técnica de _alpha-beta pruning_ @enwiki:1314951571 para reducir el espacio de búsqueda.

- Manipulación de bits: dado que el tablero es de $6 times 7$, se presta a usar
  un entero sin signo de 64 bits `uint64_t` (aunque se desperdicien algunos)
  para representar el estado del tablero. Con esto, los movimientos se reducen
  a manipular los bits de `uint64_t`.

= Rúbrica de evaluación <sec:evaluation>

- Se generan correctamente los movimientos legales en cada posición.
- El _bot_ evalúa correctamente la posición y escoge el mejor movimiento (con una
  profundidad definida).
- La eficiencia de la implementación de los algoritmos de búsqueda.
- Visualización correcta del tablero en la terminal.

= Línea de tiempo <sec:timeline>

El siguiente calendario muestra los tiempos estimados para el desarrollo del
proyecto.

#figure(placement: none, numbering: none)[
  #timeliney.timeline(
    show-grid: true,
    grid-style: (
      stroke: (thickness: 0.5pt, paint: gray, dash: "dotted"),
    ),
    {
      import timeliney: *
      headerline(..range(2).map(n => ([Semana #(n + 1)], 8)))
      task("Tablero y estados terminales", (0, 2))
      task("Movimientos", (1, 4))
      task("Minimax", (4, 11))
      task("Alpha-Beta pruning", (9, 14))
      task("Pruebas", (14, 16))
    },
  )
]

#bibliography("refs.bib")
