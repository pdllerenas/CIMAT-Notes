### Bayesian introduction

Let $X_{1}, \dots. X_{n}$ be exponential variables. 
$$
f(X_{1}, \dots, X_{n}|\lambda) = \prod_{i=1}^n \lambda e^{-\lambda x_{i}} = \lambda^n e^{\sum X_{i}} = L(\lambda)
$$

$$
\log(L(\lambda)) =  n\log \lambda - \lambda \sum X_{i}
$$
$$
l'(\lambda) =\frac{n}{\lambda} - \sum X_{i} = 0
$$

$$
\lambda \sim \Pi(\lambda), b\Pi(\lambda |X) = \frac{f(X_{1},\dots X_{n}|\lambda)\Pi(\lambda)}{\int f(X_{1}, \dots X_{n}|\omega)\Pi(\omega )dw}
$$

$$
\lambda \sim G(\alpha, \beta) = \frac{\beta^\alpha}{\Gamma(\alpha)}\lambda^{\alpha-1}e^{-\beta \lambda}
$$
$$\Pi(\lambda|X) a \lambda^n e^{-\lambda \sum X_{i}}\lambda^{\alpha -1}e^{-\beta \lambda} a \lambda^{\alpha+n-1}e^{-\alpha\left( \beta+\sum X_{i} \right)}
$$
Heuristically,
$$
\hat{\lambda} = E\lambda|X = \frac{k\alpha+n}{k\beta+\sum X_{i}}
$$
implies for $n\to \infty$, the choice of distribution is irrelevant.
### ARA in a nutshell
Soccer game between A and D, and you are the manager of team D. 
- Think of the strategy the opponent will have. This leads to playing defensive of offensive. Or choose randomly. (**Concept uncertainty**). Uncertainty in the outcome, conditional to the choices of each opponent (statistical risk analysis.)
- You may have some information about their preference to win or draw. (**Epistemic uncertainty**) 
- **Aleatory uncertainty**: 

### Bayesian decision analysis

- Set of alternatives $a\in \mathcal A$
- unknown parameter $\theta$ depending on the state of nature
- consequences $c(a,\theta)$ of action a when $theta$
- utility $u(c(a,\theta))$
- posterior distribution $\pi(\theta|x)$ when observing x
- optimal action satisfies Maximum Subjective Expected Utility Principle MEUP
- $a^*=\arg \max_{a\in A}\int u(c(a,\theta))\pi(\theta|x)d\theta$

$$
\bar{X} = (X_{1},\dots,X_{n})
$$
sample from d.f. $f(x|\lambda)$ and prior $\pi(\lambda)\implies$posterior $\pi(\lambda|\bar{X})$
loss function $L(\lambda,a)\in A$
$\hat{ \lambda}=\arg\min \mathcal E^{\pi(\lambda|\bar{X})}L(\lambda, a)=\arg\min_{a\in A}\int L(\lambda,a)\pi(\lambda|\bar{X})d\lambda$
$$
\mathcal E^{\pi(\lambda|\bar{X})} = \int (\lambda-1)^2\pi(\lambda|\bar{X})d\lambda
$$

$$
d^* = \arg\max_{d\in \mathcal D} \int_{s\in \mathcal S}\int_{a\in \mathcal A} u_{D}(d,a,s)p_{D}(s|a.d)\pi_{D}(a)\,da\,ds
$$

