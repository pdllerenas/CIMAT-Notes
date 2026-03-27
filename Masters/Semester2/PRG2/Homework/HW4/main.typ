#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#import "@preview/lovelace:0.3.1": *
#import exercise: project, subtask, task

#set text(lang: "es")

#show: project.with(
  title: "Tarea 4",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],

  abstract: [
    Realizamos un análisis del algoritmo Edmonds-Karp sobre grafos de flujo,
    mostramos una implementación del mismo y un problema de cubrimiento mínimo de
    caminos en un Directed Acyclic Graphs (DAG). Además, un análisis comparativo entre el
    algoritmo Dinic y el algoritmo Hopcroft-Karp.
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",

  show-solutions: false,
)

= Edmonds-Karp

El algoritmo Edmonds-Karp fija la elección de camino tomando el más corto, el
cual es encontrado mediante BFS. Cada BFS cuesta $O(E)$, y el número de aumentos
es $O(V E)$, por lo que el algoritmo tiene complejidad final $O(V E^2)$.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Edmonds-Karp])[
    + *input* (G: Graph, s: start node, t: end node)
    + *output* Edge capacity function
    + *for* $"Edge"(u, v) in "Edges"(G)$
      + $f(u,v) = 0$
    + *end*

    + *while* BFS finds path $p$ from $s -> t$ in the residual network $G_f$
      + $Delta = min {c_(f(u,v)) : (u,v) in p}$
      + *for* $"Edge"(u,v) in p$
        + *if* $(u,v) in "Edges"(G)$
          + $f(u,v) += Delta$
        + *else*
          + $f(u,v) -= Delta$
        + *end*
      + *end*
      + *return* f
    + *end*
  ],
) <algo:edmonds-karp>

= Ejemplo de uso de Edmonds-Karp
Supongamos que tenemos el siguente grafo.
#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `s`, name: <s>),
    edge(<s>, "-|>", <a>, label-size: 8pt, [8]),
    edge(<s>, "-|>", <b>, label-size: 8pt, [7]),
    node((1, -1), `a`, name: <a>),
    edge(<a>, "-|>", <b>, label-size: 8pt, [3]),
    edge(<a>, "-|>", <c>, label-size: 8pt, [6]),
    edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [3]),
    node((1, 1), `b`, label: "b", name: <b>),
    edge(<b>, "-|>", <c>, crossing: true, label-pos: 25%, label-size: 8pt, [2]),
    edge(<b>, "-|>", <d>, label-size: 8pt, [6]),
    node((3, -1), `c`, label: "c", name: <c>),
    edge(<c>, "-|>", <d>, label-size: 8pt, [1]),
    edge(<c>, "-|>", <t>, label-size: 8pt, [7]),
    node((3, 1), `d`, label: "d", name: <d>),
    edge(<d>, "-|>", <t>, label-size: 8pt, [8]),
    node((4, 0), `t`, label: "t", name: <t>),
  ),
  caption: [Grafo de flujos inicial.],
)

Ahora, realizamos el algoritmo. Cada figura representa una iteración.

