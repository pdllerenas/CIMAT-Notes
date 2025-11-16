#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 11",
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
  Describir y programar el método de iteración de punto fijo para sistemas de
  ecuaciones lineales en general.
]
Un sistema de ecuaciones lineales tiene la forma

$
  f_1 (x_1,x_2 & ,dots,x_n) = &                0, \
  f_2 (x_1,x_2 & ,dots,x_n) = &                0, \
               & dots.v       & dots.v #h(0.40em) \
  f_n (x_1,x_2 & ,dots,x_n) = &                0,
$
donde cada $f_i:RR^n -> RR$. Una manera corta de representar este sistema es
mediante un mapeo $bold(F):RR^n->RR^n$ definido mediante
$
  bold(F)(x_1,x_2, dots,x_n) = (f_1 (x_1, x_2, dots, x_n), f_2 (x_1, x_2, dots,x_n), dots, f_n (x_1,x_2,dots,x_n))^T.
$
Entonces, si denotamos $bold(x) = (x_1, dots, x_n)^T$ y $bold(0) = (0,dots, 0)^T$, tenemos
$
  bold(F)(bold(x)) = bold(0).
$

Sea $D = product_i^n [a_i, b_i]$ para $-infinity<a_i < b_i < infinity$. Si
$bold(G):D->D$, entonces $bold(G)$ tiene un punto fijo en $D$. Supongamos que
cada función componente de $bold(G)$ tienen derivadas parciales continuas y
existe $K > 1$ tal que
$
  abs((partial g_i (bold(x)))/(partial x_j)) <= K / n, quad "siempre que " bold(x) in D
$
par cada $j = 1,2,dots, n$ y cada función componente $g_i$. Entonces, la
sucesión de punto fijo ${bold(x)^((k))}_(k=0)^infinity$ definida por
$bold(x)^((0))$ seleccionada arbitrariamente en $D$ y generada por medio de
$
  bold(x)^((k)) = bold(G)(bold(x)^((k-1))), quad "para cada " k>=1
$
converge al único punto fijo $bold(p) in D$ y
$
  norm(bold(x)^((k)) - bold(p))_infinity <= K^k/(1-K) norm(bold(x)^((1)) - bold(x)^((0)))_infinity.
$
Para acelerar la convergencia, podemos usar la misma técnica que se usa en
Gauss-Siedel. Es decir, dado que calculamos $bold(x)_1, dots, bold(x)_n$
secuencialmente, usamos el nuevo valor de los previamente calculados para los
cálculos de los siguientes.

#question[
  Describir y programar alguno de los siguientes métodos de solución de
  sistemas de ecuaciones no lineales en general:
  1. Método de Broyden
  2. Método de Newton
  3. Método de gradiente conjugado de Fletcher-Reeves
]

El método de Newton para sistemas no lineales consiste en realizar la siguiente
iteración hasta convergencia:
$
  bold(x)^((k)) = bold(G)(bold(x)^((k-1))) = bold(x)^(k-1) - J(bold(x)^(k-1))^(-1) bold(F)(bold(x)^((k-1))),
$
donde $J$ es la matriz Jacobiana de $bold(F)$, y $bold(x)^((0))$ esta
suficientemente cerca de la solución del sistema. Esta convergencia es
cuadrática.

Como podemos notar, aquí tenemos dos condiciones que restringen el poder del método:
- Calculo de Jacobiana,
- Dependencia de elección de vector inicial.
Los métodos cuasi-Newton reemplazan la matriz Jacobiana por una matriz menos
costosa de actualizar por iteración.

1. *Método de Broyden*:
Supongamos que comenzamos con una aproximación inicial $bold(x)^((0))$ para la
solución $bold(p)$ de $bold(F)(bold(x)) = bold(0)$. Calculamos la siguiente
aproximación $bold(x)^((1))$ de la misma manera que en el método de Newton:
$
  bold(x)^((1)) = bold(x)^((0)) - J(bold(x)^((0)))^(-1) bold(F)(bold(x)^((0))).
