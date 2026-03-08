#import "@preview/problemst:0.1.2": pset
#import "@preview/arborly:0.3.2": tree

#show: pset.with(
  class: "Programación y Estructuras de Datos II",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 3",
  date: datetime(year: 2026, month: 3, day: 5),
)

#set text(lang: "es")

#show link: set text(fill: blue)
#show link: underline

#set math.equation(
  numbering: "(1)",
  supplement: none,
)

#set par(
  justify: true,
  leading: 0.52em,
)

#show ref: it => {
  // provide custom reference for equations
  if it.element != none and it.element.func() == math.equation {
    // optional: wrap inside link, so whole label is linked
    link(it.target)[(#it)]
  } else {
    it
  }
}

#rect(fill: rgb(0, 0, 0, 10), stroke: 1pt + gray, radius: 5pt)[
  === 1. Quadtree: capacidad óptima por límites superior e inferior (10 pts)
  #{
    set par(spacing: 1pt)
    line(start: (-1%, 0% + 5pt), end: (101%, 0% + 5pt), length: 100%, stroke: 1pt + gray)
  }
  Considera un _point-region quadtree_ sobre el rectangulo
  $
    Sigma = [x_min, x_max] times [y_min, y_max], quad quad x_min < x_max, quad y_min < y_max.
  $
  Cada hoja almacena hasta $C$ puntos. Si al insertar un punto una hoja excede $C$, el nodo se subdivide en cuatro hijos y sus puntos se redistribuyen.

  *Datos de prueba (generados por ti).* Construye el quadtree con capacidad C y define:
  - $S(C)$: número total de subdivisiones realizadas (nodos internos creados por overflow).
  - $D(C)$: profundidad máxima del árbol (raíz en profundidad 0).

  *Capacidades extremas.* Con las métricas anteriores, define:
  - *Capacidad máxima* $C_max$: el mayor entero $C>= 1$ tal que $S(C) >= 4$.
  - *Capacidad mínima* $C_min$: el menor entero $C>=1$ tal que $D(C) <= n -1$.

  La interpretation buscada es:
  - si $C$ es demasiado grande, el árbol casi no subdivide (mala resolution espacial);
  - si $C$ es demasiado pequeño, puede haver sobre-subdivisión (profundidades excesivas).

  *Tarea.* Calcula $C_min$ y $C_max$ para la instancia generada. Tu método debe ser eficiente para $n$ grande: se espera razonamiento algorítmico (por ejemplo, monotonicidad + búsqueda binaria, o una estrategia equivalente bien justificada).
]

== Implementación
La implementación del Quadtree se encuentra en `quadtree.h`. Dentro, tenemos 2
estructuras adicionales: `Point` y `Rectangle`. `Point` consta de una tupla de
valores que representarán las cooredenadas de los puntos en nuestro Quadtree.
`Rectangle` contiene la información de los límites de una región. La clase
`QuadTree` contiene la informacion de cada nodo del árbol.
=== Punto
#figure(
  ```cpp
  struct Point
  {
    double x, y;
    Point(double x_, double y_) : x(x_), y(y_) {}
    Point() : x(0.0), y(0.0) {}
  };
  ```,
  caption: [Implementación de `Point`.],
)
Mantenemos las coordenadas de un punto.

=== Rectángulo
#figure(
  ```cpp
  struct Rectangle
  {
    double xmin, xmax, ymin, ymax;
    Rectangle(double xmin, double xmax, double ymin, double ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
    {
    }
    bool contains(const Point &p) const
    {
      return p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax;
    }
    bool intersects(const Rectangle &o) const
    {
      return !(o.xmin > xmax || o.xmax < xmin || o.ymin > ymax || o.ymax < ymin);
    }
  };
  ```,
  caption: [Implementación de `Rectangle`.],
)
Notemos que `contains` toma los intervalos cerrados $[x_min, x_max] times [y_min, y_max]$. A primera vista, esto causaría conflicto con un punto medio. Sin embargo, debido a la manera en la que implementamos la inserción, solamente una subregión tiene los bordes completamente cerrados. Ver @impl:insert.

=== Quadtree
#figure(
  ```cpp
  bool divided_ = false;
  std::unique_ptr<QuadTree> SW_, SE_, NW_, NE_;
  std::vector<Point> values_;
  Rectangle region_;
  std::size_t capacity_;
  std::size_t depth_;
  ```,
  caption: [Atributos de la clase Quadtree.],
)

