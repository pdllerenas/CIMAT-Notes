#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/cetz:0.5.0": canvas, draw, tree
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 8",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

#set list(marker: sym.square.filled)
= Problema 1
Basándose en las ideas desarrolladas en la clase, implementar un algoritmo
determinando la distancia de Damerau-Levenshtein entre dos cadenas ADN
(conformadas por los cuatro caracteres A, C, T, G). La distancia de
Damerau-Levenshtein entre dos cadenas es definida como la mínima suma de costos
de acciones permitiendo hacer pasar la primera cadena a la segunda. Esas
acciones son parecidas a las que vimos en la clase, con un cambio. Incluyen:
- la sustitución de un caracter $a$ por otro $b$, con costo:
$
  delta_(a b) = 0 " si " a = b, 1 " si no"
$
- el salto de un caracter en una de las cadenas, con costo 1 (correspondiendo a
  quitar o agregar un caracter la primera cadena)
- una transposición de dos caracteres $a b$ se vuelven $b a$ en la segunda
  cadena, con costo 1 en total (para los dos caracteres).

El programa determinará la distancia de Damerau-Levenshtein, así como la
secuencia de acciones óptimas permitiendo pasar de la primera cadena ADN a la
segunda.

= Problema 2
Pedro vino desde lejos a estudiar en Guanajuato. Se acuerda con mucha nostalgia
del paisaje de los cerros que rodeaban Atzaltitlán, su pueblo natal, y siempre,
incluso entre los cerros de Guanajuato, intenta identificar parte de la linea
del horizonte que observa con su recuerdo de la linea del horizonte de su
querida Atzaltitlán.

Describimos una porción del horizonte de Atzaltitlán como una función constante
por pedazos sobre $m$ intervalos, tomando como valores de altura enteros
positivos $a_1, a_2, ..., a_m$ como ilustrado en la figura abajo.

Escribir una función que, dada una descripción similar para el horizonte de
Guanajuato como valores $g_1, ..., g_n$, nos de el número de veces que se puede
“reconocer” el horizonte de Azaltitlán dentro del de Guanajuato. Ojo: es
permitido trasladar globalmente y tanto horizontalmente *como verticalmente* los
horizontes para considerar empalmes.

Por ejemplo, en el perfil de Guanajuato abajo (en rojo), se encuentra un empalme
con Azaltitlán entre $g_3$ y $g_9$).

= Problema 3

Utilizando las técnicas de hashing que vimos en la clase, escribir un programa que lea una cadena
s pasada de entrada y genere de output una lista de números que corresponderán, para el $i$-ésimo
número, al número máximo de repeticiones de sub-cadenas de longitud $i$. Cuando no hay sub-cadenas
repetidas para una longitud dada, no se dará output

Por ejemplo, para la cadena
$
  s = "PARANGARIUTIRIMICUARO"
$
la respuesta esperada es
```
4
3
```
porque la sub-cadena \'A\' es repetida 4 veces (y es la más frecuente de
longitud 1), la sub-cadena \'AR\' es repetida 3 veces (y es la más frecuente de
longitud 2), y no hay sub-cadena repetida de longitud 3 o más.
