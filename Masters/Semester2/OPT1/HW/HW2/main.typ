#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Optimization I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 1",
  date: datetime(year: 2026, month: 2, day: 6),
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
  Show that $x - sin(x) = o(x^2)$, as $x -> 0$.
]

The Taylor series expansion for $sin(x)$ around $x = 0$ is given by
$
  sin(x) = sum_(n = 0)^infinity (-1)^n/((2n+1)!) x^(2n+1).
$
We may also write it as
$
  sin(x) = x - x^3/3! + O(x^5) " as " x->0
$
Therefore,
$
  (x-sin(x))/x^2 & = (x-(x - x^3/3! + O(x^5)))/x^2 \
                 & = (x^3/3! + O(x^5))/x^2 \
                 & = x/3! + O(x^3).
$
We then conclude that
$
  lim_(x->0) (x-sin(x))/x^2 & = lim_(x->0) x/3! + O(x^3) = 0.
$
This proves that $x-sin(x) = o(x^2)$.

#question[
  Suppose that $f(bold(x)) = o(g(bold(x)))$. Show that $f(bold(x)) = O(g(bold(x)))$.
]
By definition of little $o$,
$
  lim_(x->a) f(bold(x))/g(bold(x)) & = 0.
$
In particular,
$
  lim_(x->a) f(bold(x))/g(bold(x)) < infinity,
$
which by definition, means $f in O(g(bold(x)))$.

#question[
  Show that if functions $f: RR^n -> RR$ and $g:RR^n-> RR$ satisfy $f(bold(x)) = -g(bold(x)) + o(g(bold(x)))$ and $g(bold(x)) > 0$ for all $bold(x) eq.not 0$, then for all $bold(x) eq.not bold(0)$ sufficiently small, we have $f(bold(x)) < 0$.
]

By definition of little $o$, for any $epsilon > 0$, $exists delta >0$ such that if $0 < norm(x) < delta$, then $abs(f(bold(x)) + g(bold(x))) < epsilon abs(g(bold(x)))$. Since $g(bold(x)) > 0$ for all $bold(x) eq.not 0$,
$
  f(bold(x)) < f(bold(x)) + g(bold(x)) <= abs(f(bold(x)) + g(bold(x))) < epsilon abs(g(bold(x))).
$

#question[
  Compute the stationary points of $f(x,y) = (3x^4 - 4x^3 - 12x^2 + 18)/(12(1+4y^2))$ and determine their corresponding type.
]
The stationary points of a function are given by the system of equations resulting from equating the gradient to 0. Thus,
$
  nabla f(x,y) = vec((12x^3 - 12x^2 - 24x)/(12(1+4y^2)), ((8y)(3x^4 - 4x^3 - 12x^2 + 18))/(12(1+4y^2))^2) equiv vec(0, 0)
$
From the first component, we have
$
  12x^3 - 12x^2 - 24x = 0 & <==> x^3 - x^2 - 2x = 0 \
                          & <==> x(x+1)(x-2) = 0
$
Thus, the solutions to the first equation are $(x,y) in {0, -1, 2} times RR$. For the second equation, we have
$
  (8y)(3x^4 - 4x^3 - 12x^2 + 18) = 0 <==> (x,y) in {1.2011, 2.5417} times {0}.
$

#question[
  Show that the function $f(bold(x)) = 8x_1 + 12 x_2 + x_1^2 -2x_2^2$ has only one stationary points, and that is neither the maximum or minimum, but a saddle point. Plot the contour lines of $f$.
]

The gradient of $f$ is given by
$
  nabla f(bold(x)) = vec(8 + 2x_1, 12 - 4x_2).
$
Equating to zero, we get $x_1 = -4$ and $x_2 = 3$. Since these were linear polynomials, the solutions are obviously unique. However, we note that
$
  abs(nabla^2 f(-4,3)) = mat(delim: "|", 2, 0; 0, -4) = -8 < 0,
$
so by the second derivative test, this is a saddle point.

#question[
  Compute the gradient $nabla f(bold(x))$ and Hessian $nabla^2 f(bold(x))$ of the Rosenbrock function
  $
    f(bold(x)) = sum_(i=1)^(N-1) [100(x_(i+1) - x_i^2)^2 + (1-x_i)^2]
  $
  where $bold(x) = [x_1, ..., x_N]^top in RR^N$.
  If $n = 2$ show that $bold(x)^* = [1 , 1]^top$ is the only local minimizer of this function, and that the Hessian matrix at that point is positive definite. Plot the contour lines of $f$.
]

#question[
  Show, without using the optimality conditions, that $f(bold(x)) > f(bold(x)^*)$ for all $bold(x) eq.not bold(x)^*$ if
  $
    f(bold(x)) = 1/2 bold(x)^top bold(Q) bold(x) - bold(b)^top bold(x)
  $
  $bold(Q) = bold(Q)^top succ 0$ and $bold(Q) bold(x)^* = bold(b)$.
]
Since $bold(Q)$ is symmetric, we have $bold(Q)bold(x)^* = bold(b) <==> (bold(x)^*)^top bold(Q) = bold(b)^top$. Therefore,
$
  f(bold(x)^*) & = 1/2 (bold(x)^*)^top bold(Q) bold(x)^* - bold(b)^top bold(x)^* \
               & = 1/2 bold(b)^top bold(x)^* - bold(b)^top bold(x)^*\
               & = -1/2 bold(b)^top bold(x)^*.
$
Now, using positive definiteness, we know that $1/2 bold(x)^top bold(Q)bold(x) > 0$, so we have
$
    f(bold(x)) &= 1/2 bold(x)^top bold(Q) bold(x) - bold(b)^top bold(x) \ 
    &> - bold(b)^top bold(x)\
    &= f(bold(x)^*).
$
This proves the statement.

#question[
  Let $bold(x) in RR^n$ and $t > 0$. Consider a function $g:(0,infinity) -> (0, infinity)$ such that
  $
    0 < g(t) < t " for all " t>0,
  $
  and define
  $
    bold(y)(t) = bold(x) + g(t)bold(d).
  $
  Also define
  $
    a(t) colon.eq g(t)/t.
  $
  1. Prove that
  $
    bold(y)(t) in B_r(bold(x)) quad <=> quad t a(t)norm(d) < r.
  $
  2. Using only the fact that $0 < a(t) < 1$ for all $t>0$, derive a sufficient condition on $t$ (independent of the explicit form of $g$) that guarantees
  $
    bold(y)(t) in B_r(bold(x)).
  $
  3. Give one explicit choice of of $t$ that always works when $bold(d) eq.not 0$.
]

#question[
  Let $f:RR^n -> RR$ be of class $C^1$, and let $bold(x)^* in RR^n$ be a local minimizer of $f$. Without using the first-order necessary condition for optimality of the theorem, prove the following:
  1. For every $bold(v) in RR^n$,
  $
    nabla f(bold(x)^*)^top bold(v) >= 0.
  $
  2. Deduce that
  $
    nabla f(bold(x)^*) = 0.
  $
]
