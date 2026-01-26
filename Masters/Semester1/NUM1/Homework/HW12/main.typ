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
  Programar en `C++` el método de Euler, el método de Heun, el método de Taylor
  de $2degree$, y el método de Runge-Kutta de $4degree$ orden. Prueba tus
  programas con el problema de valor inicial $y' = y$, con $y(0) = 1$, en el
  intervalo $[0,4]$, cuya solución es $y(x) = e^x$. Grafica las soluciones.
]

- *Método de Euler*:
Es la técnica de aproximación mas básica para resolver problemas de valor inicial. Dada la ecuación
$
  (dif y)/(dif t) = f(t,y), quad a <= t <= b, quad y(a) = alpha,
$
se obtendrán puntos de aproximación en varios valores. Los puntos en los que se evalúa se encuentran equidistantes. Dado un $N in NN$,
establecemos $h = (b-a)/N$, y seleccionamos los puntos de evaluación
$
  t_i = a + i h, quad "para cada " i = 0, 1, 2, ..., N
$
Entonces, la distancia entre los puntos es $h = t_(i+1) - t_i$.

Supongamos que la ecuación original tiene una solución única en $[a,b]$. Por el
teorema de Taylor, esta tiene dos derivadas continuas en $[a,b]$, de tal forma
que para cada $i = 0,1,2,...,N-1$ tenemos
$
  y(t_(i+1)) = y(t_i) + (t_(i+1) - t_i) y'(t_i) + (t_(i+1) - t_i)^2/2 y''(xi_i),
$
que es equivalente a
$
  y(t_(i+1)) = y(t_i) + h y'(t_i) + (h)^2/2 y''(xi_i).
$
Ya que $y(t)$ satisface la ecuación diferencial, tenemos
$
  y(t_(i+1)) = y(t_i) + h f(t_i, y(t_i)) + (h)^2/2 y''(xi_i).
$
Entonces, el método de Euler es
$
      omega_0 & = alpha, \
  omega_(i+1) & = omega_i + h f(t_i, omega_i),quad "para cada " i = 0,1,2,...,N-1.
$
- *Método de Taylor de orden $n$*.
Esta es una generalización del método de Euler. Es decir, tomamos mas términos
de la expansión de Taylor de $f$, y llegamos a la siguiente expresión:
$
  y(t_(i+1)) & = y(t_i) + h f(t_i, y(t_i)) + h^2/2 f'(t_i, y(t_i)) + dots.c + \
             & quad h^n/(n!) f^((n-1))(t_i, y(t_i)) + h^(n+1)/((n+1)!) f^((n))(xi_i, y(xi_i)).
$
Es decir, tenemos la relación de recurren cía
$
  omega_0 = alpha,
  omega_(i+1) = omega_i + h T^((n))(t_i, omega_i), quad "para cada " i = 0, 1,2,dots,N-1,
$
donde
$
  T^((n))(t_i, omega_i) = f(t_i, omega_i) + h/2 f'(t_i, omega_i) + dots.c + h^(n-1)/(n!)f^((n-1))(t_i, omega_i).
$
- *Método de Heun*
A comparación del método de Taylor, este método tiene la ventaja de no necesitar las derivadas de la función. La relación de recurren cía es dada por
$
  omega_0 &= alpha,\
  omega_(i+1) &= omega_i + h/4 (f(t_i, omega_i) + 3 (f(t_i + (2h)/3, omega_i + (2h)/3 f(t_i + h/3, omega_i + h/3 f(t_i, omega_i))))),
$

- *Método de Runge-Kutta*
El método de Heun es el caso particular de los métodos de Runge-Kutta de orden $n$. El método mas utilizado de Runge-Kutta es el de 4to orden, dado por
$
      omega_0 & = alpha, \
  omega_(i+1) & = omega_i + 1/6 (k_1 + 2 k_2 + 2k_3 + k_4),
$
donde
$
  k_1 & = h f(t_i, omega_i), \
  k_2 & = h f(t_i+ h/2, omega_i + 1/2 k_1), \
  k_3 & = h f(t_i + h/2, omega_i + 1/2 k_2), \
  k_4 & = h f(t_(i+1), omega_i + k_3).
