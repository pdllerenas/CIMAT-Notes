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
  Crear un código en `C++` que resuelva el problema de mínimos cuadrados en el
  sentido de interpolación numérica para:
  1. Un polinomio interpolador de $n+1$ puntos de la forma $P(x) = a_0 + a_1 x
    + dots.h.c + a_n x^n$ para una función $f(x)$.
  2. Una función interpoladora trigonométrica de $n+1$ puntos para $f(x)$ de la
    forma $ F_"trig" (x) = a_0 cos(0 (pi x)/6) + a_1cos(1 (pi x)/6) + dots.h.c
    + a_n cos(n (pi x)/6). $
  3. Una función interpoladora de base radial de $n+1$ puntos para $f(x)$ de la
    forma $ F_"RBF" (x) = a_0 e^(-r^2_0) + a_1 e^(-r^2_1) + dots.h.c + a_n
    e^(r^2_n), $ donde $r = (x-x_i)$ para $i = 0,dots,n$.

    *Nota*: Recordemos que el problema de mínimos cuadrados "regularizado" resuelve el sistema:

    - Conjunto de entrenamiento
    $
      cal(T) = lr({(bold(x)^((k)), y^((k)))}, size: #110%)_(k=1)^p, quad
      y = f(bold(x)) = sum_(i=1)^m omega_i phi_i (bold(x)), quad
      Phi = (phi_1, phi_2, dots,phi_n)
    $
    $
      Phi^T Phi + Lambda bold(omega)^* = Phi^T y\
      bold(omega)^* = (Phi^T Phi + Lambda)^(-1) Phi^T y
    $

    $
      bold(omega)^* = vec(omega_1^*, omega^*_2, dots.v, omega_m^*),quad
      bold(phi)_j = vec(phi.alt_j (bold(x)^(1)), phi.alt_j (bold(x)^(2)), dots.v, phi.alt_j (bold(x)^(p))),quad
      bold(y) = vec(y_1, y_2, dots.v, y_m),quad
      bold(Lambda) = mat(
        lambda_1, , , ;
        , lambda_2, , , ;
        , , dots.down, ;
        , , , lambda_m;
      ),quad
    $
    Encontrar los valores $lambda_i$ es todo un problema de optimización, para
    nuestros fines consideremos que los valores son constantes, es decir,
    $lambda = lambda_i, forall i$
]

1. Para aproximar un conjunto de datos ${(x_i,y_i)}_(1<=i<=m)$ con un polinomio algebraico
$
  P_n (x) = a_n x^n + a_(n-1) x^(n-1) + dots.c + a_1x + a_0
$
de grado $n < m-1$, por medio del procedimiento de mínimos cuadrados se deben seleccionar constantes
$a_0, a_1,dots, a_n$ para minimizar
$
  E=sum_(i=1)^m (y_i - P_n (x_i))^2.
$
Es decir, necesitamos $(partial E)/(partial a_j) = 0$.Esto se reduce a las $n+1$ ecuaciones normales, que se escriben
de la siguiente manera:
$
  a_0 sum_(i=1)^m x_i^0 + a_1 sum_(i=1)^m x_i^1+a_2 sum_(i=1)^m x_i^2+dots.c+a_n sum_(i=1)^m x_i^n &=& sum_(i=1)^m y_i x_i^0,\
  a_0 sum_(i=1)^m x_i^1 + a_1 sum_(i=1)^m x_i^2+a_2 sum_(i=1)^m x_i^3+dots.c+a_n sum_(i=1)^m x_i^(n+1) &=& sum_(i=1)^m y_i x_i^1,\
  &dots.v&\
  a_0 sum_(i=1)^m x_i^n + a_1 sum_(i=1)^m x_i^(n+1)+a_2 sum_(i=1)^m x_i^(n+3)+dots.c+a_n sum_(i=1)^m x_i^(2n) &=& sum_(i=1)^m y_i x_i^n,\
$
Entonces, debemos de solucionar el sistema de ecuaciones lineal simétrico. Para
esto, usamos Cholesky.

2. De manera similar a la explicación anterior, con $x |-> phi_i (x)$, el
  problema se reduce a solucionar un sistema de ecuaciones, que de igual
  manera resolvemos con Cholesky.

