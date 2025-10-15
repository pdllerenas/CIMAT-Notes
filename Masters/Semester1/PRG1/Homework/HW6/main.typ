#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Programación y Algoritmos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 6",
  date: datetime.today(),
)


En este reporte presentamos las diferencias de tiempo promedio de ejecución
de 6 algoritmos distintos, donde cada uno determina si un año es bisiesto o no. Realizamos 1000 iteraciones de cada algoritmo,
probando los años en el rango [0, 10000].

La compilación se realiza mediante
```
g++ main.cpp -o main
```
Luego, el programa espera 3 argumentos adicionales:

_trials_: número de ejecuciones por algoritmo;\
_year-start_: año de comienzo para la prueba de cada algoritmo;\
_year-end_: año de finalización para la prueba de cada algoritmo.

Por ejemplo, la siguiente llamada realiza 10000 iteraciones de cada algoritmo,
desde el año 0 al año 10000.
```
./main 10000 0 10000
```

Una vez ejecutado, se imprimirá una tabla con los tiempos de ejecución promedio
de cada algoritmo.

#align(center, table(
  columns: (auto, auto, auto, auto, auto, auto),
  inset: 10pt,
  align: center,
  table.header([*GPT*], [*Microsoft v1*], [*Microsoft v2*], [*Optimized 400*], [*Optimized 16*], [*Proposed*]),
  [0.032970 ms], [0.025163 ms], [0.023187 ms], [0.027557 ms], [0.027588 ms], [0.032161 ms],
))

Los tiempos de ejecución promedio de cada algoritmo varían de ejecución en
ejecución, pero en general, podemos decir que casi siempre se cumple
el siguiente ranking:


#align(center, table(
  columns: (auto, auto),
  inset: 10pt,
  align: center,
  table.header([*\#*], [*Algoritmo*]),
  [1], [Microsoft v2],
  [2], [Microsoft v1],
  [3], [Optimized 16],
  [4], [Optimized 400],
  [5], [Proposed],
  [6], [GPT],
))

Las posiciones 2 y 3 se intercambian de vez en cuando.

