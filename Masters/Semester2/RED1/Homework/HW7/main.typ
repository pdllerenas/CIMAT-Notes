#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Programación de Redes 1",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 7",
  date: datetime(year: 2026, month: 3, day: 4),
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

#question[
  Comparar la eficiencia de TCP y UDP con grandes transmisiones de datos en tres diferentes tipos de ambientes: a) Red Local (especificar el IP de su computadora u otra computadora dentro del CIMAT), b) Nodo Local (127.0.0.1) y c) Red externa (casa o ciber-bar). 
]

#grid(
  columns: (1fr, 1fr),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([Servidor], [Ejecución (s)], [Mb/s]),
      table.hline(),
      [Localhost], [1.10], [10432.6],
      [Red Local], [224.79], [51.24],
      [Red Externa], [86.41], [133.30],
      table.hline(),
    ),
    caption: [TCP default buffer size.]
  ),
  figure(
    table(
      stroke: none,
      columns: 5,
      table.hline(),
      table.header([Servidor], [Ejecución (s)], [Mb/s], [PP], [PP %]),
      table.hline(),
      [Localhost], [3.08], [3738.79], [0], [0.00%],
      [Red Local], [8.45], [1351.89], [7240], [0.72%],
      [Red Externa], [9.66], [1191.58], [18], [0.00%],
      table.hline(),
    ),
    caption: [UDP default buffer size.]
  ),
)

#grid(
  columns: (1fr, 1fr),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([Servidor], [Ejecución (s)], [Mb/s]),
      table.hline(),
      [Localhost], [1.33], [8601.39],
      [Red Local], [15.99], [719],
      [Red Externa], [...], [...],
      table.hline(),
    ),
    caption: [TCP buffer size 32.]
  ),
  figure(
    table(
      stroke: none,
      columns: 5,
      table.hline(),
      table.header([Servidor], [Ejecución (s)], [Mb/s], [PP], [PP %]),
      table.hline(),
      [Localhost], [2.80], [4110.00], [0], [0.00%],
      [Red Local], [15.99], [719.81], [547], [0.05%],
      [Red Externa], [12.79], [900.45], [0], [0.00%],
      table.hline(),
    ),
    caption: [UDP buffer size 32.]
  ),
)

#grid(
  columns: (1fr, 1fr),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([Servidor], [Ejecución (s)], [Mb/s]),
      table.hline(),
      [Localhost], [1.32], [8695.08],
      [Red Local], [608.33], [18.93],
      [Red Externa], [167.77], [68.66],
      table.hline(),
    ),
    caption: [TCP buffer size 5000.]
  ),
  figure(
    table(
      stroke: none,
      columns: 5,
      table.hline(),
      table.header([Servidor], [Ejecución (s)], [Mb/s], [PP], [PP %]),
      table.hline(),
      [Localhost], [2.19], [5238.16], [0], [0.00%],
      [Red Local], [14.28], [805.88], [758], [0.08%],
      [Red Externa], [16.21], [710.54], [2], [0.00%],
      table.hline(),
    ),
    caption: [UDP buffer size 5000.]
  ),
)