#import "@preview/unequivocal-ams:0.1.2": ams-article, definition, proof, theorem
#import "@preview/lovelace:0.3.0": *
#import "@preview/lilaq:0.5.0" as lq

#set par(justify: true, first-line-indent: (amount: 2em, all: true))
#set math.equation(numbering: "(1)")

#show link: set text(blue)

#show: ams-article.with(
  title: [Logistic Regression and Trust-Region Optimization],
  authors: (
    (
      name: "Pedro Llerenas",
      email: "pedro.llerenas@cimat.mx",
    ),
  ),
  abstract: [We study the optimization problem arising in logistic regression
    for binary classification using the
    #link("http://yann.lecun.com/exdb/mnist/")[MNIST dataset]. Using this dataset,
    we compare the performance of steepest descent with backtracking and the
    dogleg trust-region method.],

  bibliography: bibliography("refs.bib"),
)

= Introduction

Let ${(x_i, y_i)}_(i = 1)^n$ be a dataset with $x_i in RR^(784)$ and $y_i in {0,1}$. We define the parameter vector
$
  z = vec(beta, beta_0) in RR^(784 + 1).
$
Let $tilde(x)_i = vec(x_i, 1)$. The logistic model defines the probability
$
  pi_i (z) = 1/(1+exp(-x_i^top beta-beta_0)) = 1/(1+exp(-tilde(x)_i^top z)).
$
The log-likelihood function is
$
  h(z) = sum_(i =1)^n y_i log pi_i + (1-y_i) log(1-pi_i).
$
Estimating the parameters corresponds to maximizing $h(z)$. For convenience, we
define the negative log-likelihood
$
  f(z) = -h(z),
$
so that the problem becomes a minimization problem.

We note that
$
  pi_i (z) & = 1/(1+exp(-tilde(x)_i^top z)) \
           & = 1 - exp(-tilde(x)_i^top z)/(1+exp(-tilde(x)_i^top z)) \
           & = 1 - pi_i (-z).
$
Therefore,
$
  nabla_z pi_i (z) & = pi_i^2(z)(-exp(tilde(x)_i^top z)tilde(x)_i) \
                   & = pi_i (z) (1-pi_i (-z))(-exp(-tilde(x)_i^top z)tilde(x)_i) \
                   & = pi_i (z) (1 - pi_i (z))tilde(x)_i.
$

Now, we may see that the gradient of the objective function is given by
$
  nabla_z f(z) &= sum_(i = 1)^n y_i (nabla_z pi_i (z)) / (pi_i (z)) + (1 - y_i) (-nabla_z pi_i (z))/(1-pi_i (z))\
  &= sum_(i = 1)^n y_i (pi_i (z)(1-pi_i (z))tilde(x)_i) / (pi_i (z)) - (1 - y_i) (pi_i (z)(1-pi_i (z))tilde(x)_i)/(1-pi_i (z))\
  &= sum_(i = 1)^n y_i (1-pi_i (z)) tilde(x)_i - (1 - y_i) pi_i (z)tilde(x)_i\
  &= sum_(i = 1)^n (y_i - pi_i (z))tilde(x)_i\
$
And thus, the Hessian is
$
  nabla_z^2 f(z) & = sum_(i = 1)^n pi_i (z)(pi_i (z) - 1)tilde(x)_i tilde(x)_i^top.
$



= Steepest descent with backtracking
#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [General Line Search Method])[
    + *input* $f: RR^n -> RR$, starting point $x_0 in RR^n$
    + *begin*
      + $k colon.eq 1$
      + *while* converge criterion not satisfied
        + specify a descent direction $d^k$ of $f$ at $x^k$.
        + determine a step size $alpha_k > 0$ with $f(x^k + alpha_k d^k) < f(x^k)$.
        + set $x^(k+1) colon.eq x^k + alpha_k d^k$
        + set $k colon.eq k+1$
      + *end*
    + *end*
  ],
) <algo:general>
The algorithm given in @algo:general is a generalized line search. The steepest
descent corresponds to choosing the direction with greatest change, i.e., the
(normalized) gradient of the function. Therefore, we use $d_k = nabla f(x_k)
eq.colon g_k$.

== Step size
=== Armijo Rule
Let $sigma in (0,1)$ be fixed. The _Armijo rule_ is a condition on the step size which ensures a sufficient descent in the following sense:
$
  f(x+alpha d) <= f(x) + sigma alpha nabla f(x)^top d
