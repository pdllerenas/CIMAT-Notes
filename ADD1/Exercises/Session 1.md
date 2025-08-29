1. Se van a lanzar dos dados. Se tiene interés en predecir la diferencia entre
el valor mas grande y el mas chico. Si tienes que apostar a un resultado
(diferencia) particular ¿cual tomarías? Motiva tu respuesta.

> [!note]- Solution
> We may visualize the space as a 6x6 matrix of the distances. That is,
>$$
 \begin{matrix}
 0 & 1 & 2 & 3 & 4 & 5\\
 1 & 0 & 1 & 2 & 3 & 4\\
 2 & 1 & 0 & 1 & 2 & 3\\
 3 & 2 & 1 & 0 & 1 & 2\\
 4 & 3 & 2 & 1 & 0 & 1\\
 5 & 4 & 3 & 2 & 1 & 0
 \end{matrix}
>$$


2. Si $P(C) = 0.25$, $P(D) = 0.45$ y $P(C \cap D) = 0.1$, calcula $P(C^c \cap D)$. 

> [!note]- Solution
>$$
  P(C^c\cap D) = P(D) - P(C\cap D) = 0.45 - 0.1 = 0.35
>$$


3. El senado de EE.UU consiste de 100 senadores, 2 por cada estado. Se decide formar una comisión. Para eso se eligen 50 senadores al azar. Calcula la probabilidad que Texas este representado.

> [!note]- Solution
> We look at the inverse problem: what is the probability 0 TX senators are chosen:
>$$
>P(TX = 0) = \frac{{98 \choose 50}}{{100 \choose 50}}\\
 P(TX = 0) = \frac{98}{100} \cdot \frac{97}{99} \cdots \frac{49}{51} = \frac{1}{100} \cdot \frac{1}{99} \cdot 50 \cdot 49 = \frac{50 \cdot 49}{99\cdot 100}
>$$
> Therefore,
>$$
  P(TX >= 1) = 1 - P(TX = 0) = 0.7525 
>$$


4. Se eligen al azar 20 cartas de un mazo. ¿Cual es la probabilidad que la ultima carta que se toma sea un trébol? (existe una solución muy cortita).

> [!note]- Solution
> This equivalent to shuffling the deck and choosing the 20th card. The probability of picking a club is 1/4. So is for any other card in the deck.
>$$
 \frac{1}{{52\choose 19}(52-19)}\sum_{m=0}^{13}{13\choose m}{52-13\choose 19-m}(13-m)
>$$

5. Un robot camina de punto $A$ a punto $B$. Siempre hace un paso de tamaño
1: o a la derecha (de $(x, y)$ a ($x+1, y)$) o hacia arriba (de $(x, y)$ a $(x, y+1)$). Si todos los caminos son igual de probables, calcula la probabilidad de pasar por punto $C$.

> [!note]- Solution
> Say we have 
>$$
 A = (x,y), B = (w,z),
>$$
>where we assume $w, x, y,z\in \mathbb{Z}$ with $x < w$ and $y < z$. We are looking for words of length
>$$
 \lVert A - B\rVert_{T} = w-x+z-y
>$$
> where there are exactly $\alpha \coloneqq w-x$ occurrences of East (E) and $\beta \coloneqq z-y$ occurrences of North (N). Thus,
>$$
 \frac{(\alpha + \beta)!}{\alpha! \beta!}
>$$
>possible ways of getting from A to B. Thus, in this particular case, there are
>$$
 \frac{(8+6)!}{8!6!} = \frac{14!}{8!6!}
>$$
>ways of getting from A to B. Now, the ways of getting from A to C are 
>$$
 \frac{5!}{4!} = 5
>$$
>and thus there are 
>$$
 \frac{5 \cdot 8! 6!}{14!} = \frac{5}{3003} \approx 0.001
>$$