#grid(
  columns: (1fr, 1fr),
  figure(
    diagram(
      node-stroke: 1pt,
      node((0, 0), `s`, name: <s>),
      edge(<s>, "-|>", <a>, stroke: eastern, label-size: 8pt, text(eastern, [6/8])),
      edge(<s>, "-|>", <b>, label-size: 8pt, [0/7]),
      node((1, -1), `a`, name: <a>),
      edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
      edge(<a>, "-|>", <c>, stroke: eastern, label-size: 8pt, text(eastern, [6/6])),
      edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [0/3]),
      node((1, 1), `b`, label: "b", name: <b>),
      edge(<b>, "-|>", <c>, crossing: true, label-pos: 25%, label-size: 8pt, [0/2]),
      edge(<b>, "-|>", <d>, label-size: 8pt, [0/6]),
      node((3, -1), `c`, label: "c", name: <c>),
      edge(<c>, "-|>", <d>, label-size: 8pt, [0/1]),
      edge(<c>, "-|>", <t>, stroke: eastern, label-size: 8pt, text(eastern, [6/7])),
      node((3, 1), `d`, label: "d", name: <d>),
      edge(<d>, "-|>", <t>, label-size: 8pt, [0/8]),
      node((4, 0), `t`, label: "t", name: <t>),
    ),
    caption: [Iteración 1. Flujo máximo actual de 6.],
  ),

  figure(
    diagram(
      node-stroke: 1pt,
      node((0, 0), `s`, name: <s>),
      edge(<s>, "-|>", <a>, stroke: eastern, label-size: 8pt, text(eastern, [8/8])),
      edge(<s>, "-|>", <b>, label-size: 8pt, [0/7]),
      node((1, -1), `a`, name: <a>),
      edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
      edge(<a>, "-|>", <c>, label-fill: red, label-size: 8pt, [6/6]),
      edge(<a>, "-|>", <d>, stroke: eastern, label-pos: 25%, label-size: 8pt, text(eastern, [2/3])),
      node((1, 1), `b`, label: "b", name: <b>),
      edge(<b>, "-|>", <c>, crossing: true, label-pos: 25%, label-size: 8pt, [0/2]),
      edge(<b>, "-|>", <d>, label-size: 8pt, [0/6]),
      node((3, -1), `c`, label: "c", name: <c>),
      edge(<c>, "-|>", <d>, label-size: 8pt, [0/1]),
      edge(<c>, "-|>", <t>, label-size: 8pt, [6/7]),
      node((3, 1), `d`, label: "d", name: <d>),
      edge(<d>, "-|>", <t>, stroke: eastern, label-size: 8pt, text(eastern, [2/8])),
      node((4, 0), `t`, label: "t", name: <t>),
    ),
    caption: [Iteración 2. Flujo máximo actual de 8.],
  ),
)

#grid(
  columns: (1fr, 1fr),
  figure(
    diagram(
      node-stroke: 1pt,
      node((0, 0), `s`, name: <s>),
      edge(<s>, "-|>", <a>, label-fill: red, label-size: 8pt, [8/8]),
      edge(<s>, "-|>", <b>, stroke: eastern, label-size: 8pt, text(eastern, [1/7])),
      node((1, -1), `a`, name: <a>),
      edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
      edge(<a>, "-|>", <c>, label-fill: red, label-size: 8pt, [6/6]),
      edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [2/3]),
      node((1, 1), `b`, label: "b", name: <b>),
      edge(<b>, "-|>", <c>, crossing: true, stroke: eastern, label-pos: 25%, label-size: 8pt, text(eastern, [1/2])),
      edge(<b>, "-|>", <d>, label-size: 8pt, [0/6]),
      node((3, -1), `c`, label: "c", name: <c>),
      edge(<c>, "-|>", <d>, label-size: 8pt, [0/1]),
      edge(<c>, "-|>", <t>, stroke: eastern, label-size: 8pt, text(eastern, [7/7])),
      node((3, 1), `d`, label: "d", name: <d>),
      edge(<d>, "-|>", <t>, label-size: 8pt, [2/8]),
      node((4, 0), `t`, label: "t", name: <t>),
    ),
    caption: [Iteración 3. Flujo máximo actual de 9.],
  ),

  figure(
    diagram(
      node-stroke: 1pt,
      node((0, 0), `s`, name: <s>),
      edge(<s>, "-|>", <a>, label-fill: red, label-size: 8pt, [8/8]),
      edge(<s>, "-|>", <b>, stroke: eastern, label-size: 8pt, text(eastern, [2/7])),
      node((1, -1), `a`, name: <a>),
      edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
      edge(<a>, "-|>", <c>, label-fill: red, label-size: 8pt, [6/6]),
      edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [2/3]),
      node((1, 1), `b`, label: "b", name: <b>),
      edge(<b>, "-|>", <c>, crossing: true, stroke: eastern, label-pos: 25%, label-size: 8pt, text(eastern, [2/2])),
      edge(<b>, "-|>", <d>, label-size: 8pt, [0/6]),
      node((3, -1), `c`, label: "c", name: <c>),
      edge(<c>, "-|>", <d>, stroke: eastern, label-size: 8pt, text(eastern, [1/1])),
      edge(<c>, "-|>", <t>, label-fill: red, label-size: 8pt, [7/7]),
      node((3, 1), `d`, label: "d", name: <d>),
      edge(<d>, "-|>", <t>, stroke: eastern, label-size: 8pt, text(eastern, [3/8])),
      node((4, 0), `t`, label: "t", name: <t>),
    ),
    caption: [Iteración 4. Flujo máximo actual de 10.],
  ),
)


