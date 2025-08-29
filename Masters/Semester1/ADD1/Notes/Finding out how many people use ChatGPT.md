
### Exercise:
We test individuals, each tossing a coin. If it lands tails, they answer whether they use chatgpt or not. If it lands heads, they answer if their blood type is O.

Events:
-  Coin lands on tails:  S
- Answer "no":  N
- Blood type O:  O
-  Answer no to chatgpt:  H

$$
P(N) = P(N|S) P(S) + P(N|S^C)P(S^C)
$$
$$
P(N|S) = P(\text{honest}|S)
$$
$$
P(N|S^C) = P(O|S^C)
$$
We assume that
$$
P(O|S^C) = P(O)
$$
$$
P(\text{honest}|S) = P(\text{honest})
$$
$$
P(N) = \frac{P(\text{honest})}{2} + \frac{P(O)}{2}
$$
$$
2P(N) - P(O) = P(\text{honest})
$$