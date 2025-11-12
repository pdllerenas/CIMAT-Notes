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
  Por experiencia se sabe que el numero de accesos $X$ durante una hora a una
  base de datos sigue una distribución de Poisson:
  $
    P(X=x) = exp(-lambda) lambda^x/x!, quad "con " x = 0,1,2,dots quad "y " lambda >0.
  $
  Calcula el estimador de máxima verosimilitud para $lambda$ basado en la
  muestra ${12,9,13,10,8}$.
]
Por definicion, la muestra tiene verosimilitud
$
  L(lambda | x_(1:n)) = product_(i=1)^(n) lambda^(x_i)/(x_i !) exp(-lambda) = exp(-lambda n)product_(i=1)^n lambda^(x_i)/(x_i !).
$
Entonces, su log-verosimilitud es
$
  ell(lambda) = -lambda n + sum_(i=1)^n x_i log(lambda) - log(x_i !).
$
Derivando, obtenemos
$
  ell'(lambda) = -n + 1/lambda sum_(i=1)^n x_i
$
Igualando a cero, tenemos
$
  lambda = 1/n sum_(i=1)^n x_i
$

Entonces,
$
  lambda = 1/5 (12 + 9 + 13 + 10 + 8) = #calc.round((12 + 9 + 13 + 10 + 8) / 5, digits: 4)
$
Para confirmar que es un maximo, notemos que $ell''(lambda) = -1/lambda^2 sum_(i=1)^n x_i < 0$.
#{
  set align(right)
  $square$
}

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
Por definicion,
$
  L(theta) & = P([20, 8, 4, 16,2] | theta) \
           & = product_(i=1)^n f(x_i|theta) \
           & =50!/(20!dot 8! dot 4!dot 16! dot 2!)p_1^(20) p_2^(8) p_3^(4) p_4^(16) p_5^2 \
$
#{
  set align(right)
  $square$
}



#question[
  Supongamos que $[1.15, 4.20]$ es un intervalo de $95%$ de confianza para el
  promedio $mu$ del numero de televisores por hogar en EE.UU. ¿Cómo interpretar
  eso? Clasifica cada una de las siguientes frases como cierto o falso. Motiva tu
  respuesta.
  1. 95% de los hogares tienen entre 1.15 y 4.20 televisiones.
  2. La probabilidad de que $mu$ esté entre $1.15$ y $4.20$ es 95%.
  3. De 100 intervalos calculados de la misma manera (a partir de su propia muestra), esperamos que 95% contiene $mu$.
]

Esto nos dice que si hacemos 100 muestreos, esperamos que aproximadamente 95 de los intervalos calculados contengan el verdadero
promedio $mu$.
1. Falso, es posible se tengan 0 o mas de 5 televisores, con el mismo intervalo de confianza.
2. Falso, el promedio es fijo, solo podemos decir si esta o no.
3. Cierto. Esta es la definicion.
#{
  set align(right)
  $square$
}

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

Supongamos que tienen una distribucion normal. Entonces, el promedio muestral y varianza insesgada son
#let arr = (
  9.589602,
  11.410870,
  9.618796,
  11.058610,
  9.643948,
  12.626794,
  9.276928,
  11.545544,
  10.945854,
  10.215046,
  10.551223,
  9.591871,
  9.406003,
  8.355287,
  7.477295,
  11.571839,
  8.587103,
  8.577009,
  9.908821,
  9.595157,
)
#let n = arr.len()
#let mean = arr.fold(0, (acc, x) => acc + x / n)
#let var = calc.round(arr.fold(0, (acc, x) => acc + (x - mean) * (x - mean) / (n - 1)), digits: 5)
#let sd = calc.sqrt(var)
$
  dash(X) & = mean \
      S^2 & = var
$

Entonces el valor
$
  T = sqrt(n)(dash(X) - mu)/S
$
es una distribucion $t$ de student con $19$ grados de libertad. Ahora, tenemos
$
  P(dash(X) - t_(19,0.025) S/sqrt(n) <= mu <= dash(X) + t_(19,0.025) S/sqrt(n)) = 0.95