$
- *Ejemplo*
Para resolver el ejemplo, usamos
```
make run-p1
```
#let euler-p1 = csv("Code/ex/p1/euler.csv", row-type: array).slice(1)
#let taylor2-p1 = csv("Code/ex/p1/taylor2.csv", row-type: array).slice(1)
#let heun-p1 = csv("Code/ex/p1/heun.csv", row-type: array).slice(1)
#let rk4-p1 = csv("Code/ex/p1/rk4.csv", row-type: array).slice(1)
Esto nos genera 4 archivos `csv` con la siguiente información:
#grid(
  columns: (1fr, 1fr, 1fr, 1fr),
  align: center,
  figure(
    table(
      stroke: none,
      columns: 2,
      table.hline(),
      table.header($x$, $y$),
      table.hline(),
      ..euler-p1.slice(0, 11).flatten(),
      $dots.v$,
      $dots.v$,
      ..euler-p1.slice(90, 101).flatten(),
      table.hline(),
    ),
    caption: [Euler],
  ),
  figure(
    table(
      stroke: none,
      columns: 2,
      table.hline(),
      table.header($x$, $y$),
      table.hline(),
      ..taylor2-p1.slice(0, 11).flatten(),
      $dots.v$,
      $dots.v$,
      ..taylor2-p1.slice(90, 101).flatten(),
      table.hline(),
    ),
    caption: [Taylor],
  ),
  figure(
    table(
      stroke: none,
      columns: 2,
      table.hline(),
      table.header($x$, $y$),
      table.hline(),
      ..heun-p1.slice(0, 11).flatten(),
      $dots.v$,
      $dots.v$,
      ..heun-p1.slice(90, 101).flatten(),
      table.hline(),
    ),
    caption: [Heun],
  ),
  figure(
    table(
      stroke: none,
      columns: 2,
      table.hline(),
      table.header($x$, $y$),
      table.hline(),
      ..rk4-p1.slice(0, 11).flatten(),
      $dots.v$,
      $dots.v$,
      ..rk4-p1.slice(90, 101).flatten(),
      table.hline(),
    ),
    caption: [RK4],
  ),
)

Las siguientes imágenes muestran las aproximaciones con 100 pasos en el intervalo [0,4] de la ecuación diferencial $y'=y$.

#grid(
  columns: 2,
  rows: 2,
  figure(image("euler_p1.png")), figure(image("heun_p1.png")),
  figure(image("taylor_p1.png")), figure(image("rk4_p1.png")),
)

#question[
  Se define la función $y(x)$ de la forma
  $
    y(x) = integral_0^x sqrt(1+t^3) dif t.
  $
  donde $0<= x <= 2$. *Nota*: la integral $integral sqrt(1+t^3) dif t$ es
  elíptica y por lo tanto no es posible obtener una primitiva expresarle en
  términos de funciones elementales.
  1. Utilizando la formula de cuadratura de Gauss de 3 puntos  aproxima el
    valor de la integral.
  2. La igualdad del ejercicio se puede escribir como un problema de Cauchy, es
    decir, un problema de una ecuación diferencial con condición inicial $y(0)
    = 0$, halla el valor de $y(2)$ utilizando los 4 métodos programados en el
    inciso 1 usando $h=0.2$ (es equivalente a encontrar el valor de la
    integral del inciso a). Realiza una tabla comparativa de los 4 métodos por
    cada incremento de $h$. ¿Qué piensas sobre los métodos? Grafica las
    aproximaciones de las soluciones.
]
1. La cudratura de Gauss de orden 3 fue programada en una tarea pasada. Para encontrar la aproximación mediante este método, usamos
```
make run-p2
```
Esto nos imprime en la consola el resultado de dicha integral:
```
3.24182
```
2. Para realizar las aproximaciones mediante los métodos, debemos reescribir la ecuación usando la derivada:
$
  y'(x) = sqrt(1+x^3),
$
que se sigue del teorema fundamental del cálculo.

Entonces, con el comando anterior, también calculamos los valores de cada
método usando esta ecuación diferencial. Obtenemos las siguientes tablas:

#let integral_euler = csv("Code/ex/p2/euler.csv", row-type: array).slice(1)
#let integral_taylor = csv("Code/ex/p2/taylor2.csv", row-type: array).slice(1)
#let integral_heun = csv("Code/ex/p2/heun.csv", row-type: array).slice(1)
#let integral_rk4 = csv("Code/ex/p2/rk4.csv", row-type: array).slice(1)

#grid(
  columns: 2,
  rows: 2,
  column-gutter: 2em,
  row-gutter: 3em,
  figure(
    table(
      columns: 2,
      stroke: none,
      table.hline(),
      table.header([*x*], $integral_0^x sqrt(1+t^3) dif t$),
      table.hline(),
      ..integral_euler.flatten(),
      table.hline(),
    ),

    caption: [Aproximación de integral usando método de Euler.],
  ),
  figure(
    table(
      columns: 2,
      stroke: none,
      table.hline(),
      table.header([*x*], $integral_0^x sqrt(1+t^3) dif t$),
      table.hline(),
      ..integral_taylor.flatten(),
      table.hline(),
    ),
    caption: [Aproximación de integral usando método de Taylor.],
  ),

  figure(
    table(
      columns: 2,
      stroke: none,
      table.hline(),
      table.header([*x*], $integral_0^x sqrt(1+t^3) dif t$),
      table.hline(),
      ..integral_heun.flatten(),
      table.hline(),
    ),
    caption: [Aproximación de integral usando método de Heun.],
  ),
  figure(
    table(
      columns: 2,
      stroke: none,
      table.hline(),
      table.header([*x*], $integral_0^x sqrt(1+t^3) dif t$),
      table.hline(),
      ..integral_rk4.flatten(),
      table.hline(),
    ),
    caption: [Aproximación de integral usando método de Runge-Kutta 4.],
  ),
)

