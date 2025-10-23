#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Métodos Numericos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 7",
  date: datetime.today(),
)

#set enum(numbering: "a)")
#question[
  Crear un código en `C` para evaluar un valor $z$ de un polinomio de Taylor de
  grado $n$ centrado en $x_0$.
  - El programa debe ingresar el valor $z$, $x_0$ y las $n$ derivadas de la
    función en $x_0$, y debe regresar el valor de la evaluación $P_n(z)$.
  - Prueba tu programa con la función $f(x) = e^x$ sobre el intervalo $[0,2]$,
    con $x_0 = 0$.
  - Evalúa los puntos $z = 0.5, 1,1.5, 2.0$, presenta los errores absolutos
  de aproximación con el valor real $f(z) = e^z$. Usa $n = 1,3,5,10$ para calcular
  $P_n(z)$.
]

#question[
  Crear un codigo en `C` para calcular el valor el polinomio de Lagrange de grado
  $n$ en el punto $x$ generado por $n+1$ puntos de prueba.
]

#question[
  Crear un codigo en `C` para interpolar un valor $x$ de una función $f(x)$
  en $n+1$ puntos utilizando interpolación de Neville.
]

#question[
  Crear un codigo en `C` para interpolar un valor $x$ de una función $f(x)$ en
  $n+1$ puntos utilizando interpolación progresiva por diferencias divididas de
  Newton.
]

#question[
  *Validación*. Con los códigos creados en los incisos 2), 3) y 4), realiza
  las interpolaciones correspondientes y llena la siguiente tabla. Utiliza la
  función $f(x) = e^x$ en los valores $0, 0.5, 1.0, 1.5, 2.0$ y sus evaluaciones
  como valores nodales de interpolación. Nota: Los errores son aboslutos
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
  voltage (V) vs densidad de corriente (*I*). Un electroquímico experimental
  tomó observaciones a distintas densidades de corriente y los presentóen la
  siguiente tabla.

  Su jefe le solicida conocer los voltajes para $I = 0.0075, 0.0125$ y $0.0175$ $A c m^(-2)$.
  Observa que él no tomó medidas a esos valores, entonces debe realizar nuevamente el
  experimento. Con ayuda de tus habilidades aprendidas en esta tarea ayuda
  al electroquímico a no tener que realizar nuevamente el experimento.

  #table(
    columns: (auto, auto, auto, auto, auto, auto, auto),
    $I (A slash c m ^2)$, $0.001$, $0.005$, $0.010$, $0.015$, $0.019$, $0.022$,
    [Voltage (V)], $0.7252$, $0.62625$, $0.57969$, $0.53438$, $0.43906$, $0.28125$
  )

  - Elige alguno de los métodos de los incisos 2), 3) y 4) para hallar los
  valores de voltage que necesita el electroquímico y que le ayudarán  a no tener
  que realizar el experimento nuevamente.
  - Grafica la curva de polarización (I vs V) de la tabla de datos experimentales
  y añade los valores interpolados.
  - Reflexión: ¿Qué puedes decir del método o la solución?
]


