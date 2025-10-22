#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#let question = homework.complex-question

#show: pset.with(
  class: "Programación y Algoritmos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 7",
  date: datetime.today(),
)

En este reporte, buscamos implementar un programa que reciba una cadena de
caracteres y determine si esta tiene el formato de *notación científica*. En caso
de que no sea así, el programa realizará la conversión correspondiente a su
forma en notación científica.

La *notación científica* se utiliza para expresar números demasiado _grandes_ (o
_pequeños_) que resultan inconvenientes de escribir en forma decimal.
Generalmente, esta notación emplea la base 10 para dichas representaciones. Por
ejemplo:

#align(center, table(
  columns: (auto, auto),
  [*Decimal*], [*Notación Científica*],
  $100$, $1 times 10^2$,
  $0.0003$, $3 times 10^(-4)$,
  $12.345$, $1.2345 times 10^1$,
))

Como primer solución, utilizaremos *expresiones regulares*. Las expresiones
regulares consisten de constantes, que denotan conjuntos de strings, y
operadores, que actúan sobre estos conjuntos de strings. Para nuestro objetivo,
utilizaremos la siguiente expresión:
#align(center, "[+-]?\d+(\.\d+)?([eE][+-]?\d+)")
En order, explicamos cada uno de los componentes:
#align(center, table(
  columns: (auto, 1fr),
  [*Expresión*], [*Descripción*],
  `[+-]?`, "Busca 0 o 1 ocurrencias de alguno de los caracteres '+' o '-'.",
  `\d+`, "Busca una o más ocurrencias de un dígito (0-9)",
  `(\.\d+)?`, "Busca 0 o 1 ocurrencias de la combinación de un '.' y una o más ocurrencias de un dígito (0-9)",
  `[eE]`, "Busca una ocurrencia de 'e' o 'E'",
))

Nótese que los parentesis se utilizan para agrupar expresiones. De esta manera,
podemos marcar como opcionales un conjunto de caracteres, pero si se presentan,
deben aparecer de la manera descrita dentro de los parentesis.

La siguiente forma de resolver este problema, con la utilidad añadida de
covertir/corregir ciertas expresiones a una notación científica normalizada,
se hace a partir de una máquina de estado finito. Entonces, consideremos la
siguiente:

#align(center, rect(inset: -5pt, outset: 10pt, diagram(
  node-stroke: 1pt,
  node((0, 0.5)),
  edge("-|>", label-pos: -50%, label-side: center, label-anchor: "north", label-size: 8pt, [Input]),
  node((0, 0), $q_0$, name: <q0>),
  edge(<q0>, "-", <q2>, label-size: 8pt, bend: -45deg, [D]),
  edge("-", label-size: 8pt, $+ slash -$),
  node((1, 0), $q_1$, name: <q1>),
  edge("-", label-size: 8pt, [D]),
  node((2, 0), $q_2$, name: <q2>),
  edge(<q2>, "-|>", <q2>, bend: 130deg, loop-angle: 90deg, label-size: 8pt, [D]),
  edge(<q2>, "-|>", <q10>, label-side: left, label-size: 8pt, [Integer]),
  node((2, 1), $q_10$, shape: circle, name: <q10>, extrude: (-1, -3)),
  edge(<q2>, "-", <q3>, [.]),
  node((3, 0), $q_3$, name: <q3>),
  edge(<q3>, "-", <q4>, label-size: 8pt, [D]),
  node((4, 0), $q_4$, name: <q4>),
  edge(<q4>, "-|>", <q4>, label-size: 8pt, bend: 130deg, loop-angle: 90deg, [D]),
  edge(<q4>, "-", <q5>, label-size: 8pt, [E/e]),
  node((5, 0), $q_5$, name: <q5>),
  edge(<q2>, "-|>", <q5>, label-size: 8pt, bend: 65deg, [E/e]),
  edge(<q5>, "-", <q6>, label-size: 8pt, $+ slash -$),
  edge(<q4>, "-", <q9>, label-size: 8pt, [Floating point]),
  node((4, 1), $q_9$, shape: circle, name: <q9>, extrude: (-1, -3)),
  node((6, 0), $q_6$, name: <q6>),
  edge(<q6>, "-", <q7>, label-size: 8pt, [D]),
  node((7, 0), $q_7$, name: <q7>),
  edge(<q7>, "-|>", <q7>, label-size: 8pt, bend: 130deg, loop-angle: 90deg, [D]),
  edge(<q7>, "-", label-size: 8pt, <q5>, bend: 45deg, loop-angle: 90deg, [D]),
  edge(<q7>, "-", label-size: 8pt, <q8>, label-side: left, [Scientific Notation]),
  node((7, 1), $q_8$, shape: circle, name: <q8>, extrude: (-1, -3)),
)))

