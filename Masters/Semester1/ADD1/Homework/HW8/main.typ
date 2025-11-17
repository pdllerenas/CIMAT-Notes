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
  title: "Tarea 7",
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
  Para investigar si una moneda es justa o no, alguien decide que va a lanzar
  la moneda 100 veces. Si el numero de veces de obtener sol es entre $42%$ y
  $58%$, va a apoyar la hipotesis de que la moneda es justa. Calcula la
  probabilidad de un error tipo I correspondiente.
]

Un error de tipo I se obtiene al rechazar la hipotesis nula cuando es
verdadera. Entonces, supongamos que, efectivamente, si $X$ es la v.a. del
resultado de tirar una moneda es $X ~ "Bern"(0.5)$, y al lanzarla 100 veces, es
decir, $Y ~ "Binomial"(100, 0.5)$.
Tenemos 2 casos:
- $Y <= 42$
- $Y >= 58$
Es decir, queremos $1-P(42 < Y < 58) = P(Y<= 42) + P(Y >= 58)$. Notemos que como como la moneda es justa,
$P(Y<=42) = P(Y>= 58)$ (por simetria alrededor de $Y = 50$), por lo que solo calculamos uno de ellos. Obtenemos
$
  P(Y <= 42) & = sum_(i=0)^42 binom(100, i) 0.5^i (0.5)^(100-i) \
             & = sum_(i=0)^42 binom(100, i)0.5^(100) \
             & approx 0.0666.
$
Concluimos que un error de tipo I ocurre con probabilidad
$
  2 P(Y<= 42) = 0.133.
$
#{
  set align(right)
  $square$
}

#question[
  ¿Cierto, falso o indecidible? Explica tu respuesta.
  1. En pruebas de hipótesis con el enfoque de Neyman-Pearson $alpha + "poder" = 1$.
  2. Al aumentar $alpha$, en general la curva de poder va a subir.
  3. Si el tamano de la muestra disminuye, la probabilidad de un error de tipo II va a aumentar.
]
1. Falso. Definimos $alpha = P_(H_0)$ (error tipo I) y $"poder" = 1 - P_(H_1)$ (error tipo II). Entonces,
$
  alpha + "poder" = 1 + P_(H_0) - P_(H_1).
$
Entonces, el enunciado solo se cumple cuando los errores son iguales.

2. Cierto. Cuando aumentamos $alpha$, rechazamos $H_0$ con datos menos
  extremos, aun cuando $H_0$ es verdadero . Esto implica que tambien
  rechazamos $H_0$ con datos menos extremos cuando $H_1$ es verdadero. Es
  decir, $1 - P_(H_1)$ es mayor, ya que reducimos el error de tipo II.

3. Cierto. Sabemos que el poder es inversamente proporcional a $sigma /
  sqrt(n)$ (donde el numerador de la expresion completa es la diferencia de
  los medias, como vimos en clase). Es decir, proporcional a $sqrt(n) /
  sigma$. Entonces, si decrece $n$, el poder decrece, y por lo tanto,
  el error de tipo II incrementa.

#let x = lq.linspace(-3, 5)
#let x2 = lq.linspace(-8, 8)

#figure(
  lq.diagram(
    lq.plot(mark: none, smooth: true, label: $H_0$, x, x => 1 / (2 * calc.sqrt(calc.pi)) * calc.exp(-x * x)),
    lq.plot(mark: none, smooth: true, label: $H_1$, x, x => (
      1 / (2 * calc.sqrt(calc.pi)) * calc.exp(-(x - 2) * (x - 2))
    )),
    lq.vlines(1.5, stroke: red, min: 0, max: 0.218, label: $alpha$),
  ),
  caption: [Comparacion de curvas con desviaciones estandar distintas, $alpha$ fijo.],
)

#question[
  Supongamos que se tienen muestras de dos v.a. $X$ y $Y$. Se hace la prueba de hipotesis:
  $
    H_0: EE[X] - EE[Y] eq 0 quad "vs." quad H_1: EE[X] - EE[Y] eq.not 0.
  $
  Se obtiene un valor $p$ igual a 0.002. Este valor indica:
  1. Tenemos evidencia de una gran diferencia entre los promedios de $X$ y $Y$.
  2. Tenemos evidencia fuerte de una diferencia entre los promedios de $X$ y $Y$.
  3. Observamos una diferencia grande en el promedio muestral de $X$ y $Y$.
  4. Todo lo anterior.
  5. Nada de lo anterior.
  Elige la respuesta correcta y motiva tu eleccion.
]

2. El $p$-valor diminuto nos dice que los datos observados no soportan la
  hipotesis nula, pero no nos dice nada de las magnitudes de lo que
  comparamos, solamente si es plausible o no.