Tenemos la bandera `divided_`, que indica si el arbol ha sido dividido. De esta
manera, sabemos si el nodo tiene hijos no nulos, y no intentar buscar valores
dentro del mismo. Los nodos `SW_`, `SE_`, `NW_`, `NE_` son los hijos. El vector
`values_` contiene los valores que caen dentro de la región dada por `region_`.
`capacity_` tiene la capacidad maxima por cada nodo. `depth_` es la profundidad
a la que se encuentra el nodo.


#figure(
  ```cpp
  bool insert(const Point &p)
  {
    if (!region_.contains(p))
      return false;

    if (!divided_)
    {
      if (values_.size() < capacity_)
      {
        values_.push_back(p);
        return true;
      }
      subdivide();
    }

    return SW_->insert(p) || SE_->insert(p) || NW_->insert(p) || NE_->insert(p);
  }
  ```,
  caption: [Método de inserción de un punto.],
)<impl:insert>
Para insertar un punto al árbol, revisamos si el nodo al que apuntamos lo
contiene. Si lo contiene, revisamos si el nodo está subdividido. Si no lo está,
lo insertamos a su vector de valores si tiene espacio. Si no, lo dividimos. Si
sí lo está, lo intentamos insertar en alguno de sus hijos. Notemos el órden en
el que se intenta: `SW_` primero, por lo que tenemos la siguiente relación con
los intervalos que representa cada subregion, asumiendo que la región principal
es $[0,2a] times [0,2a]$.
$
  "SW" & : [0,a] times [0,a] \
  "SE" & : (a,2a] times [0,a] \
  "NW" & : [0,a] times (a,2a] \
  "NE" & : (a,2a] times (a,2a] \
$
De esta manera, no hay ambigüedad en la qué región mantiene un punto. La
complejidad de la inserción es $O(log n)$ en esperanza dado que los datos
provienen de una distribución uniforme, por lo que esperamos que sea un árbol
balanceado.

#figure(
  ```cpp
  void subdivide()
  {
    double mx = (region_.xmin + region_.xmax) * 0.5;
    double my = (region_.ymin + region_.ymax) * 0.5;

    SW_ = std::make_unique<QuadTree>(Rectangle{region_.xmin, mx, region_.ymin, my}, depth_ + 1);
    SE_ = std::make_unique<QuadTree>(Rectangle{mx, region_.xmax, region_.ymin, my}, depth_ + 1);
    NW_ = std::make_unique<QuadTree>(Rectangle{region_.xmin, mx, my, region_.ymax}, depth_ + 1);
    NE_ = std::make_unique<QuadTree>(Rectangle{mx, region_.xmax, my, region_.ymax}, depth_ + 1);

    divided_ = true;

    for (const auto &p : values_)
    {
      SW_->insert(p) || SE_->insert(p) || NW_->insert(p) || NE_->insert(p);
    }
    values_.clear();
  }
  ```,
  caption: [Implementación de `subdivide`.],
)
Notemos que dividimos la region en partes iguales con $m_x$ y $m_y$. Creamos los
apuntadores, establecemos que el nodo ha sido dividido. Luego, intentamos
insertar los puntos dentro de la región en cada una de las subregiones. Notemos
que solo buscamos de `SW` a `NE` de manera lineal, hasta que al menos uno
regrese `true`, que ocurre cuando se inserta satisfactoriamente. Luego, borramos
el contenido de valores del nodo que acabamos de dividir.

== Capacidades mínimas y máximas

=== Mínima
Comenzamos por notar que $C_min$ no tiene mucho sentido. En general, para un
Quadtree de $n$ puntos, la profundidad es a lo más $n - 1$. En particular, $C =
1$ satisface esto trivialmente, por lo que $C_min = 1$.

=== Máxima
Realizamos una busqueda binaria en el conjunto $[0,n]$. En cada iteración, construimos el árbol con capacidad igual al punto medio. Dado que la construcción es $O(n log n)$ (esto es porque en esperanza, la inserción es $O(log n)$, ya que hemos tomado datos uniformemente distribuidos), encontrar $C_max$ toma $O(log n)[O(n log n) + O(n)] = O(n log^2 n)$.

== Funcionamiento del programa
=== Compilación
Compilamos el programa usando

