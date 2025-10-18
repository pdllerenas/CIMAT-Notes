#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question
#let proof = homework.proof

#show: pset.with(
  class: "Programación y Algoritmos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 5",
  date: datetime.today(),
)

#question[
  Comprima las imágenes de _Lena_, _Fractal Tree_ y _Barbara_,
  y muestre la tabla comparativa con los siguientes campos: Tamaño original,
  Longitud de código esperada, Razón de Compresión y Bits-per-Pixel.
  Interprete los resultados. Exponga ideas generales del porque tienen diferente
  CR y bpp.
]

#align(center, table(
  columns: (auto, auto, auto, auto, auto),
  inset: 10pt,
  align: horizon,
  table.header([*Imagen*], [*Tamaño original*], [*E[L]*], [*CR*], [*Bits-per-Pixel*]),
  [Lena],[262144 bytes], [$6.333$],$1.236$,$6.467$,
  [Fractal],[3871488 bytes], [$7.382$],$1.192$,$6.706$,
  [Barbara],[262144 bytes], [$6.065$],$1.233$,$6.488$,
))

El CR depende de que tan largos resultan los códigos. Es decir, si los codigos
en promedio son casi del mismo tamaño de un byte, la compresión será pequeña.
En particular, la imagen de fractal es la que menos CR tiene. Si observamos la
tabla de codigos, notamos que todos los valores de nivel de gris son usados,
mientras que Lena y Barbara no usan muchos de los niveles de gris, por lo que
se necesitan menos codigos (y por lo tanto, de menor longitud) para represenar
la imagen.

En cuanto al bpp, se busca que sea menor a 8, de esta manera la imagen es
verdaderamente una compresión de la original. En el caso de Lena y Barbara,
observamos valores cerca de 6, mientras que el de fractal tiene un nivel
cercano a 8. Esto se debe a que Lena y Barbara repiten mayormente los mismos
niveles de gris, mientras que el fractal tiene una distribución más uniforme.
