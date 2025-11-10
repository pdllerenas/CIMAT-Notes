#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/equate:0.3.2": equate

#show: equate.with(breakable: true)
#set math.equation(numbering: "(1)")
#show: great-theorems-init

#let proof = proofblock()
#show: pset.with(
  class: "Análisis de Datos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 7",
  date: datetime.today(),
)

#let question = homework.complex-question
#let Var = $upright(bold(V a r))$
#let Cov = $upright(bold(C o v))$
#let Cor = $upright(bold(C o r))$

#question[
  Por experiencia se sabe que el numero de accesos $X$ durante una hora a una
  base de datos sigue una distribución de Poisson:
  $
    P(X=x) = exp(-lambda) lambda^x/x!, quad "con " x = 0,1,2,dots quad "y " lambda >0.
  $
  Calcula el estimador de máxima verosimilitud para $lambda$ basado en la
  muestra ${12,9,13,10,8}$.
]

#question[
  Supongamos que $X$ es una v.a. discreta y se obtiene una muestra:
  #{
    set table(align: (x, _) => if x == 0 { left } else { right })
    show table.cell.where(x: 0): smallcaps
    figure(table(
      columns: (auto, auto, auto, auto, auto, auto),
      stroke: none,
      table.vline(x: 1, start: 0),
      table.header($bold(X)$, $omega_1$, $omega_2$, $omega_3$, $omega_4$, $omega_5$),
      table.hline(),
      [Frecuencia], $20$, $8$, $4$, $16$, $2$,
    ))
  }
  Calcula los estimadores de máxima verosimilitud para las probabilidades de $X$.
]

#question[
  Supongamos que $[1.15, 4.20]$ es un intervalo de $95%$ de confianza para el
  promedio $mu$ del numero de televisores por hogar en EE.UU. ¿Cómo interpretar
  eso? Clasifica cada una de las siguientes frases como cierto o falso. Motiva tu
  respuesta.
  1. 95% de los hogares tienen entre 1.15 y 4.20 televisiones.
  2. La probabilidad de que $mu$ esté entre $1.15$ y 4.20 es 95%.
  3. De 100 intervalos calculados de la misma manera (a partir de su propia muestra), esperamos que 95% contiene $mu$.
]

#question[
  Los tiempos de ejecución (en segundos) de un algoritmo de búsqueda para diferentes corridas son:
  #figure(table(
    columns: 5,
    stroke: none,
    $9.589602$, $11.410870$, $9.618796$, $11.058610$, $9.643948$,
    $12.626794$, $9.276928$, $11.545544$, $10.945854$, $10.215046$,
    $10.551223$, $9.591871$, $9.406003$, $8.355287$, $7.477295$,
    $11.571839$, $8.587103$, $8.577009$, $9.908821$, $9.595157$,
  ))
  Calcula un 95% intervalo de confianza para el tiempo promedio de ejecución.
]

#question[
  Si $hat(theta)$ es un estimador para $theta$, entonces ¿$hat(theta)^2$ es un
  estimador insesgado para $theta^2$?

  Si $[hat(theta)_L, hat(theta)_R]$ es un intervalo de 95% de confianza para
  $theta$, entonces ¿$[exp(hat(theta))_L, exp(hat(theta))_R]$ es un intervalo de
  95% de confianza para $exp(theta)$?
]

#question[
  Considera los siguientes datos de un estudio en Bélgica sobre la intención de voto entre 1000 parejas. Las variables $X_1$, $X_2$ indican si la mujer y el hombre,
  respectivamente, votara para un partido de la coalición (0) o de la oposición (1) en caso de que hubieran elecciones en ese momento.
  #{
    set table(align: (x, _) => if x == 0 { left } else { right })
    show table.cell.where(x: 0): smallcaps
    figure(table(
      columns: (auto, auto, auto),
      stroke: none,
      table.vline(x: 1, start: 0),
      table.header([], $X_1 = 0$, $X_1 = 1$),
      table.hline(),
      $X_2 = 0$, $245$, $170$,
      $X_2 = 1$, $218$, $367$,
    ))
  }
  1. Calcula los momios empíricos $hat(R)$ a partir de los datos observados.
  2. Se puede mostrar que si el tamaño de la muestra va a $infinity$, la distribución
  de $log(hat(R))$ converge a una normal con promedio $log(R)$, el verdadero log-momio de la distribución subyacente, y con varianza
  $
    1/n_(0,0) + 1/n_(0,1) + 1/n_(1,0) + 1/n_(1,1),
  $
  donde $n_(i,j)$ es el numero de observaciones con $X_1 = i$ y $X_2 = j$.

  Calcula el valor de $p$ de la hipótesis que hombre y mujer votan de manera independiente.
]