$
Usando una tabla de valores, tenemos $t_(19,0.025) = 2.093$. Con la mustra proporcionada:
$
  [mean - #calc.round(2.093 * sd / calc.sqrt(20), digits: 4), mean +
    #calc.round(2.093 * sd / calc.sqrt(20), digits: 4)] =
  [#calc.round(mean - 2.093 * sd / calc.sqrt(20), digits: 4), #calc.round(mean + 2.093 * sd / calc.sqrt(20), digits: 4)]
$

#question[
  Si $hat(theta)$ es un estimador insesgado para $theta$, entonces ¿$hat(theta)^2$ es un
  estimador insesgado para $theta^2$?

  Si $[hat(theta)_L, hat(theta)_R]$ es un intervalo de 95% de confianza para
  $theta$, entonces ¿$[exp(hat(theta)_L), exp(hat(theta)_R)]$ es un intervalo de
  95% de confianza para $exp(theta)$?
]

Dado que $f(x) = x^2$ es una funcion convexa, por la desigualdad de Jensen y $EE[hat(theta)] = theta$ (estimador insesgado), tenemos
$
  theta^2 = f(theta) = f(EE[hat(theta)]) <= EE[hat(theta)^2].
$
Dado que $f''(x) = 2 > 0$, la funcion es estrictamente convexa. Esto nos dice que la desigualdad de Jensen es estricta:
$
  theta^2 < EE[hat(theta)^2].
$
En conclusion, $hat(theta)^2$ *no* es un estimador insesgado de $theta^2$.

Alternativamente, $EE[hat(theta)^2] = Var(hat(theta)) + EE[hat(theta)]^2 > hat(theta)^2$.#h(18em)
#{
  set align(right)
  $triangle$
}


Sea $[hat(theta)_L, hat(theta)_R]$ un 95% intervalo de confianza para $theta$. Por definicion,
$
  P([hat(theta)_L,hat(theta)_R] in.rev theta) = 0.95,
$
Notemos que, como $exp: RR -> RR_+$ es una funcion estrictamente creciente, tenemos
$
  hat(theta)_L <=theta <= hat(theta)_R <=> exp(hat(theta)_L) <= exp(theta) <= exp(hat(theta)_R).
$
Concluimos entonces que
$
  P([exp(hat(theta)_L),exp(hat(theta)_R)] in.rev exp(theta)) = P([hat(theta)_L, hat(theta)_R] in.rev exp(theta) ) = 0.95.
$
#{
  set align(right)
  $square$
}

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

1. Por definicion:
$
  hat(R) = (245slash 170)/(218 slash 367) = #calc.round(256 / 170, digits: 4)/#calc.round(218 / 367, digits: 4) = #calc.round((256 / 170) / (218 / 367), digits: 4)
$
Tambien tenemos el log-oddsratio:
$
  log(hat(R)) = #calc.round(calc.log((256 / 170) / (218 / 367)), digits: 4)
$
#{
  set align(right)
  $triangle$
}

2. Sabemos que el oddsratio es 1 si y solo si $X_1 perp X_2$. Entonces, definimos las hipotesis de la siguiente manera:
$
  H_0 & : R = 1, \
  H_1 & : R eq.not 1.
$
#let se = calc.round(1 / 245 + 1 / 218 + 1 / 170 + 1 / 367, digits: 4)

Sabemos que
$
  log(hat(R)) tilde cal(N)(log(R), 1/245 + 1/218 + 1/170 + 1/367) = cal(N)(log(R), se).
$
Es decir, tenemos $sigma = sqrt(0.0173) = #calc.round(calc.sqrt(se), digits: 4)$
Entonces, tenemos que el $p$-valor esta dado por
$
  p = 2P(Z < -abs(0.404)/sqrt((se))) = 2P(Z< -#calc.round(0.404 / calc.sqrt(se), digits: 4)) = 2 dot 0.00107 = #(2 * 0.00107).
$
Si $alpha = 5%$, rechazamos $H_0$. Es decir, es muy probable que los votos no sean independientes.
#{
  set align(right)
  $square$
}
