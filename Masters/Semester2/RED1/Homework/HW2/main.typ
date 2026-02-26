#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Programación de Redes I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 2",
  date: datetime(year: 2026, month: 2, day: 6),
)

#set text(lang: "es")
#question[
  Dada una imagen de entrada, obtenga la frecuencia de cada uno de sus pixeles usando 1, 2, 4 procesos.
  1. Determine el tiempo de ejecución para cada caso (ver ANEXO 1);
  2. ¿El tiempo de ejecución con 2 y 4 procesos es 2 y 4 veces menor que un solo proceso?
]



#question[
  Implementar la suma y multiplicación de matrices 3D mediante $J$ procesos creados con la función *fork()*. Usar matrices cuadradas cuyas dimensiones y número de procesos son dados como línea de comando y presentar una tabla comparativa de los tiempos de ejecución.
]


#question[
  Dada una imagen de entrada, calcular el filtro de la media y mediana usando $N = {1, 2, 4, 9}$ procesos. Presentar una tabla comparativa con los tiempos de ejecución.
]