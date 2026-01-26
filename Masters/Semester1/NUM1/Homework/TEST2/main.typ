#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#set text(lang: "es")
#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Examen Parcial II",
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
  Aproxima el valor de
  $
    ln(3) = integral_1^3 1/x dif x
  $
  utilizando 4 de los métodos estudiados en clase, ademas, realiza un
  comparativo de errores absolutos respecto al valor exacto.
]

```
make run-p1
```
#let open-p1 = csv("Code/ex/p1/open.csv", row-type: array).slice(1)
#let closed-p1 = csv("Code/ex/p1/closed.csv", row-type: array).slice(1)
#let gaussian-p1 = csv("Code/ex/p1/quadrature.csv", row-type: array).slice(1)
#grid(
  columns: (1fr, 1fr, 1fr),
  align: center,
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header($n$, $integral_1^3 1/x dif x$, "abs error"),
      table.hline(),
      ..open-p1.flatten(),
      table.hline(),
    ),
    caption: [Métodos de Newton-Cotes abierto.],
  ),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header($n$, $integral_1^3 1/x dif x$, "abs error"),
      table.hline(),
      ..closed-p1.flatten(),
      table.hline(),
    ),
    caption: [Métodos de Newton-Cotes cerrado.],
  ),
  figure(
    table(
      stroke: none,
      columns: 3,
      table.hline(),
      table.header($n$, $integral_1^3 1/x dif x$, "abs error"),
      table.hline(),
      ..gaussian-p1.flatten(),
      table.hline(),
    ),
    caption: [Cuadraturas de Gauss.],
  ),
)


#question[
  Considera el problema de valor inicial,
  $
    y'' - 2y' + y & = cos (x), quad
                    y'(0) & = 1,quad
                            y(0) & = 3/2.
  $
  el cual tiene como solución única a $y(x) = 3/2 e^x - 1/2 sin(x)$.
  1. Realiza una propuesta de solución de la ecuación diferencial en el
    intervalo $x in [0,2]$ según los temas estudiados en clase. Explica lo mas
    claro posible tu procedimiento y elección.
  2. Grafica tu solución y compárala con la solución analítica en el intervalo
    $x in [0,2]$.
]

1.
Antes de aplicar algún método, debemos transformar la ecuación diferencial de
segundo orden en un sistema de ecuaciones diferenciales de primer orden.

Sea $u_1(t) = y(t)$ y $u_2(t) = y'(t)$. Entonces, obtenemos el siguiente
sistema de ecuaciones:
$
  u'_1(t) & = u_2(t), \
  u'_2(t) & = cos(t) + 2u_2(t) - u_1(t).
$<eq:odesys>
con condiciones iniciales $u_1(0) = 3/2$, $u_2(0) = 1$. Ahora, usaremos
Runge-Kutta 4 para sistemas de ecuaciones diferenciales para encontrar la
aproximación. Usamos este método porque es el que mayor estabilidad numérica
tiene, ademas de que es el mas preciso, como ha sido demostrado en tareas pasadas.

Para ejecutar el programa, usamos
```
make run-p2
```
Esto nos genera un archivo `csv` en `ex/p2/rk4.csv` (junto con los otros métodos, pero solo analizaremos la de Runge-Kutta).
Mostramos los primeros 5 y últimos 5 valores de la tabla.
#let rk4-p2 = csv("Code/ex/p2/rk4.csv", row-type: array).slice(1)

#figure(
  table(
    stroke: none,
    columns: 3,
    table.hline(),
    table.header($t$, $u_1(t)$, $u_2(t)$),
    table.hline(),
    ..rk4-p2.slice(0, 5).flatten(),
    $dots.v$,
    $dots.v$,
    $dots.v$,
    ..rk4-p2.slice(96, 101).flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones del método de Runge-Kutta 4 para el sistema de ecuaciones diferenciales @eq:odesys.],
)<t:1>
2.
Para comparar nuestra aproximación con la solución analítica, traficamos los
valores $(t, u_1(t)) = (t, y(t))$. Esto nos genera la siguiente gráfica:

#figure(
  image("rk4-p2.png"),
  caption: [En azul se muestra la solución analítica $3/2 e^x - 1/2 sin(x)$, y en rojo se muestran los puntos $(t, u_1(t))$ de la @t:1.],
)


#question[
  La siguiente tabla muestra la evolución de una enfermedad contagiosa en
  función del tiempo $t$ en días. Aquí, $c$ indica el numero de contagiados.
  #figure(table(
    columns: 5,
    $t$, $0$, $3$, $5$, $7$,
    $c$, $1$, $20$, $22$, $23$,
  ))
  1. Aproximar, mediante interpolación con el método de Lagrange el valor de
    $c(4)$.
  2. En la tabulación de $c(t)$ se observa que dicha función parece saturarse a
    medida que pasa el tiempo, es decir, presenta una asíntota horizontal
    cercana a $c approx 22$. Explica entonces por que el polinomio obtenido en
    el apartado anterior no resulta adecuado para aproximar valores de $c$ en
    tiempos superiores a $7$.
  3. Una forma de resolver el problema del inciso anterior consiste en ajustar
    los datos de la tabulación en un modelo de crecimiento logístico. Para
    este ejemplo, se propone, $ c(t) = 1/(alpha + beta exp(-3t)). $ Calcula el
    valor de los parámetros $alpha$ y $beta$ mediante el método de mínimos
    cuadrados. Para esto te sugiero re tabular los valores de la tabla
    calculando $1/c(t)$ y hacer lo mismo con la función $c(t)$.
  4. Grafica el polinomio interpolador de Lagrange comparándolo con $c(t)$ en
    el intervalo $t in [0,10]$.
]