#figure(
  ```sh
  g++ rng.cpp -o rng
  ```,
  caption: [Compilación del programa.],
)
Dado que la implementación del Quadtree se encuentra completamente del archivo `quadtree.h`, no es necesario compilar otros archivos.

=== Ejecución
Una vez compilado, ejecutamos con
#figure(
  ```sh
  ./rng
  ```,
  caption: [Ejecución del programa.],
)

El programa acepta argumentos mediante `stdin` en el siguiente formato:

#figure(
  ```
  n seed xmin xmax ymin ymax
  ```,
  caption: [Argumentos del programa.],
)
== Ejemplos
=== Ejemplo 1
Consideremos el siguiente input:
#figure(
  ```
  100 101 0 1 0 1
  ```,
  caption: [Ejemplo de input: 100 puntos, seed 101, $x_min = y_min = 0$ y $x_max = y_max = 1$. ],
)
Este ejemplo nos imprime lo siguiente:
#figure(
  ```
  23
  ```,
  caption: [Output de `100 101 0 1 0 1`.],
)

#figure(
  image("sample_img/fig1.png"),
  caption: [Ejemplo de subdivision para $n = 100$],
)
=== Ejemplo 2

#figure(
  ```
  200 101 0 1 0 1
  ```,
  caption: [Ejemplo de input: 200 puntos, seed 101, $x_min = y_min = 0$ y $x_max = y_max = 1$. ],
)
Este ejemplo nos imprime lo siguiente:
#figure(
  ```
  45
  ```,
  caption: [Output de `100 101 0 1 0 1`.],
)

#figure(
  image("sample_img/fig2.png"),
  caption: [Ejemplo de subdivision para $n = 200$],
)

=== Ejemplo 3

#figure(
  ```
  1000 101 0 1 0 1
  ```,
  caption: [Ejemplo de input: 1000 puntos, seed 101, $x_min = y_min = 0$ y $x_max = y_max = 1$. ],
)
Este ejemplo nos imprime lo siguiente:
#figure(
  ```
  238
  ```,
  caption: [Output de `1000 101 0 1 0 1`.],
)

#figure(
  image("sample_img/fig3.png"),
  caption: [Ejemplo de subdivision para $n = 1000$],
)

=== Ejemplo 4

#figure(
  ```
  8000 101 0 1 0 1
  ```,
  caption: [Ejemplo de input: 8000 puntos, seed 101, $x_min = y_min = 0$ y $x_max = y_max = 1$. ],
)
Este ejemplo nos imprime lo siguiente:
#figure(
  ```
  1972
  ```,
  caption: [Output de `8000 101 0 1 0 1`.],
)

#figure(
  image("sample_img/fig4.png"),
  caption: [Ejemplo de subdivision para $n = 8000$],
)

=== Ejemplo 5

#figure(
  ```
  100000 101 0 1 0 1
  ```,
  caption: [Ejemplo de input: 100000 puntos, seed 101, $x_min = y_min = 0$ y $x_max = y_max = 1$. ],
)
Este ejemplo nos imprime lo siguiente:
#figure(
  ```
  24929
  ```,
  caption: [Output de `100000 101 0 1 0 1`.],
)


=== Ejemplo 6

#figure(
  ```
  200000 101 0 1 0 1
  ```,
  caption: [Ejemplo de input: 200000 puntos, seed 101, $x_min = y_min = 0$ y $x_max = y_max = 1$. ],
)
Este ejemplo nos imprime lo siguiente:
#figure(
  ```
  49916
  ```,
  caption: [Output de `200000 101 0 1 0 1`.],
)

== Gráficas de resultados
#figure(
  image("sample_img/plot1.png"),
  caption: [Valores de $S(C)$ y $D(C)$ para $n = 100$ y $C in [1,100]$ con $Sigma = [0,1]^2$],
)

== Interpretación de resultados
Empíricamente se observa que $C_max approx n / 4$. Dado que los puntos son
uniformemente distribuidos en $[0,1]^2$, se espera que se encuentren
distribuidos de manera equitativa en cada uno de las subdivisiones.

De acuerdo a lo observado a la gráfica, un valor que parece óptimo $C = 10$ para
$n = 100$ y $Sigma = [0,1]^2$. Para valores más grandes, el cambio es muy pequeño en la cantidad de
subdivisiones.