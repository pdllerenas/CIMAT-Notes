#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 12",
  date: datetime.today(),
)

#show: great-theorems-init
#let mathcounter = rich-counter(identifier: "mathblocks", inherited_levels: 1)
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
  Programar en `C++` el metodo de Euler, el metodo de Heun, el metodo de Taylor
  de $2degree$, y el metodo de Runge-Kutta de $4degree$ orden. Prueba tus
  programas con el problema de valor inicial $y' = y$, con $y(0) = 1$, en el
  intervalo $[0,4]$, cuya solucion es $y(x) = e^x$. Grafica las soluciones.
]

#question[
  Se define la funcion $y(x)$ de la forma
  $
    y(x) = integral_0^x sqrt(1+t^3) dif t.
  $
  donde $0<= x <= 2$. *Nota*: la integral $integral sqrt(1+t^3) dif t$ es
  eliptica y por lo tanto no es posible obtener una primitiva expresable en
  terminos de funciones elementales.
  1. Utilizando la formula de cuadratura de Gauss de 3 puntos  aproxima el
    valor de la integral.
  2. La igualdad del ejercicio se puede escribir como un problema de Cauchy, es
    decir, un problema de una ecuacion diferencial con condicion inicial $y(0)
    = 0$, halla el valor de $y(2)$ utilizando los 4 metodos programados en el
    inciso 1 usando $h=0.2$ (es equivalente a encontrar el valor de la
    integral del inciso a). Realiza una tabla comparativa de los 4 metodos por
    cada incremento de $h$. ¿Qué piensas sobre los metodos? Grafica las
    aproximaciones de las soluciones.
]

#let lynx-data = csv("Code/ex/lynx-rabbit.txt", row-type: array).slice(1)

#question[
  Considera el siguiente problema sobre la dinamica depredador-presa de linces-conejos.

  La siguiente tabla muestra el indice de capturas de linces y conejos
  elaborada por la compañia Hudson Bay entre los años 1900 y 1920.

  #grid(
    columns: (1fr, 1fr),
    align: center,
    figure(table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([*Year*], [*Rabbits*], [*Lynx*]),
      table.hline(),
      ..lynx-data.slice(0, 11).flatten(),
      table.hline(),
    )),
    figure(table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([*Year*], [*Rabbits*], [*Lynx*]),
      table.hline(),
      ..lynx-data.slice(11).flatten(), [1922], [---], [---],
      table.hline(),
    )),
  )
  La dinamica del comportamiento de depredador presa de puede analizar mediante
  ecuaciones diferenciales del tipo _Lotka-Volterra_, para este ejemplo, las
  ecuaciones diferenciales para el problema de valor inicial que definen el
  comportamiento de las especies a traves del tiempo estan dadas por

  $
    cases(
      x'(t) = 0.4 x(t) - 0.018 x(t)y(t) & x(0) = 30, \
      y'(t) = -0.8 y(t) + 0.023 x(t)y(t) & y(0) = 4
    )
  $

  1. Grafica los puntos de la Tabla de datos medidos.
  2. Modifica los 4 metodos programados en el inciso 1 y resuelve el sistema de
    ecuaciones diferenciales con las condiciones iniicales establecidas, para
    $t=[0,25]$.
  3. En algunos casos, para el analisis de sistemas de ecuaciones diferenciales
    se estudia el comportamiento del plano fase mediante denominadas orbitas
    de solucion, el cual esta dado por la grafica de las componentes del
    sistema. Para este ejercicio, debes graficar coordenadas $(x,y) =
    ("conejos", "presas")$ (ver documento complementario).

    *Nota 1*: Para simplificar el ejercicio 3b, se sugiere pensar en
    solucionar el ejercicio 1 de manera vectorial, asi, solo se tendria que
    cambiar la dimension de los vectores para resolver este problema (ver
    ultima diapositiva del tema en la presentacion del miercoles, antes del
    avance de la tarea).

    *Nota 2*: Para mayor detalle de como se resuelve el problema de estimacion
    empirica, puntos de equilibrio, etc., revisar documento adjunto.
]
