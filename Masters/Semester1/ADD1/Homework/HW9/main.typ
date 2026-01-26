#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/equate:0.3.2": equate
#import "@preview/lilaq:0.5.0" as lq

#show: equate.with(breakable: true)
#set math.equation(numbering: "(1)")
#show: great-theorems-init

#let vimg(body) = {
  text(body)
}

#let proof = proofblock()
#show: pset.with(
  class: "Análisis de Datos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 9",
  date: datetime.today(),
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

#let question = homework.complex-question
#let Var = $upright(bold(V a r))$
#let Cov = $upright(bold(C o v))$
#let Cor = $upright(bold(C o r))$

#question[
  Un géiser en Yellowstone National Park lanza vapor y agua caliente en
  intervalos irregulares durante un cierto tiempo. Se anexan datos históricos:
  la primera columna es el tiempo de espera en minutos hasta la i-ésima
  erupción ($colon.eq e_i$) y la segunda columna indica la duración de la
  i-ésima erupción. ($colon.eq d_i$).
  1. Ajusta el modelo adecuado de regresión para predecir el tiempo que vas a
    tener que esperar hasta la siguiente erupción en función de la duración
    de la ultima erupción, es decir, $e_(i+1)$ en función de $e_i$. Discute y
    evalúa tu modelo.
  2. Construye el resultado con regresión no para métrica (k-NN)
  3. Se ha observado que después de un intervalo corto entre dos erupciones,
    sigue muchas veces un intervalo largo. Verifica eso con los datos.
    Extiende tu modelo al incorporar como productores la duración de la
    penúltima erupción y el tiempo entre la ultima y penúltima erupción.
  4. Compara el poder predictivo de los dos modelos usando validación cruzada:
    esta técnica consiste en dividir los datos de manera aleatoria en --- por
    ejemplo --- 5 bloques con $20%$ de los datos. Para cada bloque se calcula
    el error de predicción para los datos de este bloque usando el modelo
    estimado en base de los datos de los demás bloques.
  ]
  1. 

#question[
  Supongamos que tenemos dos v.a. dependientes. Alguien propone como modelo $A$:
  $
    Y = alpha_A X + epsilon_A
  $
  y otra persona:
  $
    X = alpha_B Y + epsilon_B
  $
  Si $epsilon_A, epsilon_B$ son $cal(N)(0,1)$, deriva los estimadores de máxima
  verosimilitud para $alpha_A$ y $alpha_B$. ¿Se puede concluir que
  $hat(alpha)_A = 1/hat(alpha)_B$?
]

1. 

#question[
  (*Opcional*) Supongamos que $X$ y $Y$ son dos variables aleatorias con $EE[X]
  = EE[Y] = 0$ y $Var(X) = Var(Y) > 0$. Verifica que el valor de $a$ que
  minimiza: $EE(Y - a X)^2$ es igual a $Cor(X, Y)$.

  Este resultado nos dice como predecir el valor de $Y$ a través de $hat(Y) = a
  X$, si estamos dispuestos de aceptar que $EE(Y-hat(Y))^2$ es una buena medida
  para cuantificar el error. Da un ejemplo concreto donde $EE(Y-hat(Y))^2$ no
  captura bien error de un productor.

  Verifica que en este caso: $EE(Y-hat(Y))^2 = Var(X)(1-Cor^2(X,Y))$.
]