#question[
  Para evaluar los códigos del ejercicio 1, consideremos la función de
  Sutherland que define la viscosidad de un gas según la temperatura $T$ en
  Kelvin (entre 1000), definida por $ f(x) = m_0 ((1000T)/T_0)^(3/2)((T_0 +
    S_u)/(1000T + S_u)). $ Calculemos la viscosidad del oxígeno $O_2$, para ello
  usemos $m_0 = 1.919e^(-2)$, $T_0 = 273$, y $S_u = 139$.
  Realiza lo siguiente:
  1. Evalúa los puntos $T_i = [0.273, 0.303, 0.323, 0.353, 0.423, 0.573,
      1.473]$ en la función de Sutherland. Los puntos $T_i$ corresponden a los
    $n+1$ puntos $x_i$, y las evaluaciones son los valores $y_i$ (estamos
    simulando que se ha realizado un experimento de laboratorio físico, es
    decir, sólo tenemos mediciones experimentales).
  2. Usando el valor de $lambda = 0, 1e^(-5), 1e^(-7)$, para $0 <= i <= n$,
    aplica las 3 funciones de interpolación programadas obtenidas del método
    de mínimos cuadrados en el inciso 1, con ellos calcula la viscosidad para
    $T=1.2$, compara con el valor real obtenido directamente con la función de
    Sutherland.
  3. Grafica las soluciones obtenidas en el intervalo $[0.273, 1.5]$, usa incrementos de $~0.05$.
  4. Repite los incisos a), b) y c), pero con los nodos $T_i = [0.273, 0.473,
      0.673, 0.873, 1.073, 1.273, 1.473]$. ¿Qué puedes concluir de los nodos y
    el comportamiento de las funciones de interpolación según $lambda$?
]

1. Al evaluar los puntos $T_i$, obtenemos la siguiente tabla:
#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($x_i$, $y_i$),
    table.hline(),
    $0.273$, $0.01919$,
    $0.303$, $0.0209156$,
    $0.323$, $0.0220237$,
    $0.353$, $0.0236279$,
    $0.423$, $0.0271333$,
    $0.573$, $0.033766$,
    $1.473$, $0.0614706$,
    table.hline(),
  ),
  caption: [Valores de la función Sutherland en los puntos dados.],
)

2. Para obtener los valores de cada interpolación, usamos
```
make run-p2 ARGS="ex/2/init/Ti1.txt ex/2/init/lambda.txt"
```
Esto nos genera la tabla anterior, y las siguientes tablas, en el directorio
`ex/2/inter/`.

#let data_poly = csv("Code/ex/2/inter/poly1.csv", row-type: array).slice(1)
#let data_cos = csv("Code/ex/2/inter/cos1.csv", row-type: array).slice(1)
#let data_rad = csv("Code/ex/2/inter/rad1.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $bold(lambda)$, [Valor], [Error Absoluto]),
    table.hline(),
    ..data_poly.flatten(),
    table.hline(),
  ),
  caption: [Valores de interpolación polinomial en $T = 1.2$ para distintos
    valores de $lambda$.  ],
)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $bold(lambda)$, [Valor], [Error Absoluto]),
    table.hline(),
    ..data_cos.flatten(),
    table.hline(),
  ),
  caption: [Valores de interpolación con cosenos en $T = 1.2$ para distintos
    valores de $lambda$.  ],
)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $bold(lambda)$, [Valor], [Error Absoluto]),
    table.hline(),
    ..data_rad.flatten(),
    table.hline(),
  ),
  caption: [Valores de interpolación con funciones radiales en $T = 1.2$ para distintos
    valores de $lambda$.  ],
)

3.
#figure(image("poly_inter.png"))
#figure(image("cos_inter.png"))
#figure(image("rad_inter.png"))

#pagebreak()
4.
#let data_eval = csv("Code/ex/2/sutherland/eval.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $S(T)$),
    table.hline(),
    ..data_eval.flatten(),
    table.hline(),
  ),
  caption: [Valores de Sutherland en $T_i$],
)

```
make run-p2 ARGS="ex/2/init/Ti2.txt ex/2/init/lambda.txt"
```
Esto nos genera la tabla anterior, y las siguientes tablas, en el directorio
`ex/2/inter/`.

