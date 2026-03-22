#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 3",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
    Realizamos un análisis del algoritmo Edmonds-Karp sobre redes de flujo,
    mostramos una implementación del mismo y un problema de cubrimiento mínimo de
    caminos en un Directed Acyclic Graphs (DAG). Además, un análisis comparativo entre el
    algoritmo Dinic y el algoritmo Hopcroft-Karp.
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Introducción

#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `s`, name: <s>),
    edge(<s>, "-|>", <a>, label-size: 8pt, [8]),
    edge(<s>, "-|>", <b>, label-size: 8pt, [7]),
    node((1, -1), `a`, name: <a>),
    edge(<a>, "-|>", <b>, label-size: 8pt, [3]),
    edge(<a>, "-|>", <c>, label-size: 8pt, [6]),
    edge(<a>, "-|>", <d>, label-size: 8pt, [3]),
    node((1, 1), `b`, label: "b", name: <b>),
    edge(<b>, "-|>", <c>, label-size: 8pt, [2]),
    edge(<b>, "-|>", <d>, label-size: 8pt, [6]),
    node((2, -1), `c`, label: "c", name: <c>),
    edge(<c>, "-|>", <d>, label-size: 8pt, [1]),
    edge(<c>, "-|>", <t>, label-size: 8pt, [7]),
    node((2, 1), `d`, label: "d", name: <d>),
    edge(<d>, "-|>", <t>, label-size: 8pt, [8]),
    node((3, 0), `t`, label: "t", name: <t>),
  ),
  caption: [Diagrama de flujos inicial.],
)

#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `s`, name: <s>),
    edge(<s>, "-|>", <a>, stroke: red, label-size: 8pt, [8]),
    edge(<s>, "-|>", <b>, label-size: 8pt, [7]),
    node((1, -1), `a`, name: <a>),
    edge(<a>, "-|>", <b>, label-size: 8pt, [3]),
    edge(<a>, "-|>", <c>, label-size: 8pt, [6]),
    edge(<a>, "-|>", <d>, label-size: 8pt, [3]),
    node((1, 1), `b`, label: "b", name: <b>),
    edge(<b>, "-|>", <c>, label-size: 8pt, [2]),
    edge(<b>, "-|>", <d>, label-size: 8pt, [6]),
    node((2, -1), `c`, label: "c", name: <c>),
    edge(<c>, "-|>", <d>, label-size: 8pt, [1]),
    edge(<c>, "-|>", <t>, label-size: 8pt, [7]),
    node((2, 1), `d`, label: "d", name: <d>),
    edge(<d>, "-|>", <t>, label-size: 8pt, [8]),
    node((3, 0), `t`, label: "t", name: <t>),
  ),
  caption: [Diagrama de flujos inicial.],
)
