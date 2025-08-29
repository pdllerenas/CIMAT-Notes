We have a group of n people, what is the probability that at least one of  them was born in October 12?

$$
\Omega = \{(i_{1}, \dots, i_{n}) | 1\leq i_{j} \leq 365\}
$$
$$
E = \{(i_{1}, \dots, i_{n})\in \Omega:\exists k : i_k = 285\}
$$

$$
P(E) = 1 - P(E^c) = \frac{364^n}{365^n}
$$

2. We have a group of n people. What is the probability at least two of them were born in the same day?

$$
E = \{(i_{1}, \dots, i_{n})\in \Omega:\exists j, k : j\neq k \wedge i_{k} = i_{j}\}
$$
$$
P(E) = 1- P(E) = \frac{365\cdot 364 \cdots (365 - n + 1)}{365^n}
$$