#let data_poly = csv("Code/ex/2/inter/poly.csv", row-type: array).slice(1)
#let data_cos = csv("Code/ex/2/inter/cos.csv", row-type: array).slice(1)
#let data_rad = csv("Code/ex/2/inter/rad.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $bold(lambda)$, [Valor], [Error Absoluto]),
    table.hline(),
    ..data_poly.flatten(),
    table.hline(),
  ),
  caption: [Valores de interpolación polinomial en $T = 1.2$ para distintos
    valores de $lambda$.  ],
)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $bold(lambda)$, [Valor], [Error Absoluto]),
    table.hline(),
    ..data_cos.flatten(),
    table.hline(),
  ),
  caption: [Valores de interpolación con cosenos en $T = 1.2$ para distintos
    valores de $lambda$.  ],
)

#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header($bold(T)$, $bold(lambda)$, [Valor], [Error Absoluto]),
    table.hline(),
    ..data_rad.flatten(),
    table.hline(),
  ),
  caption: [Valores de interpolación con funciones radiales en $T = 1.2$ para distintos
    valores de $lambda$.  ],
)

#figure(image("poly_inter2.png"))
#figure(image("cos_inter2.png"))
#figure(image("rad_inter2.png"))


#question[
  Crear una  librería en `C++` que calcule la aproximación a una integral en e
  intervalo $[a,b]$  arbitrario con los métodos de Newton-Cotes abierto
  $(n=0,1,2,3)$, Newton-Cotes cerrado ($n=1,2,3,4$), y cuadratura gaussiana
  $(n=1,2,3,4,5)$. Con las librerías aproxima las siguientes integrales:
  $
    integral_0^(pi/4) sin(x) dif x, quad integral_1^(1.5) x^2 ln x dif x, quad integral_0^1 x^2 e^(-x) dif x,
  $
  *Nota 1*: Crea una tabla comparativa de los resultados.
  *Nota 2*: La primer integral es la vista en clase.
]

Las formulas de Newton-Cotes abiertas son aquellas que no toman los limites del
intervalo en cuanta para la aproximación, mientras que las cerradas si. Esto
puede ser una desventaja o ventaja, dependiendo de la función que es integrada.
La cuadratura de Gauss utiliza los polinomios de Legendre como aproximaciones a
la función integrada, donde primero se hace una transformación al intervalo
$[-1,1]$, que es donde los polinomios de Legendre son ortogonales. Se usa una
tabla pre-cargada para hacer las estimaciones de las raíces y coeficientes
correspondientes de cada $n$ (ver `integration/gaussian.hpp`).

Para ejecutar los códigos, usamos
```
make run-p3-a
make run-p3-b
make run-p3-c
```
Esto nos genera 9 tablas en los directorios `ex/sin`, `ex/xln` y `ex/xexp`, que
presentamos a continuación:

#let integral_sin_open = csv("Code/ex/sin/open.csv", row-type: array).slice(1)
#let integral_sin_closed = csv("Code/ex/sin/closed.csv", row-type: array).slice(1)
#let integral_sin_gaussian = csv("Code/ex/sin/quadrature.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_0^(pi/4) sin(x)$),
    table.hline(),
    ..integral_sin_open.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de Newton-Cotes abierto],
)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_0^(pi/4) sin(x)$),
    table.hline(),
    ..integral_sin_closed.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de Newton-Cotes cerrado],
)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_0^(pi/4) sin(x)$),
    table.hline(),
    ..integral_sin_gaussian.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de cuadratura gaussiana],
)

#let integral_xln_open = csv("Code/ex/xln/open.csv", row-type: array).slice(1)
#let integral_xln_closed = csv("Code/ex/xln/closed.csv", row-type: array).slice(1)
#let integral_xln_gaussian = csv("Code/ex/xln/quadrature.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_1^(1.5) x^2 ln(x)$),
    table.hline(),
    ..integral_xln_open.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de Newton-Cotes abierto],
)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_1^(1.5) x^2 ln(x)$),
    table.hline(),
    ..integral_xln_closed.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de Newton-Cotes cerrado],
)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_1^(1.5) x^2 ln(x)$),
    table.hline(),
    ..integral_xln_gaussian.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de cuadratura gaussiana],
)

