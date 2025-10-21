#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Análisis de Datos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 5",
  date: datetime.today(),
)

#question[
  El archivo inegi.csv contiene las fechas de más de 230mil personas. Surgieron
  como parte de una encuesta que el INEGI aplicó: en cada casa se pidieron
  (entre otras cosas) las fechas de nacimientos de todos los integrantes. Para
  cada persona se tiene también su estado (1=Ags, 2=BC,3=BCS, ...). Busca
  algunas visualizaciones univariadas de los datos usando ggplot para entender
  mejor la distribución de las fechas a lo largo del año. ¿Qué observas?
]

#align(center, table(
  columns: (auto, auto, auto),
  inset: 10pt,
  align: horizon,
  table.header([*Estado*], [*fn_dia*], [*fn_mes*]),
  [1], [18], [10], 
  [1], [12], [5],
  [1], [22], [7],
))

=== Densidad de mes de nacimiento según los estados
#image("inegi_mes.png")
=== Densidad de día de nacimiento según los estados
#image("inegi_dia.png")

Podemos observar que la mayoría de los mexicanos (de todos los estados) son
nacidos cerca de enero. Es más común que sea un poco después de enero.


#question[
  En este ejercicio estudiamos con simulaciones lo que se llama de manera
  coloquial _ley de las rachas de buena suerte_. Supongamos que un juego
  consiste de $n$ vueltas: en cada una, uno participa en un juego al azar donde
  uno gana o pierde. En cada momento t, $1\leq t\leq n$, uno tiene una capital
  aculmulado $C_t$ , que puede ser un valor positivo o negativo.

  Estudia con simulaciones de la distribución del porcentaje del tiempo entre 1
  y $n$ que uno tiene un capital acumulado positvo. ¿Qué observas? ¿Cómo se
  conecta con rachas de buena o mala suerte?
]

#image("p3.png")

Podemos observar que las victorias se mantienen cerca de la línea $y = x$, que
nos dice que en promedio, se gana una y pierde una, y si tenemos una racha de
derrotas, a la larga nos podemos recuperar en cuanto a victorias.

#question[
  Un avión tiene 120 lugares y va lleno. Se tienen 120 comidas: 60 pastas y 60
  pescados. Cada pasajero hace su elección independiente de los demás. La
  probabilidad de que alguien prefiere pasta a pescado es 0.55. Usa el teorema
  de límite central para obtener una aproximación a la probabilidad que al
  menos 10 personas no tienen el platillo de su primera preferencia.
]

Sea $X$ el número de personas que prefieren pasta. Entonces $X ~ "Binomial"(n = 120, p = 0.55)$.
Si hay más de 60 personas que prefieren pasta, al menos uno no obtendrá pasta. Viceversa con el pescado. 
Entonces, el número de personas sin su plato preferido es $|X - 60|$. Por
ejemplo, si 59 prefieren pasta, 61 prefieren pescado, por lo que hay 1 persona
instisfecha. Entonces, buscamos $ |X-60| >= 10 <==> 50 <= X || X >= 70. $

Por el teorema del límite central, podemos hacer la aproximación $ X approx N(n
mu, n sigma^2) $. En este caso, $ n mu = 120 dot 0.55 = 66, quad n sigma^2 = 29.7
$ Entonces, $ X approx N(66, 29.7). $ Podemos aproximar nuestro evento con $
P(X<=50) + P(X>=70) &approx P(N <= (50-66)/5.45) + P(N>= (70-66)/5.45) \ &=
0.0017 + 0.2315 \ &= 0.2332. $ Es decir, hay $23%$ de probabilidad de que al menos 10 personas queden insatisfechas con la comida que reciben. 

