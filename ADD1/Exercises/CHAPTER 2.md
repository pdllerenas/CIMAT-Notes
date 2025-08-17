
2.6 When $P(A) = 1/3$, $P(B) = 1/2$ and $P(A\cup B) = 3/4$, what is
- $P(A\cap B)$?
- $P(A^c\cup B^c)$?
*Proof*.
	$$P(A\cap B) =P(A) + P(B) - P(A\cup B) = 1/3 + 1/2 - 3/4 = 4/12 + 6/12 - 9/12 = 1/12$$
	$$P(A^c\cup B^c) = P((A\cap B)^c) = 1 - P(A\cap B) = 1 - 1/12 = 11/12$$

2.7. Let $A$ and $B$ be two events. Suppose that $P(A) = 0.4$, $P(B) = 0.5$, and
$P(A \cap B) = 0.1$. Find the probability that $A$ or $B$ occurs, but not both.
*Proof*.
	$$P(A \oplus B) = P(A\backslash B) + P(B\backslash A) = P(A) - P(A\cap B) + P(B) - P(B\cap A) = 0.4 + 0.5 - 0.2 = 0.7$$

2.8 Suppose the events $D_1$ and $D_2$ represent disasters, which are rare:
$P(D_1) \leq 10^{-6}$ and $P(D_2) \leq 10^{-6}$ . What can you say about the probability
that at least one of the disasters occurs? What about the probability that
they both occur?

*Proof.*
$$
\begin{align*}
P(D_1\cup D_2) &= P(D_1) + P(D_2) - P(D_1\cap D_2)\\
&\leq P(D_1) + P(D_2)\\
&\leq 2\times 10^{-6}
\end{align*}
$$
$$
P(D_1\cap D_2) \leq \max\{P(D_1), P(D_2)\} = 10^{-6}
$$
