#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/cetz:0.4.2": canvas, draw
#import "@preview/cetz-plot:0.1.3": plot
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 8",
  date: datetime.today(),
)

#question[
  Crear un código en `C++` para evaluar un valor $z$ de un polinomio de Taylor de
  grado $n$ centrado en $x_0$.
  #set enum(numbering: "a)")
  1. El programa debe ingresar el valor $z$, $x_0$ y las $n$ derivadas de la
    función en $x_0$, y debe regresar el valor de la evaluación $P_n (z)$.
  2. Prueba tu programa con la función $f(x) = e^x$ sobre el intervalo $[0,2]$,
    con $x_0 = 0$.
  3. Evalúa los puntos $z = 0.5, 1,1.5, 2.0$, presenta los errores absolutos de
    aproximación con el valor real $f(z) = e^z$. Usa $n = 1,3,5,10$ para
    calcular $P_n (z)$.
]

Dada una función $f:[a,b] -> RR$, $n$ veces diferenciable, su _polinomio de
  Taylor_ de grado $n$ centrado en el punto $x_0 in (a,b)$ está dado por $ P_n
(x) = f(x_0) + f^((1))(x_0)(x-x_0) + dots.h.c + f^((n))(x_0)((x-x_0)^n)/n!. $
En general, este polinomio aproxima la función localmente, por lo que si el
valor que tratamos de evaluar está fuera del radio de convergencia del
polinomio, el valor devuelto no corresponderá al valor real de la función.

Para compilar y ejecutar el programa, usamos
```
make run-p1 ARGS="0 ex/taylor/n.txt ex/taylor/z.txt ex/taylor/x0.txt"
```
Este comando evaluará los valores del archivo `ex/taylor/z.txt` en el polinomio
de Taylor centrado en $x_0 = 0$, y usará los valores de las $n$ derivadas de
$f(x) = e^x$ en $x_0$, para cada $n$ en el archivo `ex/taylor/n.txt`. Esto nos
imprimirá, para cada $n$, el resultado de evaluar el polinomio de Taylor en
cada punto dado. En el caso de este ejercicio, el programa nos genera el
archivo `ex/taylor/table.csv`, que se ve de la siguiente manera:

#let results = csv("Code/ex/taylor/table.csv", row-type: array).slice(1)

#let chunk-size = 8

#grid(
  columns: (1fr, 1fr),
  align: center,
  table(
    columns: 4,
    table.header($bold(n)$, $bold(z)$, $bold(P_n (z))$, $bold(|P_n (z) - e^z|)$),
    ..results.slice(0, 8).flatten(),
  ),
  table(
    columns: 4,
    table.header($bold(n)$, $bold(z)$, $bold(P_n (z))$, $bold(|P_n (z) - e^z|)$),
    ..results.slice(8).flatten(),
  ),
)

#figure(
  canvas({
    import draw: *
    set-style(
      axes: (stroke: .5pt, tick: (stroke: .5pt)),
      legend: (stroke: none, orientation: ttb, item: (spacing: .3), scale: 80%),
    )
    plot.plot(
      size: (10, 8),
      y-min: -1,
      y-max: 10,
      {
        plot.add(
          domain: (-5, 5),
          t => (t, calc.exp(t)),
          label: $e^x$,
        )
        plot.add(
          domain: (-5, 5),
          t => (t, 1),
          label: $P_1(x)$,
        )
        plot.add(
          domain: (-5, 5),
          t => (t, 1 + t + t * t / 2),
          label: $P_3(x)$,
        )
        plot.add(
          domain: (-5, 5),
          t => (t, 1 + t + t * t / 2 + calc.pow(t, 3) / 6 + calc.pow(t, 4) / (calc.fact(4))),
          label: $P_5(x)$,
        )
      },
    )
  }),
  caption: $e^x$ + " y sus primeros 3 polinomios de Taylor de grado impar.",
)

#question[
  Crear un código en `C++` para calcular el valor del polinomio de Lagrange de
  grado $n$ en el punto $x$ generado por $n+1$ puntos de prueba.
]

Dados $k+1$ nodos ${x_0, x_1, dots, x_n}$, todos distintos, la base de Lagrange
para polinomios de grado menor que $k$ es dada por el conjunto ${ell_0 (x),
  ell_1 (x), dots, ell_k (x)}$, cada uno de grado $k$. Cada uno toma los valores