#figure(image("p2_compare.png"))

Los métodos ha logrado aproximar la integral a una buena precisión. La de
Runge-Kutta fue la mas precisa de las 5 aproximaciones. Sin embargo, la
cantidad de cálculos requeridos para realizar esta aproximación exceden los
necesarios para la cuadratura de Gauss, donde la mayor parte del trabajo se
hace en la compilación mediante los valores predefinidos con `constexpr` de las
raíces y coeficientes de los polinomios de Legendre. En conclusión, para
integración, la cuadratura Gaussiana es mas eficiente.

#let lynx-data = csv("Code/ex/lynx-rabbit.txt", row-type: array).slice(1)

#question[
  Considera el siguiente problema sobre la dinámica depredador-presa de linces-conejos.

  La siguiente tabla muestra el indice de capturas de linces y conejos
  elaborada por la compañía Hudson Bay entre los años 1900 y 1920.

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
  La dinámica del comportamiento de depredador presa de puede analizar mediante
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
  2. Modifica los 4 métodos programados en el inciso 1 y resuelve el sistema de
    ecuaciones diferenciales con las condiciones iniciales establecidas, para
    $t=[0,25]$.
  3. En algunos casos, para el análisis de sistemas de ecuaciones diferenciales
    se estudia el comportamiento del plano fase mediante denominadas órbitas
    de solución, el cual esta dado por la grafica de las componentes del
    sistema. Para este ejercicio, debes graficar coordenadas $(x,y) =
    ("conejos", "presas")$ (ver documento complementario).
]

Para generar los puntos correspondientes, usamos
```
make run-p3
```
#let euler_p3 = csv("Code/ex/p3/euler.csv", row-type: array).slice(1)
#let taylor2_p3 = csv("Code/ex/p3/taylor2.csv", row-type: array).slice(1)
#let heun_p3 = csv("Code/ex/p3/heun.csv", row-type: array).slice(1)
#let rk4_p3 = csv("Code/ex/p3/rk4.csv", row-type: array).slice(1)

Esto nos genera 4 archivos `csv`, que mostramos en las siguientes tablas. Dado que hay muchos valores,
solo mostramos los primeros y últimos 10 valores de cada tabla.

#grid(
  columns: 4,
  figure(
    table(
      stroke: none,
      columns: 3,
      table.vline(),
      table.hline(),
      table.header([*Year*], [*Rabbits*], [*Lynx*]),
      table.hline(),
      ..euler_p3.slice(0, 10).flatten(),
      $dots.v$,
      $dots.v$,
      $dots.v$,
      ..euler_p3.slice(90, 101).flatten(),
      table.vline(),
      table.hline(),
    ),
    caption: [Euler],
  ),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([*Year*], [*Rabbits*], [*Lynx*]),
      table.hline(),
      ..euler_p3.slice(0, 10).flatten(),
      $dots.v$,
      $dots.v$,
      $dots.v$,
      ..euler_p3.slice(90, 101).flatten(),
      table.vline(),
      table.hline(),
    ),
    caption: [Taylor],
  ),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([*Year*], [*Rabbits*], [*Lynx*]),
      table.hline(),
      ..euler_p3.slice(0, 10).flatten(),
      $dots.v$,
      $dots.v$,
      $dots.v$,
      ..euler_p3.slice(90, 101).flatten(),
      table.vline(),
      table.hline(),
    ),
    caption: [Heun],
  ),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header([*Year*], [*Rabbits*], [*Lynx*]),
      table.hline(),
      ..euler_p3.slice(0, 10).flatten(),
      $dots.v$,
      $dots.v$,
      $dots.v$,
      ..euler_p3.slice(90, 101).flatten(),
      table.vline(),
      table.hline(),
    ),
    caption: [RK4],
  ),
)

1. #figure(image("lynx_rabbit.png"))
3. #grid(
    columns: 2,
    rows: 2,
    gutter: 1cm,
    figure(image("euler_p3.png"), caption: [Método de Euler]),
    figure(image("taylor2_p3.png"), caption: [Método de Taylor de orden 2]),

    figure(image("heun_p3.png"), caption: [Método de Heun]),
    figure(image("rk4_p3.png"), caption: [Método de Runge-Kutta de orden 4]),
  )