$<armijo>
That is, we restrict the step size to one which ensures that the function at the next iteration is below the line $f(x) + sigma alpha nabla f(x)^top d$. Note that this line depends linearly on $alpha$, so if we take bigger steps, the function must be much smaller.

In general, one may modify the step size to approach the required condition in various ways. The approach we take is to simply multiply by a constant factor $rho = 0.5$.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Backtracking])[
    + *input* descent direction $d$
    + *begin*
      + $l colon.eq 0$
      + $alpha^((0)) colon.eq 1$
      + *while* @armijo not satisfied
        + determine $alpha^((l+1)) = alpha^((l)) * rho$
        + set $l colon.eq l + 1$
      + *end*
      + $alpha_k = alpha^((l))$
    + *end*
  ],
  caption: [Backtracking algorithm to determine a step size.],
) <algo:armijo>

= Trust Region Methods
== General idea
Trust region methods are similar to line search algorithms, in that to find a step, one approximates the objective function. In each iteration, these methods define a _trust region_, in which we assume that our model will adjust well to the objective function.

Let $m_k (p)$ denote the model of the objective function. The step is calculated
as an approximate minimizer of the model $m_k (p)$, restricted to the trust region $cal(R)_c$.
$
  p_k = arg min_p m_k (p), " s.t. " p in cal(R)_c.
$
The descent direction and the step size are calculated simultaneously. This differs from the traditional line search method.

If the step obtained in the trusted region did not adjust _properly_ to the
function, then we reduce the size of our trust region. Then, a new step is
calculated.
== Previous Assumptions
We will assume that the model $m_k$ is quadratic at each $x_k$. By Taylor's approximation of second order,
$
  f(x_k + p) = f(x_k) + nabla f(x_k)^top p + 1/2 p^top nabla^2f(x_k+t p)p
$
with $t in (0,1)$. If we approximate the Hessian matrix, we have
$
  m_k (p) = f(x_k) + nabla f(x_k)^top p + 1/2 p^top B_k p
$
for some symmetric $B_k$.

We will also assume that the trust region is a circle, and its size will be
completely determined by its radius.
== Step search method
To find an appropriate step, we solve the following restricted optimization problem:
$
  p_k^* = arg min_p m_k (p) = f(x_k) + nabla f(x_k)^top p + 1/2 p^top B_k p
$
such that $norm(p) <= Delta_k$, where $Delta_k$ is a the radius of the trust
region. If moreover $B_k succ 0$, the solution is given by
$
  p^* = B^(-1)_k nabla f(x_k).
$
If additionally $norm(p^*) <= Delta_k$, then the solution to the problem is
precisely the one given above: $p_k^* = B_k^(-1) nabla f(x_k)$.

To ensure that our trust region radius is _good_ enough, we measure it
with the following constant:
$
  rho_k = (f(x_k) - f(x_k + p_k))/(m_k (0) - m_k (p_k)).
$
Given that for all $k$ we have $m_k (0) > m_k (p_k)$, the sign of the constant
only relies on the numerator. That is, if $rho_k < 0$, then $f(x_k) < f(x_k + p_k)$
and thus the function increases, so we dispose of this $p_k$. If $p_k approx 1$, then
the model approaches the behavior of the objective function, so we increment the radius
of our trust region, in search for even better steps. If $0 << p_k < 1$ but
close to 1, then $Delta_k$ is not modified in the next iteration, so
$Delta_(k+1) = Delta_k$. If $rho_k > 0$ and close to 0, we reduce our trust
region.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Trust Region])[
    + *input* $hat(Delta) > 0$, $Delta_0 in (0,hat(Delta))$, $eta in [0, 1/4)$
    + *begin*
      + *for* $k = 1, 2, ...$
        + Compute $p_k$ (minimize quadratic model in the trust region)
        + Compute $rho_k$
        + *if* $rho_k > eta$
          + $x_(k+1) = x_k + p_k$
        + *else*
          + $x_(k+1) = x_k$
        + Compute $Delta_(k+1)$ given ($rho_k, Delta_k, hat(Delta_k), eta$)
      + *end*
    + *end*
  ],
) <algo:trust>

