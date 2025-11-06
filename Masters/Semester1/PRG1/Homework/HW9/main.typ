#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Programación y Algoritmos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 9",
  date: datetime.today(),
)

= Definiciones
En este reporte, creamos el tipo de dato _Flyte_, que corresponde a la
siguiente estructura de datos:
```c
struct {
  unsigned int sign: 1;
  unsigned int exponent: 4;
  unsigned int mantissa: 3;
};
```
En general, la sentencia
```c
unsigned int var: 1;
```
dentro de un struct nos define la propiedad `var` como un espacio en la memoria que
solamente ocupa 1 bit. Entonces, en este caso, tenemos 1 bit para el signo, 4 para el exponente,
y 3 para la mantisa.

== Exponentes
Por construcción, la estructura tiene tamaño 8 bits = 1 byte. Para que siga los
estándares de un formato de la IEEE 754, el exponente tiene las siguiente
propiedades:

- $E_min = 0001_2 - 0111_2 = -6$
- $E_max = 1110_2 - 0111_2 = 7$
- Sesgo $= 0111_2 = 7$.

Con estas definiciones, tenemos los siguientes casos:
#align(center, table(
  columns: 3,
  table.header([*Exponente*], [*Mantisa = 0*], [*Mantisa != 0*]),
  $0000_2$, $0$, $(-1)^("sign") times 2^(-6) times 0."bits"$,
  $0001_2, dots, 1110_2$, $(-1)^("sign") times 2^("exp" - 7) times 1.0$, $(-1)^("sign") times 2^("exp" - 7) times 1."bits"$,
  $1111_2$, $plus.minus infinity$, "NaN",
))

Entonces, el número positivo más pequeño y el más grande representables por
este sistema son
$ 
00000001_2 &= 2^(-6) = 0.015625, \
01110111_2 &= 2^(7) times 1.875 = 240,
$ 
respectivamente. 

== Conversiones
Para realizar conversiones de decimal a _flyte_, debemos tomar en cuenta que
un nivel de precisión se perderá. Por ejemplo, para convertir el número 3.42
a _flyte_, primero lo normalizamos a que tenga la forma 1.x. Notemos que
$ 
3 &= 11_2,\
0.42 &= 0.84 times 2^(-1) = 1.68 times 2^(-2) = 3.36 times 2^(-3) = 11_2
$
Con esto, tenemos 
$ 
3.42 = 11.111_2 times 2^(0) = 1.1111_2times 2^1.
$ 
Como $1 = 1_2$, el exponente que usamos en la representacion es $1 + 7 = 8 = 0100_2$, 
mientras que la mantisa es $111_2$ (truncando el bit menos significativo, ya
que solo tenemos 3 bits disponibles). Es decir, el numero en sistema _flyte_ es
$ 
3.42 = 00100111_2.
$ 
Sin embargo, para convertirlo de vuelta a decimal, notemos lo siguiente:
$ 
00100111_2 = 2^(8-7) times 1.875 = 3.75,
$ 
es decir, perdimos el valor que inicialmente habiamos ingresado.