#let integral_xexp_open = csv("Code/ex/xexp/open.csv", row-type: array).slice(1)
#let integral_xexp_closed = csv("Code/ex/xexp/closed.csv", row-type: array).slice(1)
#let integral_xexp_gaussian = csv("Code/ex/xexp/quadrature.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_0^(1) x^2 exp(-x)$),
    table.hline(),
    ..integral_xexp_open.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de Newton-Cotes abierto],
)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_0^(1) x^2 exp(-x)$),
    table.hline(),
    ..integral_xexp_closed.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de Newton-Cotes cerrado],
)

#figure(
  table(
    columns: 2,
    stroke: none,
    table.hline(),
    table.header($n$, $integral_0^(1) x^2 exp(-x)$),
    table.hline(),
    ..integral_xexp_gaussian.flatten(),
    table.hline(),
  ),
  caption: [Aproximaciones por medio de cuadratura gaussiana],
)



#question[
  Investiga el método de extrapolación Richardson, y el método de Romberg, presenta un ejemplo. No olvides referenciar tu investigación.
]

#set math.equation(numbering: "(1)")
== Extrapolación de Richardson
La _extrapolación de Richardson_, atribuida a
#link("https://en.wikipedia.org/wiki/Lewis_Fry_Richardson")[Lewis Fry
  Richardson] (1881-1953) por sus artículos @ric1, @ric2, consiste en
combinar diversas aproximaciones del valor $alpha_0$ para obtener una
aproximación más precisa de $alpha_0$.

Específicamente, sea $A(h)$ una función que aproxima el valor de $alpha_0$
para todo $h eq.not 0$. Asumimos que para algún $k >= 0$, podemos expandir
la función $A(h)$ de la siguiente manera:
$
  A(h) = alpha_0 + alpha_1 h + dots.h.c + alpha_k h^k + R_(k+1) (h),
$ <def>
donde $abs(R_(k+1) (h)) <= C_(k+1) h^(k+1)$. Las constantes $C_(k+1)$ y los
coeficientes $alpha_i$ no dependen de $h$, por lo que
$
  alpha_0 = lim_(h->0) A(h).
$
Para $delta in (0,1)$, reescribimos @def con $h |-> delta h$:
$
  A(delta h) = alpha_0 + alpha_1 delta h + dots.h.c + alpha_k delta^k h^k + R_(k+1) (delta h).
$ <transform>
Ahora, haciendo $delta$ multiplicado por @def menos @transform, obtenemos
$
  B(h) = alpha_0 + tilde(alpha)_2 h^2 + dots.h.c + tilde(alpha)_k h^k + tilde(R)_(k+1) (delta h),
$
para $i = 2,dots, k$, definimos $tilde(alpha)_i = alpha_i (delta^i - delta)/ (1-delta)$ y
$
  tilde(R)_(k+1) (h) = (R_(k+1) (delta h) - delta R_(k+1) (h))/(1-delta).
$
Con esta construcción, tenemos que $tilde(alpha)_i eq.not 0 <=> alpha_i$. En
particular, si $alpha_1 eq.not 0$, entonces $A(h)$ es una aproximación de
primer orden de $alpha_0$, mientras que $B(h)$ es de orden al menos 2. En general,
si $A(h)$ es una aproximación de $alpha_0$ de orden $p$, entonces el valor
$
  B(h) = (A(delta h) - delta^p A(h))/(1-delta^p)
$
aproxima a $alpha_0$ a un orden de al menos $p+1$.

Por inducción, generamos el siguiente algoritmo de extrapolación de Richardson:
Sea $n>= 0$, $h >0$ y $delta in (0,1)$, construimos la secuencia
$
    A_(m,0) & = A(delta^m h),                                      & m & = 0,dots,n, \
  A_(m,q+1) & = (A_(m,q) - delta^(q+1) A_(m-1,q))/(1-delta^(q+1)), & q & = 0,dots, n-1, \
            &                                                      & m & = q+1, dots,n,
$
que se puede representar mediante el siguiente diagrama  @Quarteroni_Sacco_Saleri_2000,