#theorem[$p^*$ is a global solution to the problem
  $
    min_p m_k (p) = f(x_k) + g^top p + 1/2 p^top B p
  $
  such that $norm(p) <= Delta$ if and only if, $p^*$ is feasible (i.e. the norm is
  bounded by $Delta$) and there exists $lambda >= 0$ such that
  $
               (B+lambda I)p^* & = -g \
    lambda (norm(p^*) - Delta) & = 0 \
                  B + lambda I & succ.eq 0
  $
]
We note that if $lambda = 0$ then $B p^* = -g$. In particular, if $B succ 0$,
then $p^* = -B^(-1)g$ is the approximated Newton step. If $lambda > 0$, then $B
p^* + lambda p^* = -g$, so $lambda p^* = - (B p^* + g)$ and thus $p^* =
-1/lambda nabla m_k (p^*)$. Therefore, $p^*$ is parallel to $nabla m_k (p^*)$,
so $p^*$ is the steepest descent direction.

== Cauchy Point
The _Cauchy Point_ is the minimizer to the model $m_k$ along the direction of
steepest descent of the function $f$, that is, $-nabla f(x_k)$ restricted to the
trust region.

To find this step, we solve the following restricted optimization problem:
$
  p_k^* = arg min_p m_k (p) = f(x_k) + nabla f(x_k)^top p + 1/2 p^top B_k p,
$
such that $norm(p) <= nabla_k$. In practice, an approximation to this solution
is enough. This point quantifies sufficient descent in a model. The Cauchy Point is
denoted by $p_k^C$.

In an algorithm, we first need to solve the linear problem
$
  p_k^S = arg min_p f(x_k) + nabla f(x_k)^top p " s.t. " norm(p) <= Delta_k
$<eq:cauchy1>
Then we find $tau_k >0$ that solves
$
  tau_k = arg min_(tau >= 0) m_k (tau p_k^S), " s.t. " norm(tau p_k^S) <= Delta_k
$<eq:cauchy2>
Then, we calculate the Cauchy point as $p_k^C = tau_k p_k^S$. We describe this
in the following algorithm:

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Cauchy Point])[
    + *input* $f(x_k)$, $nabla f(x_k)$, $Delta_k$
    + *begin*
      + Find $p_k^S$ that solves @eq:cauchy1
      + Find $tau_k$ that solves @eq:cauchy2
      + *return* $p_k^C = tau_k p_k^S$
    + *end*
  ],
) <algo:cauchy>

We may prove that this leads to
$
  p_k^C = - tau_k (Delta_k)/norm(nabla f(x_k)) nabla f(x_k)
$
for
$
  tau_k = cases(1 & "if" nabla f_k^top B_k nabla f_k <= 0, min(1, norm(nabla f_k)^3/(Delta_k nabla f_k^top B_k nabla f_k)) &"otherwise")
$

== Dogleg Method
An augmented method that allows for better performance is the _dogleg_ method. We combine
the strengths of the Cauchy point and add in some optimizations for particular cases.

We assume that $B_k$ is a positive definite matrix. Otherwise, we simply use
Cauchy's point. The optimal point in a trust region $Delta in [0, norm(p_k^B)]$ the
optimal trajectory is given in @fig:dogleg1.

#figure(
  image("images/optimal1.svg"),
  caption: [How a dogleg path is constructed.],
)<fig:dogleg1>

The main idea of the dogleg method is to:
- minimize the model along the gradient $p_k^U = alpha nabla f_k$.
- minimize the unrestricted quadratic model if $B_k$ is positive definite and obtain the complete step
  $p_k^B= - B_k^(-1)nabla f_k$
- compute the step size of $p_k = F(p_k^U, p_k^B)$, a function that depends on
  both the complete step and the step of steepest descent.

The dogleg trajectory has the following parametrization:
$
  tilde(p)(tau) = cases(tau p_k^U & " if " 0<= tau <= 1, p_k^U + (tau - 1)(p_k^B - p_k^U) & " if " 1<= tau <= 2)
$<cases:ptau>
One then finds that
$
  p_k^U & = - (nabla f_k^top nabla f_k)/(nabla f_k^top B_k nabla f_k)nabla f_k \
  p_k^B & = -B_k^(-1)nabla f_k