1. Ejecutamos nuestro programa con
```
make run-p3 ARGS="ex/p3/z.txt ex/p3/z.txt ex/p3/z.txt"
```
Esto nos genera el valor de $c(t) = 21.58$.

2. Para cualquier polinomio $P(x) = a_0 + a_1x + dots.c + a_n x^n$ no constante, se tiene
$
  lim_(abs(x)->infinity) abs(P(x)) = infinity.
$
Entonces, si nuestra función objetivo es algo que no va a infinito, es claro
que un polinomio no la va a aproximar al infinito. En este caso, para $t > 7$.

3. Haremos la re tabulación sugerida:
  #figure(
    table(
      columns: 5,
      $t$, $0$, $3$, $5$, $7$,
      $1/c$, $1$, $1/20$, $1/22$, $1/23$,
    ),
    caption: [Re tabulación con valores inversos.],
  )<t:2>
  Entonces, la función reciproca de la logística es
  $
    1/c(t) = alpha + beta e^(-3t).
  $
  Las funciones base son entonces ${1, e^(-3t)}$. Es decir, para el conjunto de entrenamiento dado por @t:2, generamos el sistema de ecuaciones
  $
    vec(alpha, beta) = (mat(1, e^(-3t_0); 1, e^(-3t_1); 1, e^(-3t_2); 1, e^(-3t_3);)^(top)
      mat(1, e^(-3t_0); 1, e^(-3t_1); 1, e^(-3t_2); 1, e^(-3t_3);))^(-1)
    mat(1, e^(-3t_0); 1, e^(-3t_1); 1, e^(-3t_2); 1, e^(-3t_3);)^(top)
    vec(c_0, c_1, c_2, c_3)
  $
  donde $t_i$ son los valores de $t$ en la @t:2, y los $c_i$ son los valores de $c$ en la misma tabla.

  Usando nuestro programa con
  ```
  make run-p3-b ARGS="ex/p3b/t.txt ex/p3b/c.txt"
  ```
  Obtenemos que los coeficientes son
  $
    alpha & = 0.0462715, \
     beta & = 0.953729.
  $
  Es decir, la función logística que mejor aproxima nuestros datos es
  $
    c(t) = 1/(0.0462715 + 0.954729 e^(-3t))
  $

4. Usando
```
make run-p3-d ARGS="ex/p3b/t.txt ex/p3b/c.txt"
```
Obtenemos los coeficientes
#let p3-d-coeff = csv("Code/ex/p3d/coeffs.csv")
#figure(
  table(
    columns: 1,
    table.header("Coefficientes"),
    ..p3-d-coeff.flatten(),
  ),
  caption: [Re tabulación con valores inversos.],
)
Es decir, tenemos el polinomio
$
  1 + 11.5512 x - 2.14286 x^2 + 0.134524 x^3.
$
#figure(
  image("p3-d.png"),
  caption: [Comparación de la curva logística y polinomio de Lagrange.],
)



#question[
  Hallar el mínimo de la siguiente función en el dominio $(x,y)in [-1, -2/5] times [-3/5, 3/5]$
  $
    f(x,y) = x e^(-x^2-y^2)
  $
  Elige un de los métodos de minimización estudiados para hallar el mínimo de
  la función, comenzar con el $(-1/2,-1/2)$. Tu defines la tolerancia. ¿Cuantas
  iteraciones se alcanzaron? Explica claramente el por que.
]

Los extremos de la función se encuentran en los valores $x in RR^2$ tal que $nabla f(x,y) = 0$. Entonces, para minimizar la función,
resolvemos el sistema de ecuaciones no-lineales
$
  (partial f)/(partial x) & = e^(-(x^2+y^2)) - 2x^2e^(-(x^2+y^2)) & = 0, \
  (partial f)/(partial y) & = -2x y e^(-(x^2+y^2))                & = 0.
$
Usaremos el método de Broyden con tolerancia de $1times 10^(-12)$. Usando

```
make run-p4
```
Obtenemos la siguiente tabla de iteraciones en `ex/p4/f.csv`:
#let broyden-p4 = csv("Code/ex/p4/f.csv", row-type: array).slice(1)

#figure(
  table(
    stroke: none,
    columns: 3,
    table.hline(),
    table.header($x$, $y$, $norm(x-x_("prev"))$),
    table.hline(),
    ..broyden-p4.flatten(),
    table.hline(),
  ),
  caption: [Iteraciones de Broyden para encontrar el cero del gradiente.],
)

Dado que el método de Broyden no se auto corrige como el método de Newton, debemos ajustar el paso máximo que
puede tomar cada iteración. Tomando el paso máximo como 0.5 y reescalando aquellos que lo sobrepasen, el método
converge en 7 iteraciones.

A comparación del método de Newton --- que converge cuadrática mente ---, el
método de Broyden converge de manera supe lineal, pero requiere de menos
cálculos aritméticos. Si adicionamos la mejora de Sherman-Morrison, la cantidad
es aun menor. Esto quiere decir que posiblemente el método de Broyden converge
mas lentamente.
