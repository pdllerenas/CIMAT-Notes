#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question


#show: pset.with(
  class: "Optimization I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 2",
  date: datetime(year: 2026, month: 2, day: 17),
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
  Calculate and classify the stationary points of the following function
  $
    f(x_1, x_2) = (x_1^2 + x_2^2 -1)^2 + (x_2^2 - 1)^2
  $
  Plot the function.
]

The gradient is given by

$
  nabla f(x_1, x_2) = vec(delim: "[", 4x_1(x_1^2+x_2^2-1), 4x_2(x_1^2+2x_2^2-2)).
$
Setting the gradient to zero, we find the following:
$
  0 equiv nabla f(x_1, x_2) = vec(delim: "[", 4x_1(x_1^2+x_2^2-1), 4x_2(x_1^2+2x_2^2-2)) <==> cases(4x_1(x_1^2+x_2^2-1) &= 0, 4x_2(x_1^2+2x_2^2-2) &= 0)
$
The trivial solution $(0,0)$ satisfies the equations.

The first equation gives, for $x_1 != 0$,
$
  x_1^2 + x_2^2 = 1,
$
If $x_2 = 0$, then the first equations becomes
$
  x_1^2 = 1,
$
so $(plus.minus 1, 0)$ are solutions. For $x_2 != 0$, the second equation becomes
$
  x_1^2 + 2x_2^2 = 2.
$
These equations represent a unit circle centered at $(0,0)$ and an ellipse with semi-major axis $sqrt(2)$ and semi-minor axis $1$, centered at $(0,0)$. Since the semi-minor axis is 1, (same as the circle, since the unit circle has semi-minor and semi-major axis equal to 1), they only intersect at $(0,1)$ and $(0,-1)$.

Now, if $x_1 = 0$, we have from the second equation $8x_2^3 = 8$, so $x_2 = 1$. If $x_2 = 0$, from the first equation we have $x_1 = 1$. Therefore, the stationary points are ${(0,1), (0,-1), (1, 0), (-1 ,0), (0,0)}$.

To classify the points, we use the Hessian, which is given by
$
  nabla^2 f(x_1, x_2) = mat(delim: "[", 12x_1^2 + 4x_2^2 - 4, 8x_1x_2; 8x_1x_2, 4x^2_1+24x_2^2-8).
$
We evaluate the determinants at each point:
$
    abs(nabla^2 f(0,0)) & = mat(delim: "|", - 4, 0; 0, -8) > 0 \
   abs(nabla^2 f(1, 0)) & = mat(delim: "|", 8, 0; 0, -4) < 0. \
  abs(nabla^2 f(-1, 0)) & = mat(delim: "|", 8, 0; 0, -4) < 0. \
   abs(nabla^2 f(0, 1)) & = mat(delim: "|", 0, 0; 0, 16) = 0. \
  abs(nabla^2 f(0, -1)) & = mat(delim: "|", 0, 0; 0, 16) = 0. \
$
Therefore, we have the following table:
#figure(
  table(
    stroke: none,
    columns: 2,
    table.hline(),
    table.header([*Point*], [*Classification*]),
    table.hline(),
    $(0,0)$, table.vline(), [Local maximum],
    $(1,0)$, table.vline(), [Saddle point],
    $(-1,0)$, table.vline(), [Saddle point],
    $(0,1)$, table.vline(), [Inconclusive],
    $(0,-1)$, table.vline(), [Inconclusive],
    table.hline(),
  ),
  caption: [Stationary point classification for $f(x_1,x_2) = (x_1^2 + x_2^2 -1)^2 + (x_2^2 - 1)^2$],
)


#question[
  Let $A succ 0$. Show that $A_(i j) < (A_(i i) + A_(j j)) / 2$.
]

