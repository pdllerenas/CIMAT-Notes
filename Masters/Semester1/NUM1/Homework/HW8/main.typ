#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/cetz:0.4.2": canvas, draw
#import "@preview/cetz-plot:0.1.3": plot
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 7",
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
  Crear un código en `C++` para calcular el valor el polinomio de Lagrange de grado
  $n$ en el punto $x$ generado por $n+1$ puntos de prueba.
]

#question[
  Crear un código en `C++` para interpolar un valor $x$ de una función $f(x)$
  en $n+1$ puntos utilizando interpolación de Neville.
]

#question[
  Crear un código en `C++` para interpolar un valor $x$ de una función $f(x)$ en
  $n+1$ puntos utilizando interpolación progresiva por diferencias divididas de
  Newton.
]

#question[
  *Validación*. Con los códigos creados en los incisos 2), 3) y 4), realiza
  las interpolaciones correspondientes y llena la siguiente tabla. Utiliza la
  función $f(x) = e^x$ en los valores $0, 0.5, 1.0, 1.5, 2.0$ y sus evaluaciones
  como valores nodales de interpolación. Nota: Los errores son absolutos.
]

#table(
  columns: (auto, auto, auto, auto, auto, auto, auto, auto),
  $z$, $f(z)$, [I. Lagrange], [Error L.], [I. Neville], [Error N.], [I. Diferencias divididas], [Error DD],
  $0.4$, [], [], [], [], [], [], [],
  $0.8$, [], [], [], [], [], [], [],
  $1.2$, [], [], [], [], [], [], [],
  $1.6$, [], [], [], [], [], [], [],
  $1.9$, [], [], [], [], [], [], [],
)

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


