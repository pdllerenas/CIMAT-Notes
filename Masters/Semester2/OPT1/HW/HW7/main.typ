#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lilaq:0.6.0" as lq
#import "@preview/frame-it:2.0.0": *
#import "@preview/lovelace:0.3.1": *
#import exercise: project, subtask, task
#set page(numbering: "1")
#set heading(numbering: "1.")
#set math.equation(numbering: "(1)")

#let (example, feature, variant, syntax) = frames(
  feature: ("Feature", eastern),
  // For each frame kind, you have to provide its supplement title to be displayed
  variant: ("Variant", olive),
  // You can provide a color or leave it out and it will be generated
  example: ("Example", gray),

  syntax: ("Syntax",),
)
// This is necessary. Don't forget this!
#show: frame-style(styles.boxy)

#set text(lang: "en")

#show: project.with(
  title: "Homework 7",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],
  semester: "Semester 2",
  abstract: none,
  author: "Pedro D. Llerenas",
)


#feature[Exercise 1][Hessian of functions][
  1. Determine the Hessians of the functions defined on page 4 and implement them.
  2. Write a function whose input is a symmetric matrix, and such that:
    - the function computes the eigenvalues of the matrix. You can use the function `numpy.linalg.eigvalsh()` to compute the eigenvalues.
    - the function returns the highest and lowest eigenvalues of the matrix.
  3. Write a function which receives the highest and lowest eigenvalues $lambda_n$ and $lambda_1$ of a symmetric matrix. The function prints $lambda_1, lambda_n$, and a message according to the following conditions:
    - If $lambda_1 > 0$, print *Definite positive matrix*.
    - If $lambda_n < 0$, print *Definite negative matrix*.
    - If $lambda_1 < 0$ and $lambda_n >0$, print *Indefinite matrix*.
]

#example[Solution][
  See `sample_functions.py` and `p1.py`.
]

#feature[Exercise 2][Joined gradient descent and Newton method][
  The function to implement receives the following parameters:
  - The objective function $f:RR^n -> RR$,
  - The gradient $nabla f$ of the objective function,
  - The hessian $nabla^2 f$ of the objective function,
  - The initial point $x_0$,
  - The maximum number of iterations $N$,
  - A threshold $tau > 0$,
  - The parameters $alpha_("init"), c_1, rho$, and a maximum number of iterations $N_b$ for the backtracking algorithm.
  The function to implement performs the following steps:
  Fix $m = 0$. For $k = 0, dots, N-1$, do the following steps:
  1. Compute the gradient $g_k$ at the point $x_k$, i.e. $g_k = nabla f(x_k)$.
  2. If $norm(g_k) < tau$, do $"res" = 1$ and stop the loop.
  3. If the stopping criterion is not fulfilled, compute the Hessian at $x_k$: $B_k = nabla^2 f(x_k)$.
  4. Try to perform a Cholesky decomposition of $B_k$:
    - if the decomposition holds, we obtain $B_k = L L^top$. Then, solve the system $L L^top p_k = - g_k$ to obtain the descent direction $p_k$.
    - If the factorization fails, define the descent direction as $p_k = - g_k$, then do $m = m+1$.
  5. Compute the step size $alpha_k$ using the backtracking Algorithm with the condition of sufficient descent.
  6. Compute the updating of the sequence $x_(k+1) = x_k + alpha_k p_k$.

  Return $x_k, g_k, m$ and res.
]

#example[Solution][
  See `p2.py` and `backtracking.py`.
]

#feature[Exercise 3][Strongly convex functions and quasi-Newton methods][
  1. Show that if $g in C^1(RR^n)$ is convex, then
  $
    (nabla g(x) - nabla g(y))^top (x - y) >= 0 quad forall x,y in RR^n.
  $
  2. Deduce from 1. that a strongly convex function $f in C^1(RR^n)$ of parameter $m > 0$ satisfies that
  $
    (nabla f(x) - nabla f(y))^top (x-y) >= m norm(x - y)^2 quad forall x, y in RR^n
  $
  3. Deduce from 2. a property of quasi-Newton methods applied to strongly convex functions.
]

