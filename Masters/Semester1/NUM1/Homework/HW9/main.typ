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
  Consideremos la función $sin(x)$:
  1. Genera un código basado en la interpolación de Hermite con diferencias
    divididas visto en clase.
  2. Con los valores de la siguiente tabla, estima el valor de $sin(0.34)$
    utilizando interpolación de Hermite con diferencias divididas. Halla el
    error absoluto de la aproximación con el valor correcto.
    #align(center, table(
      columns: (auto, auto, auto),
      stroke: none,
      table.hline(),
      $bold(x)$, $bold(sin x)$, $bold(D_x sin x = cos x)$,
      table.hline(),
      $0.30$, $0.29552$, $0.95534$,
      $0.32$, $0.31457$, $0.94924$,
      $0.35$, $0.34290$, $0.93937$,
      table.hline(),
    ))
  3. A la tabla añade $sin(0.33) = 0.32404$ y $cos(0.33) = 0.94604$ y recalcula
    el valor de $sin(0.34)$. Halla el error absoluto, ¿qué puedes concluir?
]

2.
Dada una función $f in C^(1)[a,b]$ y los puntos $x_0,dots, x_n in [a,b]$
todos distintos, el polinomio único de menor grado que conicide con $f$ y $f'$
en los puntos $x_0,dots,x_n$ es el polinomio de Hermite de grado a lo más $2n+1$
dado por
$ H_(2n+1) (x) = sum_(j=0)^n f(x_j) H_(n,j) (x) + sum_(j=0)^n f'(x_j) hat(H)_(n,j) $

donde

$
       H_(n,j) (x) & = [1 - 2(x - x_j) L'_(n,j) (x_j)] L^2_(n,j) (x) \
  hat(H)_(n,j) (x) & = (x-x_j) L^2_(n,j) (x),
$
y $L_(n,j) (x)$ es el $j-$ésimo coeficiente del polinomio de Lagrange de grado
$n$.

Si $f in C^(2n+2)[a,b]$, entonces
$
  f(x) = H_(2n+1) (x) + ((x-x_0)^2 dots.h.c (x-x_n)^2)/((2n+2)!)f^((2n+2)) (xi(x)),
$
para $xi(x)$ en el intervalo $(a,b)$.

Podemos reducir la complejidad de construcción de dichos polinomios con el uso
de diferencias divididas. Definimos la sucesión ${z_i}_(i = 0)^(2n+1)$, dada por

$
  z_(2i) = z_(2i+1) = x_i, quad forall i = 0,dots,n.
$

Construimos el polinomio de Hermite por

$
  H_(2n+1) (x) = f[z_0] + sum_(k = 1)^(2n+1) f[z_0, dots, z_k] (x-z_0)(x-z_1) dots (x-z_(k-1))
$

Para el ejemplo dado, queremos interpolar el valor $x = 0.34$ para $f(x) = sin(x)$.
Para ejecutar el programa, usamos

```
make run-p1 ARGS="0.34 ex/hermite/x.txt ex/hermite/fx.txt ex/hermite/dfx.txt"
```
Esto nos imprime en la consola
```
f(0.34) = 0.33348889007407406
sin(0.34) = 0.3334870921408144
abs_err = 1.7979332596618214e-06
```
Además, nos imprime los correspondientes coeficientes del polinomio generado,
en el archivo `ex/hermite/coeff.csv`.

3.
Ahora, si incluimos el punto adicional, tenemos la tabla
#align(center, table(
  columns: (auto, auto, auto),
  stroke: none,
  table.hline(),
  $bold(x)$, $bold(sin x)$, $bold(D_x sin x = cos x)$,
  table.hline(),
  $0.30$, $0.29552$, $0.95534$,
  $0.32$, $0.31457$, $0.94924$,
  $0.33$, $0.32404$, $0.94604$,
  $0.35$, $0.34290$, $0.93937$,
  table.hline(),
))
Para correr nuestro programa con este dato adicional, usamos
```
make run-p1 ARGS="0.34 ex/hermite/x2.txt ex/hermite/fx2.txt ex/hermite/dfx2.txt"
```
Esto nos imprime en la consola
```
f(0.34) = 0.3334977765925926
sin(0.34) = 0.3334870921408144
abs_err = 1.0684451778208981e-05
```
Además, nos imprime los correspondientes coeficientes del polinomio generado,
en el archivo `ex/hermite/coeff.csv`.

Observamos que el error absoluto es mayor a cuando tenemos solamente 3 puntos. Si graficamos los polinomios dados,
obtenemos las siguientes gráficas:
#figure(
  image("hermite.png", width: 80%),
  caption: [Ambos polinomios de Hermite comparados con $sin x$],
)
Como podemos observar, la adición del punto $x=0.33$ causa que el polinomio
generado por el algoritmo oscile alrededor del punto $x=0.34$. Si observamos los
coeficientes de los polinomios generados,
#align(center, table(
  columns: (auto, auto),
  [Hermite 4 puntos], [Hermite 3 puntos],
  $0.29552$, $0.29552$,
  $0.95534$, $0.95534$,
  $-0.142$, $-0.142$,
  $-1.05$, $-1.05$,
  $-32.7778$, $20.7778$,
  $17574.1$, $-436.296$,
  $-744815$, [NA],
  $2.94556e+07$, [NA],
))
En particular, observamos los últimos coeficientes, que hacen que la curva
que pasa por el punto $x=0.34$ oscile demasiado, causando un mayor error
absoluto.

