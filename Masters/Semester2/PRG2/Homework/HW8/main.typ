#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/cetz:0.5.0": canvas, draw, tree
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 8",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Problema 1
Bas´andose en las ideas desarrolladas en la clase, implementar un algoritmo determinando la distancia
de Damerau-Levenshtein entre dos cadenas ADN (conformadas por los cuatro caracteres A, C, T, G).
La distancia de Damerau-Levenshtein entre dos cadenas es definida como la m´ınima suma de costos
de acciones permitiendo hacer pasar la primera cadena a la segunda. Esas acciones son parecidas a
las que vimos en la clase, con un cambio. Incluyen: