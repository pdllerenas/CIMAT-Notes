#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 10",
  date: datetime.today(),
)

#show: great-theorems-init
#let mathcounter = rich-counter(identifier: "mathblocks", inherited_levels: 1)
#let theorem = mathblock(blocktitle: "Teorema", counter: mathcounter)
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

#question[
  Describir y programar el método de iteración de punto fijo para sistemas de ecuaciones lineales en general.
]

#question[
  Describir y programar alguno de los siguientes metodos de solucion de sistemas de ecuaciones no lineales en general:
  1. Método de Broyden
  2. Método de Newton
  3. Método de gradiente conjugado de Fletcher-Reeves
]

#question[
  Con los programas creados en 1) y 2), obten la solucion de los siguientes sistemas de ecuaciones no lineales:
  1. Inicializar con el vector inicial de $arrow(x)_0 = [1,5]^T$,
  $
        x_1 + x_2 & = 3, \
    x_1^2 + x_2^2 & = 9
  $
  2. Inicializar con el vector inicial de $arrow(x)_0 = [0.1, 0.1, -0.1]^T$,
  $
                3x_1 - cos(x_2 x_3) - 1/2 & = 0, \
    x_1^2 - 81(x_2+0.1)^2 + sin(x_3)+1.06 & = 0, \
    e^(-x_1 x_2) + 20 x_3 + (10 pi - 3)/3 & = 0.
  $
]