#question[
  Genera un código que realice interpolación cúbica natural. Puedes basarte en el algoritmo visto en clase. Con
  este algoritmo, recrea la parte superior de la imagen del pato dado por los puntos de la tabla:
  #table(
    columns: 22,
    align: center,
    stroke: 0.5pt,
    inset: 5pt,
    table.header(
      [$x$],
      [0.9],
      [1.3],
      [1.9],
      [2.1],
      [2.6],
      [3.0],
      [3.9],
      [4.4],
      [4.7],
      [5.0],
      [6.0],
      [7.0],
      [8.0],
      [9.2],
      [10.5],
      [11.3],
      [11.6],
      [12.0],
      [12.6],
      [13.0],
      [13.3],
    ),
    [$f(x)$],
    [1.3],
    [1.5],
    [1.85],
    [2.1],
    [2.6],
    [2.7],
    [2.4],
    [2.15],
    [2.05],
    [2.1],
    [2.25],
    [2.3],
    [2.25],
    [1.95],
    [1.4],
    [0.9],
    [0.7],
    [0.6],
    [0.5],
    [0.4],
    [0.25],
  )


  #align(center, lq.diagram(
    title: [Dato del pato],
    xlim: (0, 14),
    ylim: (-4, 4),
    lq.scatter(
      (0.9, 1.3, 1.9, 2.1, 2.6, 3.0, 3.9, 4.4, 4.7, 5.0, 6.0, 7.0, 8.0, 9.2, 10.5, 11.3, 11.6, 12.0, 12.6, 13.0, 13.3),
      (1.3, 1.5, 1.85, 2.1, 2.6, 2.7, 2.4, 2.15, 2.05, 2.1, 2.25, 2.3, 2.25, 1.95, 1.4, 0.9, 0.7, 0.6, 0.5, 0.4, 0.25),
    ),
  ))
  *Nota*: Para la interpolación puedes utilizar secciones o quitar puntos. La
  idea es que se acerque a la forma adecuada. Sólo debes explicar tu
  procedimiento en el reporte. Si alguno desea recrear al pato completo se le
  puede hacer llegar los puntos a interpolar.
]

Como se observó en el problema anterior, la interpolación por medio de
polinomios de orden superior pueden oscilar de forma errática; es decir,
pequeños cambios en pequeños intervalos pueden causar que el polinomio
generado tenga fluctuaciones enormes sobre todo el rango.

Alternativamente, se busca interpolar la curva por tramos, donde cada
subintervalo requiere de una nueva función de aproximación. La *interpolación
  de spline cúbico* es aquella que aproxima la función por tramos mediante el uso
de polinomios cúbicos. Esto nos garantiza que la función de aproximación
es continuamente diferenciable, y sus segundas derivadas son continuas.

Dada una función $f:[a,b] -> RR$ y un conjunto de nodos $a = x_0 < dots < x_n = b$,
un *interpolante de spline cúbico* $S$ para $f$ es una función que satisface las
siguientes condiciones:
1. $S(x)$ es un polinomio cúbico que se denota $S_j (x)$ en el subintervalo
  $[x_j, x_j+1]$ para cada $j = 0, 1,dots,n-1$;
2. $S_j (x_j) = f(x_j)$ y $S_j (x_(j+1)) = f(x_(j+1))$ para cada $j = 0,1,dots,n-1$;
3. $S_(j+1) (x_(j+1)) = S_j (x_(j+1))$ para cada $j = 0,1,dots,n-2$;
4. $S_(j+1) ' (x_(j+1)) = S_j ' (x_(j+1))$ para cada $j = 0, 1, dots, n-2$;
4. $S_(j+1) '' (x_(j+1)) = S_j '' (x_(j+1))$ para cada $j = 0, 1, dots, n-2$;
5. Uno de los siguientes conjuntos de condiciones de frontera se satisface:
  1. $S''(x_0) = S''(x_n) = 0$ (*frontera natural*);
  2. $S'(x_0) = f'(x_0)$ y $S'(x_n) = f'(x_n)$ (*frontera condicionada*).


Para este ejercicio, usaremos la frontera natural.

Ejecutamos nuestro programa con
```
make run-p2 ARGS="ex/spline-nat/x.txt ex/spline-nat/fx.txt"
```
Esto nos genera la tabla `ex/spline-nat/table.csv`, que contiene los datos de los polinomios
interpoladores de cada subintervalo. Se ve de la siguiente manera:

#let data = csv("Code/ex/spline-nat/table.csv", row-type: array).slice(1)

#let chunk-size = 10