$
Si no es factible calcular $J(bold(x^((0))))$, aproximamos mediante diferencias
finitas. Para la siguiente iteración, diferimos del método de Newton. Mas
precisamente, seguimos la idea general del método de la secante. En el caso
unidimensional, usamos
$
  f'(x_1) approx (f(x_1) - f(x_0))/(x_1 - x_0)
$
como reemplazo de la derivada.

Para sistemas no lineales, el denominador se convierte en un vector, por lo que
la expresión no está bien definida. Sin embargo, podemos reemplazar la matriz
jacobiana por una matriz $A_1$ tal que
$
  A_1(bold(x)^((1)) - bold(x)^((0))) = bold(F)(bold(x)^((1))) - bold(F)(bold(x)^((0))).
$
Usando que si $W subset RR^n$ es un sub-espacio lineal tenemos la descomposición ortogonal
$
  RR^n = W plus.circle W^perp.
$
Entonces,
$
  RR^n = "span"(bold(x)^((1)) - bold(x)^((0))) plus.circle "span"(bold(x)^((1)) - bold(x)^((0)))^perp,
$
por lo que para definir a $A_1$ de manera única, debemos especificar como actúa
en el complemento ortogonal. La definimos de tal manera que
$
  A_1 bold(z) = J(bold(x)^((0))) bold(z), quad "siempre que " (bold(x)^((1)) - bold(x)^((0)))^T bold(z) = 0.
$
Es decir, que no realice cambios en la dirección ortogonal.

Dadas estas condiciones, definimos a $A_1$ de manera única:
$
  A_1 = J(bold(x)^((0))) + ([bold(F)(bold(x)^((1))) - bold(F)(bold(x)^((0))) -
    J(bold(x)^((0)))(bold(x)^((1)) - bold(x)^((0)))](bold(x)^((1)) -
    bold(x)^((0)))^T)/(norm(bold(x)^((1)) - bold(x)^((0)))_2^2).
$
Entonces, la expresión de la siguiente iteración se convierte en
$
  bold(x)^((2)) = bold(x)^((1)) - A_1^(-1)bold(F)(bold(x)^((1))).
$
Definiendo $A_0 equiv J(bold(x)^((0)))$, repetimos la iteración para encontrar
$bold(x)^((3))$. Obtenemos la formula general
$
  bold(x)^((i+1)) = bold(x)^((i)) - A_i^(-1) bold(F)(bold(x)^((i))),
$
donde
$
  A_i = A_(i-1) + (bold(y)_i - A_(i-1) bold(s)_i)/norm(bold(s)_i)_2^2 bold(s)_i^T
$
con $bold(y)_i = bold(F)(bold(x)^((i))) - bold(F)(bold(x)^((i-1)))$ y
$bold(s)_i = bold(x)^((i)) - bold(x)^((i-1))$. Con esto, reducimos el numero de
evaluaciones funcionales escalares de $n^2+n$ a $n$ (solo debemos calcular
$bold(F)(bold(x)^((i)))$). Sin embargo, se siguen necesitando $cal(O)(n^3)$
cálculos para resolver
$
  A_i bold(s)_(i+1) = -bold(F)(bold(x)^((i))).
$<eq:sys>
Podemos realizar una mejora al método para evitar invertir la matriz $A_i$ en
cada iteración. La formula de *Sherman-Morrison* nos dice que si $A$ es una
matriz no singular y que $bold(x)$ y $bold(y)$ son los vectores que satisfacen
$bold(y)^T A^(-1)bold(x) eq.not -1$. Entonces $A + bold(x) bold(y)^T$ es no
singular y
$
  (A+bold(x) bold(y)^T)^(-1) = A^(-1) - (A^(-1) bold(x) bold(y)^T A^(-1))/(1+bold(y)^T A^(-1) bold(x)).
