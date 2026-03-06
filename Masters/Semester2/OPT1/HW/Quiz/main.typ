#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#import "@preview/lovelace:0.3.0": *
#let question = homework.complex-question
#let ip(x, y) = $lr(angle.l #x, #y angle.r)$

#show: pset.with(
  class: "Optimization I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Quiz 2",
  date: datetime(year: 2026, month: 3, day: 5),
)
#show: great-theorems-init
#let proof = proofblock()
#let mathcounter = rich-counter(identifier: "mathblocks", inherited_levels: 1)
#let theorem = mathblock(
  blocktitle: "Theorem",
  counter: mathcounter,
)

#show link: set text(fill: blue)
#show link: underline

#set math.equation(
  numbering: "(1)",
  supplement: none,
)

#set par(
  justify: true,
  leading: 0.52em,
)

#show ref: it => {
  // provide custom reference for equations
  if it.element != none and it.element.func() == math.equation {
    // optional: wrap inside link, so whole label is linked
    link(it.target)[(#it)]
  } else {
    it
  }
}

#set enum(
  full: true, // necessary to receive all numbers at once, so we can know which level we are at
  numbering: (..nums) => {
    let nums = nums.pos() // just positional args
    let num = nums.last() // just the current level’s number
    let level = nums.len() // level is the amount of numbers available

    // format for current level (or stop at i. If going too deep)
    let format = ("a)", "i)", "i)").at(calc.min(2, level - 1))
    let result = numbering(format, num) // formatted number
    if level < 3 {
      // first 2 levels for example
      strong(result)
    } else {
      result
    }
  },
)


#question[
  Define the following concepts:
  1. Descent direction.
  2. Steepest descent direction.
  3. (Weak) Wolfe conditions.
  4. First Order Necessary Condition for Optimality.
  5. Second Order Necessary Condition for Optimality.
  6. Second Order Sufficient Condition for Optimality.
  7. Line search.
]

#proof[
  1. A _descent direction_ for $f$ at $x in RR^n$ is any vector $v in RR^n$ such that
  $
    nabla f(x)^top v = v^top nabla f(x) < 0.
  $
  2. The _steepest descent direction_ of $f$ at $x$ is given by
  $
    op("argmin ", limits: #true)_(v in RR^n) ip(nabla f(x), v)
  $
  3.
  $
                  f(x_k + alpha d_k) & <= f(x_k) + alpha sigma_1 nabla f(x_k)^top d_k \
    nabla f(x_k + alpha d_k)^top d_k & >= sigma_2 nabla f(x_k) d_k
  $
  for $0 < sigma_1 < sigma_2 < 1$.
  4. If $x^*$ is a local minimizer and $f$ is continuously differentiable in an open neighborhood of $x^*$, then $nabla f(x^*) = 0$.
  5. If $x^*$ is a local minimizer of $f$ and $nabla^2 f$ exists and is continuous in an open neighborhood of $x^*$, then $nabla f(x^*) = 0$ and $nabla^2 f(x^*)$ is positive definite.
  6. Suppose that $nabla^2 f$ exists and is continuous in an open neighborhood of $x^*$, and that $nabla f(x^*) = 0$ and $nabla^2 f(x^*)$ is positive definite. Then $x^*$ is a strict local minimizer of $f$.
  7. Minimization of a function in a descent direction. $f(x_k + alpha_k d_k)$
]

#question[
  Write the pseudocode for a line search method that satisfies the Armijo condition in Newton's direction, assuming that the Hessian is positive definite.
]

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



#question[
  Consider the algorithm
  $
    x_(k+1) = x_k - alpha_k H_k g_k
  $
  where $f:RR^3 -> RR$, $f in cal(C)^2(RR^3)$, $g_k = nabla f(x_k)$ and $H_k in RR^(3 times 3)$ is given by:
  $
    H_k = mat(delim: "[", a^2, -a, -a; -a, -a^2, 3a; -a, 3a, -a^2)
  $
  for some $a in RR$. Suppose on the $k$-th iteration of the algorithm it holds that $g_k = [1, 1, 1]^top$. Find the values of $a in RR$ for which there exists $alpha_k > 0$ that satisfies:
  $
    alpha_k = op("argmin", limits: #true)_(alpha > 0) f(x_k - alpha H_k g_k)
  $
]
#proof[
  We have
  $
    alpha H_k g_k & = alpha mat(delim: "[", a^2, -a, -a; -a, -a^2, 3a; -a, 3a, -a^2) vec(delim: "[", 1, 1, 1,) \
                  & =alpha vec(delim: "[", a^2 - 2a, -a^2 + 2a, -a^2 + 2a)\
                  & =alpha a(a-2) vec(delim: "[", 1, -1, -1)\
  $
  We also have
  $
    d/(d alpha)f(x_k - alpha H_k g_k) = nabla f(x_k - alpha H_k g_k)^top H_k g_k
  $
  At $alpha = 0$,
  $
    nabla f(x_k - alpha H_k g_k)^top H_k g_k &= g_k^top H_k g_k\
    &= -a(a-2)
  $
  If $alpha > 0$, we need $-g_k^top H_k g_k < 0$, so $0<a<2$.
  
]

#question[
  Let $Q in RR^(2 times 2)$ be a symmetric matrix with eigenvalues $0 < lambda_1 < lambda_2$ and let $u_1, u_2$ be the eigenvectors associated with $lambda_1, lambda_2$, respectively.
  Apply the steepest descent method with exact line search to minimize the function
  $
    f(x) = 1/2 x^top Q x
  $
  starting from the point
  $
    x_0 = 1/lambda_1 u_1 + 1/lambda_2 u_2.
  $
  1. Find the expression for $x_k$.
  2. Show that the algorithm converges to the optimal solution as $k -> oo$
  3. Show that the rate $f(x_(k+1))/f(x_k)$ is strictly less than $(lambda_2 - lambda_1)/(lambda_2 + lambda_1)$
]