#figure(
  table(
    columns: 6,
    stroke: none,
    table.hline(),
    table.header($bold(i)$, $bold(x_i)$, $bold(a_i)$, $bold(b_i)$, $bold(c_i)$, $bold(d_i)$),
    table.hline(),
    ..data.flatten(),
    table.hline(),
  ),
  caption: [Coeficientes del spline cúbico natural (pato)],
)
#figure(
  image("natural.png"),
  caption: [Spline cúbico que sigue la forma del pato],
)

#question[
  Genera un código que realice interpolación cúbica fijo o sujeto, puedes
  basarte en el algoritmo visto en clase. Con este algoritmo recrea la parte
  superior de Snoopy, dados los puntos de la tabla.
  #grid(
    columns: (1fr, 1fr, 1fr),
    align: center,
    figure(
      caption: [Curve 1],
      table(
        columns: 4,
        stroke: none,
        table.hline(),
        $i$, $x_i$, $f(x_i)$, $f'(x_i)$,
        table.hline(),
        $0$, $1$, $3.0$, $1.0$,
        $1$, $2$, $3.7$, [],
        $2$, $5$, $3.9$, [],
        $3$, $6$, $4.2$, [],
        $4$, $7$, $5.7$, [],
        $5$, $8$, $6.6$, [],
        $6$, $10$, $7.1$, [],
        $7$, $13$, $6.7$, [],
        $8$, $17$, $4.5$, $-0.67$,
        table.hline(),
      ),
    ),
    figure(
      caption: [Curve 2],
      table(
        columns: 4,
        stroke: none,
        table.hline(),
        $i$, $x_i$, $f(x_i)$, $f'(x_i)$,
        table.hline(),
        $0$, $17$, $4.5$, $3.0$,
        $1$, $20$, $7.0$, [],
        $2$, $23$, $6.1$, [],
        $3$, $24$, $5.6$, [],
        $4$, $25$, $5.8$, [],
        $5$, $27$, $5.2$, [],
        $6$, $27.7$, $4.1$, $-0.4$,
        table.hline(),
      ),
    ),
    figure(
      caption: [Curve 3],
      table(
        columns: 4,
        stroke: none,
        table.hline(),
        $i$, $x_i$, $f(x_i)$, $f'(x_i)$,
        table.hline(),
        $0$, $27.7$, $4.1$, $0.33$,
        $1$, $28$, $4.3$, [],
        $2$, $29$, $4.1$, [],
        $3$, $30$, $3.0$, $-1.5$,
        table.hline(),
      ),
    ),
  )
]

Como explicamos en el segundo ejercicio, los splines cúbicos tienen la ventaja sobre los
métodos que utilizan un solo polinomio para la interpolación de muchos puntos. En este caso,
usaremos el método condicionado, que nos permite tener control sobre las derivadas en los puntos
iniciales, lo que hace posible los cambios repentinos en la pendiente de cada función
por sub-intervalo.

Para ejecutar el código correspondiente, hacemos 3 llamadas, una para cada función dada.
```
make run-p3 ARGS="ex/spline-cond/x1.txt ex/spline-cond/fx1.txt 1.0 -0.67 ex/spline-cond/table1.csv"
make run-p3 ARGS="ex/spline-cond/x2.txt ex/spline-cond/fx2.txt 3.0 -4.0 ex/spline-cond/table2.csv"
make run-p3 ARGS="ex/spline-cond/x3.txt ex/spline-cond/fx3.txt 0.33 -1.5 ex/spline-cond/table3.csv"
```
Cada ejecución nos genera una tabla `ex/spline-cond/table.csv` con los
coeficientes correspondientes de cada uno de los polinomio interpoladores.
Se ven de la siguiente manera:
#let data1 = csv("Code/ex/spline-cond/table1.csv", row-type: array).slice(1)
#let data2 = csv("Code/ex/spline-cond/table2.csv", row-type: array).slice(1)
#let data3 = csv("Code/ex/spline-cond/table3.csv", row-type: array).slice(1)

#figure(
  table(
    columns: 6,
    stroke: none,
    table.hline(),
    table.header($bold(i)$, $bold(x_i)$, $bold(a_i)$, $bold(b_i)$, $bold(c_i)$, $bold(d_i)$),
    table.hline(),
    ..data1.flatten(),
    table.hline(),
  ),
  caption: [Coeficientes del spline cúbico condicionado para la Curva 1],
)

#figure(
  table(
    columns: 6,
    stroke: none,
    table.hline(),
    table.header($bold(i)$, $bold(x_i)$, $bold(a_i)$, $bold(b_i)$, $bold(c_i)$, $bold(d_i)$),
    table.hline(),
    ..data2.flatten(),
    table.hline(),
  ),
  caption: [Coeficientes del spline cúbico condicionado para la Curva 2],
)

#figure(
  table(
    columns: 6,
    stroke: none,
    table.hline(),
    table.header($bold(i)$, $bold(x_i)$, $bold(a_i)$, $bold(b_i)$, $bold(c_i)$, $bold(d_i)$),
    table.hline(),
    ..data3.flatten(),
    table.hline(),
  ),
  caption: [Coeficientes del spline cúbico condicionado para la Curva 3],
)
#figure(image("conditioned.png"), caption: [Contorno de Snoopy usando splines cúbicos])