$
Al sustituir $A = A_(i-1)$, $bold(x) = (bold(y_i) - A_(i-1)
bold(s)_i)/norm(bold(s)_i)_2^2$, y $bold(y) = bold(s)_i$,
obtenemos (ver @burden2016)
$
  A_i^(-1) = A_(i-1)^(-1) + ((bold(s)_i - A_(i-1)^(-1)bold(y)_i)bold(s)_i^T
  A_(i-1)^(-1))/(bold(s)_i^T A_(i-1)^(-1)bold(y)_i).
$
Con esta reducción, ya no necesitamos resolver el sistema @eq:sys.
#question[
  Con los programas creados en 1) y 2), obtén la solución de los siguientes
  sistemas de ecuaciones no lineales:
  1. Inicializar con el vector inicial de $arrow(x)_0 = [1,5]^T$,
  $
        x_1 + x_2 & = 3, \
    x_1^2 + x_2^2 & = 9
  $<eq:g1>
  2. Inicializar con el vector inicial de $arrow(x)_0 = [0.1, 0.1, -0.1]^T$,
  $
                3x_1 - cos(x_2 x_3) - 1/2 & = 0, \
    x_1^2 - 81(x_2+0.1)^2 + sin(x_3)+1.06 & = 0, \
    e^(-x_1 x_2) + 20 x_3 + (10 pi - 3)/3 & = 0.
  $<eq:g2>
]

Presentaremos los resultados de utilizar la iteración de punto fijo y el método
de Broyden. Para usar la iteración de punto fijo, despejamos la funciones de la
siguiente manera:

$
  x_1 & = 3 - x_2, \
  x_2 & = sqrt(9 - x_1^2)
$<eq:f1>

$
  x_1 & = 1/3 cos(x_2 x_3) + 1/6, \
  x_2 & = 1/9sqrt(x_1^2 + sin(x_3)+1.06) - 0.1, \
  x_3 & = -1/20 e^(-x_1 x_2)- (10 pi - 3)/60.
$<eq:f2>
Para ejecutar el programa, usamos
```
make run-p3
```
Esto nos genera 2 archivos txt en `ex/fixed/` y otros 2 txt en `ex/broyden`. Se
ven de la siguiente manera:

#let data_fixed_1 = csv("Code/ex/fixed/f1.txt", row-type: array).slice(1)
#let data_fixed_2 = csv("Code/ex/fixed/f2.txt", row-type: array).slice(1)

#let data_broyden_1 = csv("Code/ex/broyden/f1.txt", row-type: array).slice(1)
#let data_broyden_2 = csv("Code/ex/broyden/f2.txt", row-type: array).slice(1)

#figure(
  table(
    columns: 3,
    stroke: none,
    table.hline(),
    table.header($x_1$, $x_2$, [Error Absoluto]),
    table.hline(),
    ..data_fixed_1.flatten(),
    table.hline(),
  ),
  caption: [Iteraciones de punto fijo en el sistema @eq:f1.],
)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($x_1$, $x_2$, $x_3$, [Error Absoluto]),
    table.hline(),
    ..data_fixed_2.flatten(),
    table.hline(),
  ),
  caption: [Iteraciones de punto fijo en el sistema @eq:f2.],
)

#figure(
  table(
    columns: 3,
    stroke: none,
    table.hline(),
    table.header($x_1$, $x_2$, [Error Absoluto]),
    table.hline(),
    ..data_broyden_1.flatten(),
    table.hline(),
  ),
  caption: [Método de Broyden en el sistema @eq:g1.],
)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($x_1$, $x_2$, $x_3$, [Error Absoluto]),
    table.hline(),
    ..data_broyden_2.flatten(),
    table.hline(),
  ),
  caption: [Método de Broyden en el sistema @eq:g2.],
)

#bibliography("ref.bib")