#question[
  Durante los juegos olimpicos de Salt Lake City surgio en un periodico la
  discusion si en las pruebas de 1500m de patinaje, la persona en el carril
  exterior no tendria ventaja sobre el carril interior. Se organizaron 24 pruebas
  (una se cancelo por caida). Abajo los tiempos. Aplica una(s) pruebas de
  estadistica relevante para contestar esta pregunta.
]
#figure(table(
  columns: 8,
  stroke: none,
  table.hline(),
  table.vline(),
  table.header(
    [*Race number*], table.vline(),
    [*Inner lane*],
    [*Outer lane*],
    [*Difference*],
    [*Race number*], table.vline(),
    [*Inner lane*],
    [*Outer lane*],
    [*Difference*],
  ),
  table.hline(),
  table.vline(),
  $1$, $107.04$, $105.98$, $1.06$,
  table.vline(),
  $2$, $109.24$, $108.20$, $1.04$,
  $3$, $111.02$, $108.40$, $2.62$,
  $4$, $108.02$, $108.58$, $-0.56$,
  $5$, $107.83$, $105.51$, $2.32$,
  $6$, $109.50$, $112.01$, $-2.51$,
  $7$, $111.81$, $112.87$, $-1.06$,
  $8$, $111.02$, $106.40$, $4.62$,
  $9$, $106.04$, $104.57$, $1.47$,
  $10$, $110.15$, $110.70$, $-0.55$,
  $11$, $109.42$, $109.45$, $-0.03$,
  $12$, $108.13$, $109.57$, $-1.44$,
  $14$, $105.86$, $105.97$, $-0.11$,
  $15$, $108.27$, $105.63$, $2.64$,
  $16$, $107.63$, $105.41$, $2.22$,
  $17$, $107.72$, $110.26$, $-2.54$,
  $18$, $106.38$, $105.82$, $0.56$,
  $19$, $107.78$, $106.29$, $1.49$,
  $20$, $108.57$, $107.26$, $1.31$,
  $21$, $106.99$, $103.95$, $3.04$,
  $22$, $107.21$, $106.00$, $1.21$,
  $23$, $105.34$, $105.26$, $0.08$,
  $24$, $108.76$, $106.75$, $2.01$,
  table.hline(),
))


#let blue = rgb("3b00ff")     // Americano nativo
#let orange = rgb("f90016")   // Caucasiano

#question[
  En un estudio sobre herencia genetica de Margolin [1988], se tomaron muestras de diferentes grupos etnicos. Tomaron muestras de sangre de cada individuo de
  diferentes grupos etnicos. Tomaron muestras de sangre de cada individuo y se midieron varias substancias. nos limitamos a los grupos Americano nativo y Caucasiano
  y la variable MSCE (mean sister chromatid exchange). Los datos obtenidos son:

  #figure(table(
    columns: 10,
    stroke: none,
    [#text(fill: blue, "Americano nativo")],
    table.vline(),
    $8.50$,
    $9.48$,
    $8.65$,
    $8.16$,
    $8.83$,
    $7.76$,
    $8.63$,
    [---],
    [---],
    table.hline(),
    [#text(fill: orange, "Caucasiano")], $8.27$, $8.20$, $8.25$, $8.14$, $9.00$, $8.10$, $7.20$, $8.32$, $7.70$,
  ))
  Usa una prueba no parametrica para la hipotesis de que los conjuntos de datos
  provienen de una misma distribucion. Compara tu resultado con los que se
  obtienen con una prueba computacionalmente intensiva.

  Hint: para permutar valores; por ejemplo en `R` se obtiene una permutacion de
  $1,2,dots, 9, 10$ con el comando `sample(1:10, 10)`, o sea, muestra sin
  reemplazo. En `Python` puedes usar `numpy.random.permutation`.
]

Definimos nuestras hipotesis como
$ 
H_0: "misma distribucion", quad H_1: "diferente distribucion".
$ 
Usaremos la _Wilcoxon inversion test_ o _U-test_ (test 49). Asumimos que ambas
distribuciones son continuas y que las muestras son aleatorias e
independientes. Juntando y ordenando los datos, tenemos la siguiente lista:

#table(
  stroke: none,
  columns: 17,
  align: center,
  [
    #text(fill: orange, "7.20")
  ],
  [
    #text(fill: orange, "7.70")
  ],
  [
    #text(fill: blue, "7.76")
  ],
  [
    #text(fill: orange, "8.10")
  ],
  [
    #text(fill: orange, "8.14")
  ],
  [
    #text(fill: blue, "8.16")
  ],
  [
    #text(fill: orange, "8.20")
  ],
  [
    #text(fill: orange, "8.25")
  ],
  [
    #text(fill: orange, "8.27")
  ],
  [
    #text(fill: orange, "8.32")
  ],
  [
    #text(fill: blue, "8.50")
  ],
  [
    #text(fill: blue, "8.63")
  ],
  [
    #text(fill: blue, "8.65")
  ],
  [
    #text(fill: blue, "8.83")
  ],
  [
    #text(fill: orange, "9.00")
  ],
  [
    #text(fill: blue, "9.48")
  ],
)
Tenemos $2 + 4 + 5 + 5 + 5 + 6 + 7 = #(2 + 4 + 5 + 5 + 5 + 6 + 7)$ inversiones.
El numero de no-inversiones es $9dot 7 - 34 = #(9 * 7 - (2 + 4 + 5 + 5 + 5 + 6
+ 7))$ Para $alpha = 0.05$, el valor critico es 15, por lo que *no* rechazamos la hipotesis nula.

#question[
  Sea $c$ una cierta cadena binaria de longitud 100. Se quiere verificar si
  proviene de una muestra $"Bern"(0.5) eq.colon H_0$. Para eso se calcula el
  numero de cambios ($T_1$) y la longitud de la cadena mas larga de un mismo
  valor ($T_2$). Un cambio es un 1 seguido por un 0, o un 0 seguido por un 1 en
  la cadena.

  Usando muchas simulaciones de cadenas bajo $H_0$, estima y visualiza la
  distribucion de $T = (T_1, T_2)$.

  Pide un conocido generar una cadena binaria lanzando 100 veces una mondeda y
  otra cadena que se inventa (fake). Aplica lo anterior para distinguir cual de
  los dos es fake basado en el valor de $p$.
]