#proof[
  Let $x in RR^n$, then $x^top A x > 0$. In particular, if $x = e^i - e^j$, where $e^i$ is the $i$-th vector from the standard basis of $RR^n$, we have
  $
    0 < (e^i - e^j)^top A (e^i - e^j) & = (e^i)^top A e^i - 2(e^j)^top A e^i +(e^j)^top A e^j \
                                      & = A_(i i) - 2 A_(i j) + A_(j j).
  $
  Rearranging, we obtain the desired inequality:
  $
    A_(i j) < (A_(i i) + A_(j j))/2
  $
]

#question[
  Let $f:RR^n -> RR$ be a convex function. Show that for all $x, y$,
  $
    f(y) >= f(x) + alpha(f(x) - f(z)),
  $
  where $alpha > 0$ and $z = x+1/alpha (x-y)$.
]
#proof[
  By convexity,
  $
    f(z) = f(x + 1/alpha (x-y)) <= (1+1/alpha)f(x) - (1/alpha)f(y).
  $
  Multiplying both sides by $alpha$, we have
  $
    alpha f(z) <= (alpha+1)f(x) - f(y).
  $
  Rearranging, we obtain
  $
    f(y) <= f(x) + alpha (f(x) - f(z)).
  $
]

#question[
  Suppose the gradient descent algorithm is used with a fixed step size to minimize the function
  $
    f(x) = 1/2 x^top mat(delim: "[", 3/2, 2; 0, 3/2) x + x^top vec(delim: "[", 3, -1) - 22.
  $
  Determine the range of values the step size may attain to converge to the minimizer.
]





#question[
  Show that the set
  $
    S = {x : abs(x^top y) <= 1; x, y in RR^n}
  $
  is convex.
]
#proof[
  Let $x_1, x_2 in S$. Let $y in RR^n$ and $alpha in (0,1)$. By the triangle inequality,
  $
    abs((alpha x_1 + (1-alpha)x_2)^top y) & = abs(alpha x_1^top y + (1-alpha)x_2^top y) \
                                          & <= abs(alpha x_1^top y) + abs((1-alpha)x_2^top y) \
                                          & = alpha abs(x_1^top y) + (1-alpha)abs(x_2^top y) \
                                          & <= alpha + (1 - alpha) \
                                          & = 1.
  $
  That is, $alpha x_1 + (1-alpha)x_2 in S$, so $S$ is convex.
]

#question[
  Let
  $
    f(x) = 1/2 x^top Q x - b^top x
  $
  where $Q$ is symmetric and positive definite. If $d_k$ is a descent direction.
  - Calculate the exact step size, $alpha_k^(M G) = arg min_(alpha>0) norm(nabla f(x_k + alpha d_k))$
]

#proof[
  The gradient of $f(x)$ is given by
  $
    nabla f(x) = Q x - b.
  $
  Evaluated at the point, we have
  $
    nabla f(x_k + alpha d_k) = Q (x_k + alpha d_k) - b.
  $
  Since minimizing the square of the norm is equivalent to minimizing the norm, we minimize the square.
  Thus, we simplify the squared norm, using the symmetry of $Q$:
  $
    norm(Q(x_k + alpha d_k) - b)^2 &= (Q(x_k + alpha d_k) - b)^top (Q(x_k + alpha d_k) - b)\
    &=x_k^top Q^2 x_k + alpha^2 d_k^top Q^2 d_k + 2alpha x_k^top Q^2 d_k - 2 b^top Q(x_k+alpha d_k)
  $
  Differentiating w.r.t. $alpha$, we have
  $
    d/(d alpha) norm(Q(x_k + alpha d_k) - b)^2 = 2alpha d_k^top Q^2 d_k + 2x_k^top Q^2 d_k - 2b^top Q d_k.
  $
  Equating to zero, we obtain
  $
    alpha &= (b^top Q d_k - x_k^top Q^2 d_k)/(d_k^top Q^2 d_k) \
    &= ((b^top- x_k^top Q)Q d_k)/(d_k^top Q^2 d_k) \
    &= -(g_k^top Q d_k)/(d_k^top Q^2 d_k).
  $

]