#grid(
  columns: (1fr, 1fr),
  figure(
    diagram(
      node-stroke: 1pt,
      node((0, 0), `s`, name: <s>),
      edge(<s>, "-|>", <a>, label-fill: red, label-size: 8pt, [8/8]),
      edge(<s>, "-|>", <b>, stroke: eastern, label-size: 8pt, text(eastern, [7/7])),
      node((1, -1), `a`, name: <a>),
      edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
      edge(<a>, "-|>", <c>, label-fill: red, label-size: 8pt, [6/6]),
      edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [2/3]),
      node((1, 1), `b`, label: "b", name: <b>),
      edge(<b>, "-|>", <c>, crossing: true, label-fill: red, label-pos: 25%, label-size: 8pt, [2/2]),
      edge(<b>, "-|>", <d>, stroke: eastern, label-size: 8pt, text(eastern, [5/6])),
      node((3, -1), `c`, label: "c", name: <c>),
      edge(<c>, "-|>", <d>, label-fill: red, label-size: 8pt, [1/1]),
      edge(<c>, "-|>", <t>, label-fill: red, label-size: 8pt, [7/7]),
      node((3, 1), `d`, label: "d", name: <d>),
      edge(<d>, "-|>", <t>, stroke: eastern, label-size: 8pt, text(eastern, [8/8])),
      node((4, 0), `t`, label: "t", name: <t>),
    ),
    caption: [Iteración 5. Flujo máximo actual de 15.],
  ),

  figure(
    diagram(
      node-stroke: 1pt,
      node((0, 0), `s`, name: <s>),
      edge(<s>, "-|>", <a>, label-fill: red, label-size: 8pt, [8/8]),
      edge(<s>, "-|>", <b>, label-fill: red, label-size: 8pt, [7/7]),
      node((1, -1), `a`, name: <a>),
      edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
      edge(<a>, "-|>", <c>, label-fill: red, label-size: 8pt, [6/6]),
      edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [2/3]),
      node((1, 1), `b`, label: "b", name: <b>),
      edge(<b>, "-|>", <c>, crossing: true, label-fill: red, label-pos: 25%, label-size: 8pt, [2/2]),
      edge(<b>, "-|>", <d>, label-size: 8pt, [5/6]),
      node((3, -1), `c`, label: "c", name: <c>),
      edge(<c>, "-|>", <d>, label-fill: red, label-size: 8pt, [1/1]),
      edge(<c>, "-|>", <t>, label-fill: red, label-size: 8pt, [7/7]),
      node((3, 1), `d`, label: "d", name: <d>),
      edge(<d>, "-|>", <t>, label-fill: red, label-size: 8pt, [8/8]),
      node((4, 0), `t`, label: "t", name: <t>),
    ),
    caption: [Diagrama final.],
  ),
)

== Corte mínimo del grafo
Un corte $(S,T)$ de un grafo de flujo $G = (V,E)$ es una partición de $V$ en $S$ y $T$. Si $f$ es un flujo, entonces el flujo neto $f(S,T)$ es
$
  f(S,T) = sum_(u in S) sum_(v in T) f(u,v) - sum_(u in S) sum_(v in T) f(v,u)