$
So we need to find a minimizer $tau^*$ for @cases:ptau.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Dogleg Trust Region])[
    + *input* $hat(Delta) > 0$, $Delta_0 in (0, hat(Delta))$ and $eta in [0,1/4]$
    + *begin*
      + *for* $k = 0,1,2,...$
      + *begin*
        + Compute $p_k$ using Dogleg
        + Compute $rho_k$
        + *if* $rho_k > eta$
          + $x_(k+1) = x_k + p_k$
        + *else*
          + $x_(k+1) = x_k$
        + Compute $Delta_(k+1)$ using $(rho_k, p_k, Delta_k, hat(Delta)_k)$
    + *end*
  ],
) <algo:doglegregion>

= Implementations
We use @Julia-2017 to implement both algorithms. We provide some snippets below, but the full code can be found attached to this document.
== Line Search with Backtracking
#figure(
  ```julia
  for k in 1:max_iter
  	gₖ = g(xₖ)

  	if norm(gₖ) < tol
  		println("Converged in $k iterations.")
  		return xₖ, f(xₖ), k
  	end

  	pₖ = -gₖ
  	α_guess = min(1.0, 1.0 / norm(gₖ))

  	αₖ = backtracking_armijo(f, gₖ, xₖ, pₖ, α₀ = α_guess)

  	xₖ += αₖ * pₖ
  end
  ```,
  caption: [Snippet of steepest descent with Armijo backtracking.],
)

== Dogleg
#figure(
  ```julia
  for k in 1:max_iter
  	gₖ = g(xₖ)
  	if (norm(gₖ) < tol)
  		println("Converged in $k iterations.")
       return xₖ, f(xₖ), k
  	end

  	Bₖ = H(xₖ)
  	pₖ = step_finder(gₖ, Bₖ, Δₖ)

  	reduction = f(xₖ) - f(xₖ + pₖ)
  	model_reduction =-(dot(gₖ, pₖ) + 0.5 * dot(pₖ, Bₖ * pₖ))

  	ρₖ = reduction / model_reduction
     if ρₖ < 0.25
       Δₖ *= 0.25
     elseif (ρₖ >= 0.75 && isapprox(norm(pₖ), Δₖ, atol=1e-4))
       Δₖ = min(2*Δₖ, Δ)
     end

  	if ρₖ > η
  		xₖ += pₖ
  	end
  end
  ```,
  caption: [Snippet of dogleg trust region method. We use the dogleg method as the `step_finder` function.],
)

= Results
To run the experiments, use
#figure(
  ```sh
  julia main.jl
  ```,
)
this will print the time taken by each method, its iterations, the plots will be
saves as images, and the error is printed.

We run the methods with 1 iteration to compile the code (Julia is JIT compilation), so that the actual tests to not take the compilation time into account.

Note that we capped the iterations of the steepest descent to 5000, as it seemed pointless to wait for it to converge. The difference is clear.

The time taken for the steepest descent was 124.63 seconds (did not converge after 5000 iterations), while the time for the trust region took only 5.39 seconds.

#figure(
  table(
    columns: 5,
    stroke: none,
    table.hline(),
    table.header([Method], [Time], [$f_k$], [$norm(g_k)$], [Iterations]),
    table.hline(),
    [Gradient Descent], [124.63], [0.0318],[0.0001], [5000],
    [Dogleg], [5.39], [0.0318], [7.86e-7],[23],
    table.hline(),
  ),
)

#figure(
  image("plot_fk.png"),
  caption: [Iterations vs $f(x_k)$. Both methods arrive to zero rapidly. However, the steepest descent method does not converge rapidly. See the next graph.],
)
#figure(
  image("plot_gk.png"),
  caption: [Iterations vs gradient. The trust region converges rapidly, while gradient descent follows an exponential decrease. The log scale makes this look nicer.],
)

#figure(
  image("plot_deltak.png"),
  caption: [Iterations vs $Delta_k$. The region stabilizes to the maximum set region size after 5 iterations.],
)
== Classification Error
We define this as
$
  "error" = 1/n sum_(i = 1)^n abs(bold(1)_(pi_i (hat(z)) > 0.5) - y_i)
$
In both cases, we obtain an error of $0.05%$. Note that minimizing the objective function and minimizing the error function are different problems, we cannot apply the methods we described earlier as the error function is not differentiable. However, minimizing the error function is more practical from a real-world viewpoint.

= Conclusions
The trust region method allows for a faster convergence on the MNIST dataset. Although each iteration is more expensive, the number of iterations needed to converge is significantly lower than steepest descent. Both methods obtain an error in classification of $0.05%$.