#example[Solution][
  1. The first order condition for convexity states
  $
    g(y) & >= g(x) + nabla g(x)^top (y - x), \
    g(x) & >= g(y) + nabla g(y)^top (x - y) \
  $
  Thus, adding these inequalities gives
  $
    g(x) + g(y) >= g(x) + g(y) + (nabla g(y) - nabla g(x))^top (x-y)\
    <=> (nabla g(x) - nabla g(y))^top (x - y) >= 0.
  $
  2. By definition,
  $
    f(y) & >= f(x) + nabla f(x)^top (y - x) + m/2 norm(y-x), \
    f(x) & >= f(y) + nabla f(y)^top (x - y) + m/2 norm(x-y) \
  $
  and thus, adding both inequalities gives the desired inequality:
  $
    (nabla f(x) - nabla f(y))^top (x-y) >= m norm(x - y)^2
  $
  3. The curvature is strictly positive ($s_k^top y_k > 0$).
]

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Modified BFGS])[
    + *Input* $f(x)$, $nabla f(x)$, $x_0$, $tau > 0$, $H_0$ an approximation of the inverse Hessian of $f$ at $x_0$, $N$ the maximum number of iterations, and the parameters for the backtracking algorithm.
    + Compute $g_0 = nabla f(x_0)$
    + Define $I$ as the identity matrix and res = 0
      + *for* $k = 0, 1, dots, N -1$ *do*
        + *if* $norm(g_k) < tau$ *then*
          + Set res = 1
          + *break*
        + *end*
        + Compute $p_k = -H_k g_k$
        + *if* $p_k^top g_k >0$ *then*
          + $lambda_1 = 10^(-5) + (p_k^top g_k)/(g_k^top g_k)$
          + $H_k = H_k + lambda_1 I$
          + $p_k = p_k - lambda_1 g_k$
        + *end*
        + $x_(k+1) = x_k + alpha p_k$, for $alpha_k$ obtained from backtracking
        + $s_k = x_(k+1) - x_k$
        + $y_k = g_(k+1) - g_k$
        + *if* $y_k^top s_k <= 0$ *then*
          + $lambda_2 = 10^(-5) - (y_k^top s_k^top)/(y_k^top y_k)$
          + $H_(k+1) = H_k + lambda_2 I$
        + *else*
          + $rho_k = 1/(y_k^top s_k)$
          + $H_(k+1) = (I - rho_k s_k y^top_k) H_k (I - rho_k y_k s^top_k) + rho_k s_k s_k^top$
        + *end*
      + *end*
    + *return* $x_k, g_k, k$, res
    + *end*
  ],
)

#feature[Exercise 4][Modified BFGS Algorithm][
  1. Implement the modified BFGS algorithm. If the objective function has 2 variables, make the algorithm return the list of all the points $x_0, dots, x_k$ to be able to use them later on to plot the trajectory.

  2. Test the algorithm on the functions and initial points $x_0$ defined on page 4, where:
  - $H_0$ is the identity.
  - The maximum number of iterations is $N=10000$.
  - The threshold $tau = sqrt(n epsilon_m)$, where $epsilon_m$ is the machine epsilon.
  - For the backtracking algorithm, use $alpha_"init" = 1, c_1 = 0.1, rho = 0.6$, and the maximum number of iterations is $N_b = 100$.

  Print $f(x_0)$ and the following values at each iteration:
  - the iteration number $k$.
  - the norm of the gradient $norm(g_k)$.
  - if $n <= 6$, print the vector $x_k$. Otherwise, print the first and last three components of $x_k$.
  - $f(x_k)$

  If $n = 2$, plot the level lines of the function and the trajectory defined by all the points $x_0, dots, x_k$ generated by the algorithm.
]

#example[Solution][
  See `p4.py`.
]

#figure(
  image("beale.png")
)

#figure(
  image("himmelblau.png")
)

#figure(
  image("rosen2.png")
)

#figure(
  image("rosen200.png")
)

#figure(
  image("rosen600.png")
)