#figure(
  diagram(
    node((0, 0), $A_(0,0)$, name: <A00>),
    node((1, 1), $A_(1,1)$, name: <A11>),
    node((2, 2), $A_(2,2)$, name: <A22>),
    node((3, 3), $A_(3,3)$, name: <A33>),
    node((4, 4), $dots.down$, name: <A44>),
    node((5, 5), $A_(n,n)$, name: <A55>),
    edge(<A00>, "->", <A11>),
    edge(<A11>, "->", <A22>),
    edge(<A22>, "->", <A33>),
    edge(<A33>, "->", <A44>),
    edge(<A44>, "->", <A55>),
    node((0, 1), $A_(1,0)$, name: <A10>),
    node((0, 2), $A_(2,0)$, name: <A20>),
    node((0, 3), $A_(3,0)$, name: <A30>),
    node((0, 4), $dots.down$, name: <A40>),
    node((0, 5), $A_(n,0)$, name: <A50>),
    edge(<A11>, "<-", <A10>),
    edge(<A21>, "<-", <A20>),
    edge(<A31>, "<-", <A30>),
    edge(<A51>, "<-", <A50>),
    node((1, 2), $A_(2,1)$, name: <A21>),
    node((1, 3), $A_(3,1)$, name: <A31>),
    node((1, 4), $dots.down$, name: <A41>),
    node((1, 5), $A_(n,1)$, name: <A51>),
    node((2, 3), $A_(3,2)$, name: <A32>),
    node((2, 4), $dots.down$, name: <A42>),
    node((2, 5), $A_(n,2)$, name: <A52>),
    edge(<A22>, "<-", <A21>),
    edge(<A31>, "->", <A32>),
    edge(<A32>, "->", <A33>),
    node((3, 5), $A_(n,3)$, name: <A53>),
    edge(<A51>, "->", <A52>),
    edge(<A52>, "->", <A53>),
    edge(<A53>, "->", <A54>),
    node((4, 5), $dots.c$, name: <A54>),
    node((3, 4), $dots.down$, name: <A34>),
    edge(<A54>, "->", <A55>),
    edge(<A10>, "->", <A21>),
    edge(<A20>, "->", <A31>),
    edge(<A30>, "->", <A41>),
    edge(<A40>, "->", <A51>),

    edge(<A21>, "->", <A32>),
    edge(<A20>, "->", <A31>),
    edge(<A31>, "->", <A42>),
    edge(<A41>, "->", <A52>),

    edge(<A32>, "->", <A34>),
    edge(<A20>, "->", <A31>),
    edge(<A31>, "->", <A42>),
    edge(<A41>, "->", <A52>),
    edge(<A41>, "->", <A52>),
    edge(<A34>, "->", <A54>),
    edge(<A42>, "->", <A53>),
  ),
  caption: [Diagrama de recurrencia. Las flechas indican el orden en el que se construyen los términos.],
)

Se puede demostrar @Quarteroni_Sacco_Saleri_2000 que para $n>=0$ y $delta in (0,1)$,
$
  A_(m,n) = alpha_0 + cal(O)((delta^m h)^(n+1)), quad quad m = 0,dots, n.
$
En particular, para $n = 0$, la convergencia a $alpha_0$ es de $cal(O)(delta^m h)$.

=== Ejemplo 1.
Podemos emplear extrapolación de Richardson para aproximar la derivada de $f(x) = x e^(-x) cos(2x)$ en el punto $x=0$.
De acuerdo al proceso explicado, necesitamos una función que aproxime este valor. Proponemos
$
  A(h) = (f(x+h) - f(x))/h,
