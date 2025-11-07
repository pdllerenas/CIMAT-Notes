#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/cetz:0.4.2": canvas, draw
#import "@preview/cetz-plot:0.1.3": plot
#import "@preview/lilaq:0.5.0" as lq
#let question = homework.complex-question

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 10",
  date: datetime.today(),
)
#set par(
  justify: true,
  leading: 0.52em,
)

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
  Crear un codigo en `C++` que resuelva el problema de minimos cuadrados en el
  sentido de interpolacion numerica para:
  1. Un polinomio interpolador de $n+1$ puntos de la forma $P(x) = a_0 + a_1 x
     + dots.h.c + a_n x^n$ para una funcion $f(x)$.
  2. Una funcion interpoladora trigonometrica de $n+1$ puntos para $f(x)$ de la
     forma $ F_"trig" (x) = a_0 cos(0 (pi x)/6) + a_1cos(1 (pi x)/6) + dots.h.c
     + a_n cos(n (pi x)/6). $ 
  3. Una funcion interpoladora de base radial de $n+1$ puntos para $f(x)$ de la
     forma $ F_"RBF" (x) = a_0 e^(-r^2_0) + a_1 e^(-r^2_1) + dots.h.c + a_n
     e^(r^2_n), $ donde $r = (x-x_i) $ para $i = 0,dots,n$.

     *Nota*: Recordemos que el problema de minimos cuadrados "regularizado" resuelve el sistema:

     - Conjunto de entrenamiento 
     $
      cal(T) = lr({(bold(x)^((k)), y^((k)))}, size:#110%)_(k=1)^p, quad
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
     bold(Lambda) = mat(lambda_1, , , ;
     , lambda_2, , ,;
     , , dots.down, ;
     , , , lambda_m;
   ),quad
     $ 
     Encontrar los valores $lambda_i$ es todo un problema de optimización, para
     nuestros fines consideremos que los valores son constantes, es decir,
     $lambda = lambda_i, forall i$
]

#question[
  Para evaluar los códigos del ejercicio 1, consideremos la función de Sutherland que define la viscosidad de un gas según la temperatura $T$ en Kelvin (entre 1000), definida
  por $f(x) = m_0 ((1000T)/T_0)^(3/2)((T_0 + S_u)/(1000T + S_u))$. Calculemos la viscosidad del oxígeno $O_2$, para ello usemos $m_0 = 1.919e^(-2)$, $T_0 = 273$, y $S_u = 139$.
  Realiza lo siguiente:
  1. Evalúa los puntos $T_i = [0.273, 0.303, 0.323, 0.353, 0.423, 0.573,
     1.473]$ en la función de Sutherland. Los puntos $T_i$ corresponden a los
     $n+1$ puntos $x_i$, y las evaluaciones son los valores $y_i$ (estamos
     simulando que se ha realizaod un experimento de laboratorio físico, es
     decir, sólo tenemos mediciones experimentales).
  2. Usando el valor de $lambda = 0, 1e^(-5), 1e^(-7)$, para $0 <= i <= n$,
     aplica las 3 funciones de interpolacion programadas obtenidas del metodo
     de minimos cuadrados en el inciso 1, con ellos calcua la viscosidad para
     $T=1.2$, compara con el valor real obtenido directamente con la funcion de
     Sutherland.
  3. Grafica las soluciones obtenidas en el intervalo $[0.273, 1.5]$, usa incrementos de $~0.05$.
  4. Repide los incisos a), b) y c), pero con los nodos $T_i = [0.273, 0.473, 0.673, 0.873, 0.1073, 0.1273, 1.473]$.
  ¿Qué puedes concluir de los nodos y el comportamiento de las funciones de interpolación según $lambda$?
]

#question[
  Crear una  librería en `C++` que calcule la aproximacion a una integral en e intervalo $[a,b]$  arbitrario con los metodos de Newton-Cotes abierto $(n=0,1,2,3)$,
  Newton-Cotes cerrado ($n=1,2,3,4$), y cuadratura gaussiana $(n=1,2,3,4,5)$. Con las librerias aproxima las siguientes integrales:
  $ 
  integral_0^(pi/4) sin(x) dif x, quad integral_1^(1.5) x^2 ln x dif x, quad integral_0^1 x^2 e^(-x) dif x,
  $ 
  *Nota 1*: Crea una tabla comparativa de los resultados. 
  *Nota 2*: La primer integral es la vista en clase.
]

#question[
  Investiga el método de extrapolación Richardson, y el método de Romberg, presenta un ejemplo. No olvides referenciar tu investigación.
]
