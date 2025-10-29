#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/cetz:0.4.2": canvas, draw
#import "@preview/cetz-plot:0.1.3": plot
#import "@preview/lilaq:0.5.0" as lq
#let question = homework.complex-question

#show: pset.with(
  class: "Métodos Numéricos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 9",
  date: datetime.today(),
)
#set par(
  justify: true,
  leading: 0.52em,
)

#question[
  Consederemos la función $sin(x)$:
  - Genera un código basado en la interpolación de Hermite con diferencias
    divididas visto en clase.
  - Con los valores de la siguiente tabla, estima el valor de $sin(0.34)$
    utilizando interpolación de Hermite con diferencias divididas. Halla el
    error absoluto de la aproximación con el valor correcto.
    #align(center, table(columns: (auto, auto, auto), 
    $bold(x)$, $bold(sin x)$, $bold(D_x sin x = cos x)$,
    $0.30$, $0.29552$, $0.95534$,
    $0.32$, $0.31457$, $0.94924$,
    $0.35$, $0.34290$, $0.93937$,
  )
  )
  - A la tabla añade $sin(0.33) = 0.32404$ y $cos(0.33) = 0.94604$ y recalcula
    el valor de $sin(0.34)$. Halla el error absoluto, ¿Qué puedes concluir?
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

#question[
  Genera un código que realice interpolación cúbica natural. Puedes basarte en el algoritmo visto en clase. Con
  este algoritmo, recrea la parte superior de la imagen del pato dado por los puntos de la tabla:
#table(
  columns: 22,
  align: center,
  stroke: 0.5pt,
  inset: 5pt,
  table.header(
    [$x$], [0.9], [1.3], [1.9], [2.1], [2.6], [3.0], [3.9], [4.4], [4.7],
    [5.0], [6.0], [7.0], [8.0], [9.2], [10.5], [11.3], [11.6], [12.0], [12.6], [13.0], [13.3],
  ),
  [$f(x)$], [1.3], [1.5], [1.85], [2.1], [2.6], [2.7], [2.4], [2.15], [2.05],
  [2.1], [2.25], [2.3], [2.25], [1.95], [1.4], [0.9], [0.7], [0.6], [0.5], [0.4], [0.25],
)


#align(center, lq.diagram(
  title: [Dato del pato],
  xlim: (0,14),
  ylim: (-4,4),
  lq.plot(
    (0.9, 1.3, 1.9, 2.1, 2.6, 3.0, 3.9, 4.4, 4.7,5.0, 6.0, 7.0, 8.0, 9.2, 10.5, 11.3, 11.6, 12.0, 12.6, 13.0, 13.3), 
    (1.3, 1.5, 1.85, 2.1, 2.6, 2.7, 2.4, 2.15, 2.05, 2.1, 2.25, 2.3, 2.25, 1.95, 1.4, 0.9, 0.7, 0.6, 0.5, 0.4, 0.25)
  )
))
]
*Nota*: Para la interpolación puedes utilizar secciones o quitar puntos. La idea
es que se acerque a la forma adecuada. Sólo debes explicar tu procedimiento en
el reporte. Si alguno desea recrear al pato completo se le puede hacer llegar
los puntos a interpolar.

#question[
  Genera un código que realice interpolación cúbica fijo so sujeto, puedes basarte en el algoritmo visto en clase.
  Con este algoritmo recrea la parte superior de Snoopy, dados los puntos de la tabla.
]

#align(center, table(columns: 4, 
  $i$, $x_i$, $f(x_i)$, $f'(x_i)$,
  $0$, $1$, $3.0$, $1.0$,
  $1$, $1$, $3.0$, $1.0$,
  $2$, $1$, $3.0$, $1.0$,
  $3$, $1$, $3.0$, $1.0$,
  $4$, $1$, $3.0$, $1.0$,
  $5$, $1$, $3.0$, $1.0$,
  $6$, $1$, $3.0$, $1.0$,
  $7$, $1$, $3.0$, $1.0$,
  $8$, $1$, $3.0$, $1.0$,
))

#align(center, table(columns: 4, 
  $i$, $x_i$, $f(x_i)$, $f'(x_i)$,
  $0$, $1$, $3.0$, $1.0$,
  $1$, $1$, $3.0$, $1.0$,
  $2$, $1$, $3.0$, $1.0$,
  $3$, $1$, $3.0$, $1.0$,
  $4$, $1$, $3.0$, $1.0$,
  $5$, $1$, $3.0$, $1.0$,
  $6$, $1$, $3.0$, $1.0$,
))
