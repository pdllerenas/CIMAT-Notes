#import "@preview/unequivocal-ams:0.1.2": ams-article, definition, proof, theorem
#import "@preview/lovelace:0.3.0": *
#import "@preview/lilaq:0.5.0" as lq

#set math.equation(numbering: "(1)")

#show: ams-article.with(
  title: [Gradient Descent with Armijo Condition],
  authors: (
    (
      name: "Gerardo Rocha",
      email: "gerardo.rocha@cimat.mx",
    ),
    (
      name: "Pedro Llerenas",
      email: "pedro.llerenas@cimat.mx",
    ),
  ),
  abstract: [We present an implementation of gradient descent using the Armijo
    condition for the step size. We present a performance test via the Rosenbrock
    function, and perform an application on the iris dataset.],
  bibliography: bibliography("refs.bib"),
)

In general, it is seldom possible to find an explicit local solution to the minimization problem
$
  min_(x in RR^n) f(x)
$
In practice, iterative methods are used to approximate local minimizers. Convergence of a possible algorithmic approach is analyzed and then implemented in some programming language. In our case, we chose Julia (to learn the syntax).

Suppose $f:RR^n->RR$ is a continuously differentiable function. We follow these steps to approximate a solution to the minimization problem:
1. At a point $x in RR^n$, one chooses a direction $d in RR^n$ in which the function value decreases (*descent method*).
2. Starting at $x$, one proceeds along the direction $d$ as long as the function value of $f$ _reduces sufficiently_. (*step size strategy*).
Formally, we describe it as the following.
#definition[
  Let $f: RR^n$ and $x in RR^n$. The vector $d in RR^n$ is called a _descent direction_ of $f$ at $x$, if there exists if there exists an $hat(a) > 0$ such that
  $
    f(x + alpha d) < f(x) " for all " alpha in (0, hat(alpha)].
  $
]
If $f$ is continuously differentiable at $x in RR^n$, then it is sufficient to show
$
  nabla f^top d < 0
$
to conclude that $d$ is a descent direction. For example, the negative gradient is a descent direction, as
$
  - nabla f^top nabla f = - norm(nabla f) < 0.
$
= General Descent Method
The following algorithm presents a general descent method. The direction and conditions for the step size are not explicitly implemented.
#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [General Descent Method])[
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
The convergence criterion can be either a maximum number of iterations reached, or a norm convergence on the gradient, which we wish to nullify.

= Armijo Rule
Let $sigma in (0,1)$ be fixed. The _Armijo rule_ is a condition on the step size which ensures a sufficient descent in the following sense:
$
  f(x+alpha d) <= f(x) + sigma alpha nabla f(x)^top d
$<armijo>
That is, we restrict the step size to one which ensures that the function at the next iteration is below the line $f(x) + sigma alpha nabla f(x)^top d$. Note that this line depends linearly on $alpha$, so if we take bigger steps, the function must be much smaller.

In general, one may modify the step size to approach the required condition in various ways. The approach we take is to simply multiply by a constant factor $rho = 0.5$.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Armijo condition])[
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
) <algo:armijo>

It is possible to prove that these algorithms theoretically converge for some initial conditions. See @Hintermuller.

= The Rosenbrock function
The $n$-dimensional _Rosenbrock function_ is defined as
$
  f(x_1, ..., x_n) = sum_(i=1)^(n-1) [100(x_(i+1) - x_(i)^2)^2 + (1-x_i)^2].
$
It is _the_ function used to test optimization algorithms, as its global minimum is inside a long, narrow parabolic-shaped valley. It can be shown (see previous Homework) that the global minimum is at the point $x = (1,...,1)$.

== Performance test
=== Implementation
The implementation of the algorithms can be found in `gradient.jl`. To run the code, in the terminal execute the following command (while `cd`'d into the repository)
#figure(
  ```sh
  julia --project=. gradient.jl
  ```,
)

This will print the solutions found after convergence (either in norm or maximum iterations). The default seed is 2026, and can be changed manually inside the code. Additionally, two `csv` log files are created. `rosebrock_log.csv` contains the iteration information on the gradient descent method applied to the Rosenbrock function. The `step_log.csv` file contains information on the step size at each iteration.

=== Results
With the following initial conditions,
$
            sigma & colon.eq 10^(-4), \
              rho & colon.eq 0.5, \
            k_max & colon.eq 10000, \
  epsilon_(t o l) & colon.eq 10^(-6).
$
We will test two initial points for the algorithm. First, the _classical_ initialization:
$
  x_(2i - 1)^((0)) = -1.2,quad x_(2i)^((0))=1 quad i in {1,..., 64}
$
And the other initialization will be random, given a set seed.

#let c_rosenbrock = csv("classical_rosenbrock.csv")
#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header([*Iteration*], [*Time*], $bold(f(x_k))$, $norm(bold(nabla f(x_k)))$),
    table.hline(),
    ..c_rosenbrock.slice(1, 6).flatten(),
    $dots.v$,
    $dots.v$,
    $dots.v$,
    $dots.v$,
    ..c_rosenbrock.slice(1000, 1001).flatten(),
    $dots.v$,
    $dots.v$,
    $dots.v$,
    $dots.v$,
    ..c_rosenbrock.slice(10000 - 4, 10001).flatten(),
    table.hline(),
  ),
  caption: [Rosenbrock function with classical initialization performance test with gradient descent and Armijo condition.],
)