Este diagrama se puede explicar de la siguiente manera:

#table(
  columns: (auto, 1fr),
  [*Nodo*], [*Descripción*],
  $q_0$, "Recibe string a analizar. Se dirige a " + $q_1$ + "si el primer caracter es +/-, a " + $q_2$ + " si es un dígito.",
  $q_1$, "Si el siguiente caracter del string es entero, avanzar a " + $q_2$,
  $q_2$, "Realiza un bucle hasta que se dejen de encontrar dígitos contiguos. Si ya no hay más caracteres, el dígito es un entero, y se dirige a  " + $q_10$ + ". Si el caracter es e/E, avanza a " + $q_5$ + ". Si es un punto decimal, avanza a " + $q_3$,
  $q_3$, "El caracter debe ser un dígito. Si lo es, avanza a " + $q_4$ , 
  $q_4$, "Un bucle que encuentre dígitos hasta que se acaben los caracteres (punto flotante), avanza a " + $q_9$ + ", o se encuentre una e/E, se avanza a " + $q_5$,
  $q_5$, "Busca +/- o un dígito. Si encuentra un signo, avanza a " + $q_6$ + ". Si encuentra un dígito, avanza a " + $q_7$,
  $q_6$, "Busca un dígito. Si lo es, avanza a " + $q_7$,
  $q_7$, "Bucle que encuentra dígitos, hasta que se terminen los caracteres. Avanza a " + $q_8$,
  $q_8$, "Regresa que el string es notación científica",
  $q_9$, "Regresa que el string es de punto flotante",
  $q_10$, "Regresa que el string es un entero",
)

En particular, esta nos permite ver qué tipo de expresión se leyó: entera, de punto flotante, o en notación científica.
Notemos que este sistema clasifica expresiones como $11.1 e 10$ como notación científica. Nosotros buscamos 
normalizar esta representación.

El siguiente programa utiliza ambas técnicas para validar e intenta corregir las expresiones que no se encuentran
en notación científica. Se compila usando 

```
g++ main.cpp -o main
```
Y se ejecuta usando
```
./main
```
Un ejemplo de interacción con el programa es el siguiente:
```
=============================================
Enter a number in scientific notation:
12.2
Regex says: Invalid.
Finite state machine says: 1.22e1
=============================================
=============================================
Enter a number in scientific notation:
1.22e1
Regex says: Valid.
Finite state machine says: 1.22e1
=============================================
=============================================
Enter a number in scientific notation:
+12.2
Regex says: Invalid.
Finite state machine says: +1.22e1
=============================================
=============================================
Enter a number in scientific notation:
+1.22e1
Regex says: Valid.
Finite state machine says: +1.22e1
=============================================
=============================================
Enter a number in scientific notation:
-0.023e-2
Regex says: Valid.
Finite state machine says: -0.023e-2
=============================================
=============================================
Enter a number in scientific notation:
-23.23e-3
Regex says: Valid.
Finite state machine says: -2.323e-2
=============================================
```
Notemos que el regex nos dice si la notación es válida o no, mientras que la máquina
de estado finito nos normaliza la expresión. 