$

La capacidad del corte $(S,T)$ es:
$
  c(S,T) = sum_(u in S) sum_(v in T) c(u,v)
$
Un corte mínimo de un grafo es un corte cuya capacidad es mínima en todos los cortes de la red.

Aplicando estas definiciones a nuestro ejemplo,

#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `s`, name: <s>),
    edge(<s>, "-|>", <a>, label-fill: red, label-size: 8pt, [8/8]),
    edge(<s>, "-|>", <b>, label-fill: red, label-size: 8pt, [7/7]),
    node((1, -1), `a`, name: <a>),
    edge(<a>, "-|>", <b>, label-size: 8pt, [0/3]),
    edge(<a>, "-|>", <c>, label-fill: red, label-size: 8pt, [6/6]),
    edge(<a>, "-|>", <d>, label-pos: 25%, label-size: 8pt, [2/3]),
    node((1, 1), `b`, label: "b", name: <b>),
    edge(<b>, "-|>", <c>, label-fill: red, label-pos: 25%, label-size: 8pt, [2/2]),
    edge(<b>, "-|>", <d>, label-size: 8pt, [5/6]),
    node((3, -1), `c`, label: "c", name: <c>),
    edge(<c>, "-|>", <d>, label-fill: red, label-size: 8pt, [1/1]),
    edge(<c>, "-|>", <t>, label-fill: red, label-size: 8pt, [7/7]),
    node((3, 1), `d`, label: "d", name: <d>),
    edge((4, 1), "-", crossing: true, (1.5, -1.5), stroke: red),
    edge(<d>, "-|>", <t>, label-fill: red, label-size: 8pt, [8/8]),
    node((4, 0), `t`, label: "t", name: <t>),
  ),
  caption: [Diagrama con corte. Los nodos de $S$ son ${s,a,b,c}$, y los nodos de $T$ son ${c,t}$.],
),

Notemos que pare este corte, tenemos
$
  f(S,T) & = f(a,c) + f(b,c) + f(d,t) - f(c,d) \
         & = 6 + 2 + 8 - 1 \
         & = 15.
$
Este valor coincide con el flujo total del grafo.

== Eficiencia de BFS
Si decidimos no usar BFS para encontrar el camino más corto, podemos terminar con más iteraciones de las necesarias. La siguiente secuencia de iteraciones presenta este problema.

= Cubrimiento de caminos en un DAG
Para encontrar un cubrimiento de caminos, usaremos un matching bipartito. Esto resulta en la solución requerida ya que se genera una relación donde un nodo tiene a lo más una arista. Al hacer el seguimiento de los nodos, se crea un camino. Se tiene entonces que
$
  "min paths" = "total vertices" - "max matches"
$
#figure(
  ```cpp
  bool bip_match(int u, const matrix &adj, std::vector<bool> &seen, std::vector<int> &matchR)
  {
    for (int v : adj[u])
    {
      if (!seen[v])
      {
        seen[v] = true;
        // if not matched or can restructure matched nodes
        if (matchR[v] < 0 || bip_match(matchR[v], adj, seen, matchR))
        {
          matchR[v] = u;
          return true;
        }
      }
    }
    return false;
  }
  ```,
  caption: [Función que intenta hacer un match del nodo $u$ con el grafo derecho.],
)

Una vez realizado el match, solamente generamos los caminos y regresamos el cálculo de la fórmula para los _min paths_.

Para compilar el programa, usamos
#figure(
  ```sh
  g++ dag_cover.cpp -o cover
  ```,
)
Luego, ejecutamos usando
#figure(
  ```sh
  ./cover
  ```,
)

Para el input, la primera línea es el número $N$ de vértices, la segunda el número de aristas, luego en pares, las aristas entre los vértices. Por ejemplo,