$ell_j (x_m) = delta_(j m)$, donde $delta_(j m)$ denota la delta de Kronecker.

Explícitamente, estos se ven de la siguiente manera:
#show stack: set text(size: 7pt)
$
  ell_j (x) &= (x-x_0)/(x_j-x_0) dots.h.c (x-x_(j-1))/(x_j - x_(j-1)) dot (x-x_(j+1))/(x_j-x_(j+1)) dots.h.c (x-x_k)/(x_j-x_k)\
  &= product_(#stack(spacing: 4pt, $0<=m<=k$, $m != j$)) (x-x_m)/(x_j - x_m).
$

Para este ejercicio, interpolaremos el valor en $x = 1$, usando como ejemplo
los puntos $ (-2,2), (0,4), (2,3), (5,4). $
Por las definiciones anteriores, el polinomio que se generará tendrá la forma
$ P_3 (x) = 2ell_0 (x) + 4ell_1 (x) + 3ell_2 (x) + 4 ell_3 (x). $

Para ejecutar el código correspondiente, usamos
```
make run-p2 ARGS="ex/lagrange/z.txt ex/lagrange/x.txt ex/lagrange/y.txt"
```
Esto nos genera el archivo `ex/lagrange/table.txt`, que contiene los puntos insertados
por el archivo `ex/lagrange/z.txt`, y su respectiva evaluación en el polinomio
de Lagrange generado por los puntos dados. La tabla generada es la siguiente:

#let results_lagrange = csv("Code/ex/lagrange/table.csv", row-type: array).slice(1)

#grid(
  columns: (1fr, 1fr),
  align: center,
  table(
    columns: 2,
    table.header($bold(z)$, $bold(P_n (z))$),
    ..results_lagrange.slice(0, 5).flatten(),
  ),
  table(
    columns: 2,
    table.header($bold(z)$, $bold(P_n (z))$),
    ..results_lagrange.slice(5).flatten(),
  ),
)

Como podemos observar, el polinomio coincide con los puntos originalmente dados,
por lo que podemos concluir que el método funciona correctamente.

#question[
  Crear un código en `C++` para interpolar un valor $x$ de una función $f(x)$
  en $n+1$ puntos utilizando interpolación de Neville.
]

Dados $n+1$ puntos (con cada coordenada $x$ distinta), existe un único polinomio de grado a lo más $n$ que pasa por los puntos dados.
El algoritmo consiste en resolver la relación de recurrencia
#set math.cases(gap: 1em)
$
  p_(i,j) = cases(
    y_i "if" j = i " and " 0 <= i <= n,
    ((x-x_i)p_(i+1,j) (x) - (x-x_j) p_(i, j-1) (x))/(x_j - x_i) "if" 0<=i<j<=n
  )
$
El valor buscado es $p_(0,n)$. Para este ejercicio, usaremos el ejemplo de $f(x) = log(x)$, con los puntos
$ x_0 = 2.0, quad x_1 = 2.2,quad x_2 = 2.3. $
Las evaluaciones de estos puntos en $f$ son
$ f(x_0) = 0.6931, quad f(x_1) = 0.7885,quad f(x_2) = 0.8329 $
Para ejecutar nuestro programa, usamos
```
make run-p3 ARGS="ex/neville/z.txt ex/neville/x.txt ex/neville/y.txt"
```
Esto inserta los valores de $x$ y $f(x)$ dados. El archivo `z.txt` contiene los puntos a interpolar.
Esto nos genera el archivo `ex/neville/table.csv`, que se ve de la siguiente manera:

// #let results_neville = csv("Code/ex/neville/table.csv", row-type: array).slice(1)
#let results_neville = csv("Code/ex/neville/table.csv", row-type: array).slice(1)

#align(center, table(
  columns: 3,
  table.header($bold(z)$, $bold(P (z))$, $bold(|P(z) - log(z)|)$),
  ..results_neville.flatten(),
))

#question[
  Crear un código en `C++` para interpolar un valor $x$ de una función $f(x)$ en
  $n+1$ puntos utilizando interpolación progresiva por diferencias divididas de
  Newton.
]

