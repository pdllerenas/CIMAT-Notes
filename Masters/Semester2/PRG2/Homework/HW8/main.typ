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

== Solución
Usamos programación dinámica para poblar la matriz de posibles estados. Comenzamos por
las orillas, que por como menciona el problema, serán
$
  E[i][0] = i\
  E[0][j] = j\
$
Es decir, por cada caracter que saltamos, el costo es 1. Con estos valores iniciales, tenemos la siguiente
recursión:
$
  E[i][j] = min cases(
    E[i - 1][j - 1] & "if " P[i] == S[j],
    E[i - 1][j - 1] + 1 & "if " P[i] != S[j],
    E[i - 2][j - 2] + 1 & "if " P[i - 1] == S[j] "and" P[i] == S[j - 1] "and" i\,j >= 2,
    E[i - 1][j] + 1,
    E[i][j-1] + 1,
  )
$
Luego, para recuperar las acciones que llevan a la solución, realizamos backtracking. Es decir, comenzando de la esquina inferior derecha de la matriz,
buscamos un valor que lo haya llevado a esa posición.

== Complejidad
Generar la matriz es $O(n m)$ en tiempo y espacio, donde $n$ y $m$ son los
tamaños de las cadenas. Realizar backtracking es $O(n+m)$.

== Ejecución
Usar
#figure(
  ```sh
  g++ p1.cpp -o p1
  ```,
)
y luego
#figure(
  ```sh
  ./p1
  ```,
)
Luego, se piden 2 cadenas de input.

== Ejemplo
#figure(
  ```
  ACTG
  ACGT
  Strings: ACTG -> ACGT
  Damerau-Levenshtein distance: 1
  Optimal Sequence: Match Match Transpose
  -----------------------------------
  GACTCG
  AGTCGC
  Strings: GACTCG -> AGTCGC
  Damerau-Levenshtein distance: 3
  Optimal Sequence: Transpose Transpose Transpose
  -----------------------------------
  AAACGT
  ACGTAA
  Strings: AAACGT -> ACGTAA
  Damerau-Levenshtein distance: 4
  Optimal Sequence: Delete Delete Match Match Match Match Insert Insert
  -----------------------------------
  ```,
)

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

== Solución
Este problema es equivalente a uno resuelto anteriormente con KMP (tarea 6). En
particular, https://codeforces.com/problemset/problem/471/D. Usamos la misma
solución.

== Ejecución
Usar
#figure(
  ```sh
  g++ p2.cpp -o p2
  ```,
)
y luego
#figure(
  ```sh
  ./p2
  ```,
)
Luego, introducir los dígitos como se describe en el problema de Codeforces.
== Ejemplo

#figure(
  ```
  13 5
  2 4 5 5 4 3 2 2 2 3 3 2 1
  3 4 4 3 2
  ```,
)
da como respuesta
#figure(
  ```
  2
  ```,
)


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

== Solución
Realizamos un hash
$
  h(s[i,j])p^i mod m = h(s[0,j]) - h(s[0, i-1]) mod m
$
de manera iterativa para cada longitud de subcadena. Realizamos un _sliding
window_ para una longitud fija, lo que nos proporciona con el hash de todas las
subcadenas de longitud $l$. Luego, imprimimos las repeticiones máximas, que se
obtienen a partir de hashes repetidos. Si no hay repeticiones, salimos del
programa, ya que para cadenas mayores, es imposible tener repeticiones (por
monotonía)

== Complejidad
Realizar el hash de todas las subcadenas toma $O(n^2)$ (nuestra implementación
no hace la optimización de $O(n)$), y espacio $O(m)$.

== Ejecución
Usar
#figure(
  ```sh
  g++ p3.cpp -o p3
  ```,
)
y luego
#figure(
  ```sh
  ./p3
  ```,
)
Luego, se espera una cadena de caracteres.
== Ejemplo
#figure(
  ```
  PARANGARIUTIRIMICUARO
  ```,
)
genera la respuesta
#figure(
  ```
  4
  3
  ```,
)