#let r_rosenbrock = csv("random_rosenbrock.csv")
#figure(
  table(
    columns: 4,
    stroke: none,
    table.hline(),
    table.header([*Iteration*], [*Time*], $bold(f(x_k))$, $norm(bold(nabla f(x_k)))$),
    table.hline(),
    ..r_rosenbrock.slice(1, 6).flatten(),
    $dots.v$,
    $dots.v$,
    $dots.v$,
    $dots.v$,
    ..r_rosenbrock.slice(1000, 1001).flatten(),
    $dots.v$,
    $dots.v$,
    $dots.v$,
    $dots.v$,
    ..r_rosenbrock.slice(10000 - 4, 10001).flatten(),
    table.hline(),
  ),
  caption: [Rosenbrock function with random initialization performance test with gradient descent and Armijo condition.],
)

=== Comparison

#figure(
  table(
    stroke: none,
    columns: 6,
    table.hline(),
    table.header([*Init*], [*Iteration*], [*Time (s)*], $f_("final")$, $norm(nabla f_("final"))_2$, [*Stop Reason*]),
    table.hline(),
    [Classical], [10000], [2.7052325], [51.32370], [3.2776], [Max iterations],
    [Random \ (seed=2026)], [10000], [2.00332], [$2.1e-6$], [0.0032], [Max iterations],
    table.hline(),
  ),
)

=== Interpretation
The classical initialization starts far from the solution ($x = (1, ..., 1)$). This means that for odd indices of the sum, the partial derivative in that coordinate expects the even index to be close to 1.44 (to nullify the derivative), while on even indices, it is the opposite; it wants the odd index to be 1, while being -1.2. This effect is present at all iterations, and so the norm of the gradient does not seem to converge at the 10,000 iterations.
On the random initialization, the values are chosen from 0 to 1 uniformly. These are all _close_ 1 in space, and contrary to the previous example, these do seem to get close to the solution in the limited iterations (with the given tolerance).
The only execution time difference was in the first iteration, where the value of the solution is far off from the theoretical solution.

After $k = 10000$ iterations, the classical initialization ended with $norm(nabla f(x_k)) = 3.27$, while the random initialization ended with $norm(nabla f(x_k)) = 0.003$. Therefore, the random initialization performed better than the classical one.

=== Step Analytics
// classical
#let c_step = csv("classical_rosenbrock step_log.csv").slice(1)
#let c_values = c_step.map(row => float(row.at(1)))
#let c_sorted = c_values.sorted()
#let n = c_sorted.len()

#let c_median = if (calc.rem(n, 2) == 0) { c_sorted.at(int(n / 2)) } else {
  (c_sorted.at(int(n / 2 - 1)) + c_sorted.at(int(n / 2)) / 2)
}

#let c_count = c_values.filter(v => v == 1.0).len()
#let c_mean_b = (c_step.map(row => int(row.at(2))).sum()) / n

#let r_step = csv("random_rosenbrock step_log.csv").slice(1)
#let r_values = r_step.map(row => float(row.at(1)))
#let r_sorted = r_values.sorted()
#let n = r_sorted.len()

#let r_median = if (calc.rem(n, 2) == 0) { r_sorted.at(int(n / 2)) } else {
  (r_sorted.at(int(n / 2 - 1)) + r_sorted.at(int(n / 2)) / 2)
}

#let r_count = r_values.filter(v => v == 1.0).len()
#let r_mean_b = (r_step.map(row => int(row.at(2))).sum()) / n

The step sizes at each iterations can be summarized in the following table.
#figure(
  table(
    stroke: none,
    columns: 4,
    table.hline(),
    table.header([*Init*], $bold(%{alpha_k = 1})$, [*Median $alpha_k$*], $bold(dash(b))$),
    table.hline(),
    [Classical], [#{ c_count / n }], [#{ c_median }], [#{ c_mean_b }],
    [Random], [#{ r_count / n }], [#{ r_median }], [#{ r_mean_b }],
    table.hline(),
  ),
  caption: [Comparison of step analytics.]
)
Note that we had no steps of size 1. In fact, they were quite small. This is likely due to the _ugliness_ of the Rosenbrock function, which forms a valley, and thus it cannot make big steps without overshooting.

#let x = lq.arange(0, 100)


#align(start,
lq.diagram(
  title: lq.title([First 100 steps of Classical and Random initializations.]),
  lq.plot(x, x => c_values.slice(0,100).at(x), label:"Classical"),
  lq.plot(x, x => r_values.slice(0,100).at(x), label:"Random"),
))

#align(end,
lq.diagram(
  title: lq.title([Last 100 steps of Classical and Random initializations.]),
  lq.plot(x, x => c_values.slice(10000-100,10000).at(x), label:"Classical"),
  lq.plot(x, x => r_values.slice(10000-100,10000).at(x), label:"Random"),
))
At the start of the iterations, there is one big step, and then a stabilization happens. As we approach the minimum, the step oscillates between two values, although these are both quite small.