#figure(
  ```
  6
  6
  5 2
  5 0
  4 0
  4 1
  2 3
  3 1
  ```,
)

Visualmente, tenemos el siguiente grafo:
#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `5`, name: <s>),
    edge(<s>, "-|>", <a>, label-size: 8pt, []),
    edge(<s>, "-|>", <b>, label-size: 8pt, []),
    node((1, -1), `2`, name: <a>),
    edge(<a>, "-|>", <c>, label-size: 8pt, []),
    node((1, 1), `0`, label: "b", name: <b>),
    edge(<b>, "<|-", <d>, label-size: 8pt, []),
    node((3, -1), `3`, label: "c", name: <c>),
    edge(<c>, "-|>", <t>, label-size: 8pt, []),
    node((3, 1), `4`, label: "d", name: <d>),
    edge(<d>, "-|>", <t>, label-size: 8pt, []),
    node((4, 0), `1`, label: "t", name: <t>),
  ),
  caption: [Ejemplo de DAG a particionar.],
)

Esto nos genera el output:

#figure(
  ```
  Min Path Count:
  2
  PATHS:
  4 -> 0
  5 -> 2 -> 3 -> 1
  ```,
)

Entonces, nuestro grafo se descompone en los siguientes caminos:
#figure(
  diagram(
    node-stroke: 1pt,
    node((0, 0), `5`, name: <s>),
    edge(<s>, "-|>", <a>, stroke: red, label-size: 8pt, []),
    node((1, -1), `2`, name: <a>),
    edge(<a>, "-|>", <c>, stroke: red, label-size: 8pt, []),
    node((1, 1), `0`, label: "b", name: <b>),
    edge(<b>, "<|-", <d>, stroke: blue, label-size: 8pt, []),
    node((3, -1), `3`, label: "c", name: <c>),
    edge(<c>, "-|>", <t>, stroke: red, label-size: 8pt, []),
    node((3, 1), `4`, label: "d", name: <d>),
    node((4, 0), `1`, label: "t", name: <t>),
  ),
  caption: [Ejemplo de DAG particionado.],
)

== Complejidad

La complejidad temporal es $O(V E)$. Esto se debe a lo siguiente:

#figure(
  ```cpp
  for (int i = 0; i < V; i++)
  {
    std::vector<bool> seen(V, false);

    if (bip_match(i, adj, seen, matchR))
    {
      max_matching++;
    }
  }
  ```,
)
Este bucle hace $V$ veces la llamada `bip_match`. Por su definición, esta misma
es $O(E)$, ya que revisamos las aristas del grafo (en el peor caso todas). La
complejidad espacial es solamente $O(V+E)$, ya que `seen`, `matchL` y `matchR`
son de tamaño $V$, y la matriz de adyacencia es tamaño $O(V+E)$.

= Análisis comparativo de algoritmos Dinic vs Hopcroft-Karp
== Hopcroft-Karp
El algoritmo de Hopcroft-Karp @hopcroft1973, @cormen2009 tiene el objetivo de encontrar el matching bipartito maximo. 
La manera en la que este algoritmo mejora al previamente visto de complejidad
$O(V E)$, es que encuentra varios caminos aumentantes en cada iteración en diferentes fases.

1. Se realiza un BFS de cada nodo que no está emparejado del lado izquierdo del
doble grafo. La distancia al nodo sin match más cercano es marcada. El BFS deja
de buscar para distancias mayores. Esto organiza el grafo por niveles.

2. Luego, se usa DFS con los niveles construidos por el BFS. Buscamos el
conjunto maximal de caminos que no comparten vértice. 

3. Invertimos los vértices, de tal manera que los 
== Dinic
El algoritmo de Dinic @dinitzAlgorithmSolutionProblem1970, @cpalgodinic,
descubierto por Yefim Dinitz en 1970, consiste en resolver el problema de flujo
máximo.

El algoritmo se enfoca en saturar las aristas hasta que no pueda haber más flujo 

#bibliography("ref.bib")