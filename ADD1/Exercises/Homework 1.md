8. Elige al azar $b, c \in [0, 1]$ de manera independiente. ¿Cual es la probabilidad
que $x^2 + 2bx + c = 0$ tenga dos raíces reales?

> [!note] Solution
> To have real roots, the polynomial must have a non-negative discriminant. That is,
>$$
  b^2-4c \geq 0 \iff b \geq 2\sqrt{ c }.
>$$
>Plotting this in a unit square results in the following graph:
>![[Pasted image 20250821102729.png]]
>The area of this is given by
>$$
\int_{0}^{c} 2\sqrt{ x }dx = \frac{4}{3}c^{\frac{3}{2}},
>$$
>where $c$ is the point where $2\sqrt{ x } = 1$. That is, $x = \frac{1}{4}$. Thus, the probability is $\frac{1}{4}-\frac{4}{3}\left( \frac{1}{4} \right)^{ 3/2 } = \frac{1}{4}-\frac{1}{6} =\frac{1}{12}$.