$
una elección obvia al ser la derivada la que intentamos aproximar. Tomemos $delta = 0.5$, $n = 5$ y $h = 0.1$. La siguiente tabla
nos presenta los errores absolutos $E_(m,k)$ del valor real $alpha_0$ y la aproximación $A_(m,k)$.
#figure(
  table(
    stroke: none,
    columns: 6,
    table.hline(),
    table.header($E_(m,0)$, $E_(m,1)$, $E_(m,2)$, $E_(m,3)$, $E_(m,4)$, $E_(m,5)$),
    table.hline(),
    $0.113$, $dash$, $dash$, $dash$, $dash$, $dash$,
    $5.3times 10^(-2)$, $6.1times 10^(-3)$, $dash$, $dash$, $dash$, $dash$,
    $2.6times 10^(-2)$, $1.7times 10^(-3)$, $2.2times 10^(-4)$, $dash$, $dash$, $dash$,
    $1.3times 10^(-2)$, $4.5times 10^(-4)$, $2.8times 10^(-5)$, $5.5times 10^(-7)$, $dash$, $dash$,
    $6.3times 10^(-2)$, $1.1times 10^(-4)$, $3.5times 10^(-6)$, $3.1times 10^(-8)$, $3.0times 10^(-9)$, $dash$,
    $3.1times 10^(-3)$,
    $2.9times 10^(-5)$,
    $4.5times 10^(-7)$,
    $1.9times 10^(-9)$,
    $9.9times 10^(-11)$,
    $4.9times 10^(-12)$,
    table.hline(),
  ),
)

== Metodo de Romberg
Se le atribuye a #link("https://en.wikipedia.org/wiki/Werner_Romberg")[Werner
  Romberg] por su articulo @Romberg1955 describiendo el método de integración.
Consiste en utilizar la extrapolación de Richardson para aproximar la integral
de una función $f:RR -> RR$ mediante el método del trapecio. Es decir, si
nuestra función de aproximación es el método del trapecio para aproximar la
integral, entonces podemos extrapolar una mejor aproximación.

#theorem[
  Sea $f in C^(2k+2)([a,b])$ para $k >= 0$. La aproximación de $alpha_0 = integral_a^b f(x) dif x$ dada por la regla del trapecio es
  $
    I_(1,m)(f) = alpha_0 + sum_(i=1)^k B_(2i)/((2i)!) h_m^(2i) (f^((2i-1)) (b) - f^((2i-1)) (a)) + B_(2k+2) /((2k+2)!) h_m^(2k+2) (b-a) f^((2k+2))(eta),
  $
  donde $h_m = (b-a)/m$ para $m>=1$, $eta in (a,b)$ y $ display(B_(2j) = (-1)^(j-1)lr([sum_(n=1)^infinity 2/((2n pi)^(2j))](2j)!)), quad "para "j>=1. $
]<trap>

Podemos notar que @trap es un caso especial de @def, usando $h = h_m^2$ y $A(h) = I_(1,m)(f)$. Entonces, el algoritmo de Richardson nos provee la
siguiente recursión:
$
    A_(m,0) & = A(delta^m h),                                            & m & = 0,dots,n, \
  A_(m,q+1) & = (A_(m,q) - delta^(2(q+1)) A_(m-1,q))/(1-delta^(2(q+1))), & q & = 0,dots, n-1, \
            &                                                            & m & = q+1, dots,n,
$<rr>
Usando $h = b-a$ y $delta = 0.5$ en @rr, con $s = 2^m$ sub-intervalos de longitud $h_s = (b-a)/2^m$, tenemos
$
    A_(m,0) & = I_(1,s)((b-a)/2^m),                       & m & = 0,dots,n, \
  A_(m,q+1) & = (4^(q+1)A_(m,q) - A_(m-1,q))/(4^(q+1)-1), & q & = 0,dots, n-1, \
            &                                             & m & = q+1, dots,n.
$
Entonces, tenemos la siguiente convergencia:
$
  A_(m,n) = integral_a^b f(x) dif x + cal(O)(h_s^(2(n+1))), quad n >= 0.
$
=== Ejemplo 2.
Aproximamos el valor de la integral $alpha_0 = integral_0^pi e^x cos(x) dif x = -(e^pi + 1)/2$. Obtenemos la siguiente tabla
#figure(
  table(
    stroke: none,
    columns: (auto, auto),
    align: (center, left),
    table.hline(),
    table.header($k$, $E_k$),
    table.hline(),
    $0$, $22.71$,
    $1$, $0.4775$,
    $2$, $5.926times 10^(-2)$,
    $3$, $7.410times 10^(-5)$,
    $4$, $8.923times 10^(-7)$,
    $5$, $6.850times 10^(-11)$,
    $6$, $5.330times 10^(-14)$,
    $7$, $0$,
    table.hline(),
  ),
)


#bibliography("ref.bib")