Dados $n+1$ puntos con coordenadas $x$ distintas, la diferencias divididas se definen recursivamente mediante
$
  [y_k] &:= y_k, & k in {0,dots,n}\
  [y_k, dots, y_(k+j)] &:= ([y_(k+1), dots, y_(k+j)] - [y_k, dots, y_(k+j-1)])/(x_(k+j) - x_k) &#h(5em) k in {0,dots, n-j}, j in {1,dots,n}\
$

Algunas propiedades de estos objetos son linealidad e interpolación de polinomios. Es decir, si $P$ es una
función polinomial de grado a lo más $n$, y $p[x_0, dots, x_n]$ son las diferencias divididas, entonces
$
  P_(n-1) (x) = & p[x_0] \
                & +p[x_0, x_1](x-x_0) \
                & +p[x_0, x_1, x_2](x-x_0)(x-x_1) \
                & +dots.h.c+p[x_0, dots, x_n](x-x_0)(x-x_1) dots.h.c (x-x_(n-1))
$

Como ejemplo, tomaremos los puntos $ (1,2), (3,3), (4,2), (8,10). $

Corremos nuestro programa con el siguiente comando:
```
make run-p4 ARGS="ex/newton/z.txt ex/newton/x.txt ex/newton/y.txt"
```
Esto nos genera el archivo `ex/newton/table.csv`, que se ve de la siguiente manera:

#let results_newton = csv("Code/ex/newton/table.csv", row-type: array).slice(1)

#align(center, table(
  columns: 2,
  table.header($bold(z)$, $bold(P (z))$),
  ..results_newton.flatten(),
))

#question[
  *Validación*. Con los códigos creados en los incisos 2), 3) y 4), realiza
  las interpolaciones correspondientes y llena la siguiente tabla. Utiliza la
  función $f(x) = e^x$ en los valores $0, 0.5, 1.0, 1.5, 2.0$ y sus evaluaciones
  como valores nodales de interpolación. Nota: Los errores son absolutos.
]

#let results_all = csv("Code/ex/all/table.csv", row-type: array).slice(1)

#align(center, table(
  columns: 8,
  table.header($bold(z)$, $bold(f (z))$, [*I. Lagrange*], [*Error L.*], [*I. Neville*], [*Error N.*], [*I. Diferencias divididas*], [*Error DD*]),
  ..results_all.flatten(),
))
#align(center, table(
  columns: (auto, auto, auto, auto, auto, auto, auto, auto),
  $z$, $f(z)$, [I. Lagrange], [Error L.], [I. Neville], [Error N.], [I. Diferencias divididas], [Error DD],
  $0.4$, [], [], [], [], [], [], [],
  $0.8$, [], [], [], [], [], [], [],
  $1.2$, [], [], [], [], [], [], [],
  $1.6$, [], [], [], [], [], [], [],
  $1.9$, [], [], [], [], [], [], [],
))

#question[
  *Aplicación*. Consideremos el problema de un experimento para una celda de
  combustible de hidrógeno que produce electricidad. Una manera de verificar el
  rendimiento experimental es analizando la denominada curva de polarización,
  voltaje (V) vs densidad de corriente (*I*). Un electroquímico experimental
  tomó observaciones a distintas densidades de corriente y los presentó en la
  siguiente tabla.

  Su jefe le solicita conocer los voltajes para $I = 0.0075, 0.0125$ y $0.0175$ $A c m^(-2)$.
  Observa que él no tomó medidas a esos valores, entonces debe realizar nuevamente el
  experimento. Con ayuda de tus habilidades aprendidas en esta tarea ayuda
  al electroquímico a no tener que realizar nuevamente el experimento.

  #table(
    columns: (auto, auto, auto, auto, auto, auto, auto),
    $I (A slash c m^2)$, $0.001$, $0.005$, $0.010$, $0.015$, $0.019$, $0.022$,
    [Voltaje (V)], $0.7252$, $0.62625$, $0.57969$, $0.53438$, $0.43906$, $0.28125$,
  )

  - Elige alguno de los métodos de los incisos 2), 3) y 4) para hallar los
  valores de voltaje que necesita el electroquímico y que le ayudarán  a no tener
  que realizar el experimento nuevamente.
  - Grafica la curva de polarización (I vs V) de la tabla de datos experimentales
  y añade los valores interpolados.
  - Reflexión: ¿Qué puedes decir del método o la solución?
]


