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
  Let $f_1(x_1, x_2) = x_1^2 - x_2^2$, $f_2(x_1, x_2) = 2 x_1 x_2$. Represent
  the level sets associated with $f_1(x_1,x_2) = 12$ and $f_2 (x_1, x_2) = 16$
  on the same figure using Python. Indicate on the figure, the points $bold(x)
  = [x_1, x_2]^top$ for which $f(bold(x)) = [f_1(x_1, x_2), f_2(x_1,x_2)]^top = [12, 16]^top$.
]
We may analytically solve the system
$
  cases(x_1^2 - x_2^2 &= 12, 2x_1 x_2 &= 16)
$
to find that the intersection points are $(4,2)$ and $(-4,-2)$.
#figure(
  image("p1.png"),
  caption: [In #text("blue", blue), $x_1^2 - x_2^2 = 12$ and in #text("red", red), $2 x_1 x_2 = 16$. The intersection points are given by {(4,2), (-4, -2)}. ],
)


#question[
  Consider the function $f(bold(x)) = (bold(a)^top bold(x))(bold(b)^top
    bold(x))$, where $bold(a)$, $bold(b)$ and $bold(x)$ are $n$-dimensional
  vectors.

  Compute the gradient $nabla f(bold(x))$ and the Hessian $nabla^2 f(bold(x))$.
]

#proof[
  We define
  $
    h(bold(x)) = bold(a)^top bold(x), quad
    g(bold(x)) = bold(b)^top bold(x).
  $
  Their gradients are given by
  $
    nabla h(bold(x)) = bold(a), quad nabla g(bold(x)) = bold(b).
  $
  Now, by @thm:product-rule (Product Rule), we obtain
  $
                                    nabla f(bold(x)) & = nabla ( h(bold(x)) g(bold(x))) \
                                                     & = g(bold(x)) nabla h(bold(x)) + h(bold(x)) nabla g(bold(x)) \
                                                     & = (bold(b)^top bold(x)) bold(a) + (bold(a)^top bold(x)) bold(b) \
    (bold(b)^top bold(x), bold(a)^top bold(x) in RR) & = bold(a)bold(b)^top bold(x) + bold(b)bold(a)^top bold(x) \
                                                     & = (bold(a)bold(b)^top + bold(b)bold(a)^top) bold(x).
  $
  Therefore, for the Hessian, we have
  $
    nabla^2 f(bold(x)) & = nabla((bold(a)bold(b)^top + bold(b)bold(a)^top) bold(x)) \
                       & = (bold(a)bold(b)^top + bold(b)bold(a)^top)^top \
                       & = bold(a)bold(b)^top + bold(b)bold(a)^top.
  $
]


#question[
  Compute the gradient of
  $
    f(theta) eq.def 1/2 sum_(i=1)^(n) [g(bold(x)_i) - g(bold(A x)_i + bold(b))]^2
  $
  with respect to $theta$, where $theta = [a_(11) , a_(12), a_(21), a_(22),
    b_(1), b_(2)]^top$, $bold(x)_i in RR^2$, $bold(A)in RR^(2times 2)$, $bold(b)
  in RR^2$ are defined as follows:

  $
    bold(A) & = mat(delim: "[", a_(11), a_(12); a_(21), a_(22)) \
    bold(b) & = [b_1, b_2]^top.
  $
  and $g:RR^2 -> RR$ is $cal(C)^1$.
]

#proof[
  Define
  $
    epsilon_i(theta) = g(bold(x)_i) - g(bold(A x)_i + bold(b)).
  $
  Thus, we write
  $
    f(theta) = 1/2 sum_i epsilon_i (theta)^2.
  $
  We have that
  $
    nabla_theta epsilon_i^2 (theta) & = 2 epsilon_i (theta) nabla_theta epsilon_i (theta).
  $
  Therefore,
  $
    nabla f(theta) & = sum_i epsilon_i (theta) nabla_theta epsilon_i (theta).
  $
  Now note that
  $
    nabla_theta epsilon_i (theta) &= nabla_theta [g(bold(x)_i) - g(bold(A x)_i + bold(b))]\
    &= -nabla g(bold(A x)_i + bold(b)) nabla_theta (bold(A x)_i + bold(b))\
    &= -nabla g(bold(A x)_i + bold(b)) [e_1 bold(x)_i, e_2 bold(x)_i, e_3 bold(x)_i, e_4 bold(x)_i, e_5, e_6]^top\
    &= vec(delim: "[", -nabla g(bold(A x)_i + bold(b))bold(x)_i^top, -nabla g(bold(A x)_i + bold(b))).
  $
  Therefore, we conclude

  $
    nabla f(theta) = sum_i (g(bold(x)_i) - g(bold(A x)_i + bold(b))) vec(delim: "[", -nabla g(bold(A x)_i + bold(b))bold(x)_i^top, -nabla g(bold(A x)_i + bold(b))).
  $
]

#question[
  Let $f(r,theta)$ be a function from $RR^2$ to $RR$ with $r = sqrt(x^2+y^2)$ and
  $theta = arctan y/x$. Compute $(partial f) / (partial x)$ and $(partial f) /
  (partial y)$.
]

#proof[
  We first compute the partial derivatives of $r(x,y)$ and $theta(x, y)$. These are given by
  $
    (partial r) / (partial x) = x/sqrt(x^2 + y^2) = x/r, quad (partial r) / (partial y) = y/sqrt(x^2 + y^2) = y/r.\
    (partial theta) / (partial x) = -(y/x^2)/(1 + y^2/x^2) = -y/(x^2+y^2) = -y/r^2, quad (partial theta) / (partial y) = (1/x)/sqrt(1 + y^2/x^2) = x/sqrt(x^2+y^2) = x/r^2.
  $
  Therefore, by the chain rule,
  $
    (partial f) / (partial x) = (partial f) / (partial r) (partial r) / (partial x) + (partial f) / (partial theta) (partial theta) / (partial x)
    = (partial f) / (partial r) x/r - (partial f) / (partial theta) y/r^2,\
    (partial f) / (partial y) = (partial f) / (partial r) (partial r) / (partial y) + (partial f) / (partial theta) (partial theta) / (partial y)
    = (partial f) / (partial r) y/r+ (partial f) / (partial theta) x/r^2.
  $
]

#question[
  The directional derivatives $(partial f)/(partial v)(x_0, y_0, z_0)$ of a
  differentiable function $f$ are $3/sqrt(2)$, $1/sqrt(2)$, $-1/sqrt(2)$ in the
  directions of the vectors $[0, 1/sqrt(2), 1/sqrt(2)]^top$, $[1/sqrt(2), 0,
    1/sqrt(2)]^top$ and $[1/sqrt(2), 1/sqrt(2), 0]^top$. Compute $nabla f(x_0,
    y_0, z_0)$.
]

#proof[
  Note that the three directions form a basis for $RR^3$, and thus the gradient can be completely determined.
  Given that $f$ is $cal(C)^1$ and each $v$ is a unit vector,
  $
    nabla f(x_0, y_0, z_0) dot v = D_v f(x_0, y_0, z_0),
  $
  Let $nabla f(x_0, y_0, z_0) = (x,y,z)$, then we have a system of three equations:
  $
    0x + y/sqrt(2) + z/sqrt(2) & = 3/sqrt(2) \
    x/sqrt(2) + 0y + z/sqrt(2) & = 1/sqrt(2) \
    x/sqrt(2) + y/sqrt(2) + 0z & = -1/sqrt(2) \
  $
  which simplifies to
  $
    y + z & = 3 \
    x + z & = 1 \
     x+ y & = -1 \
  $
  and thus
  $
    nabla f(x_0, y_0, z_0) = (-3/2, 1/2, 5/2).
  $
]


#question[
  Show that the level curves of the function $f(x,y) = x^2 + y^2$ are
  orthogonal to the level curves of $g(x,y) = y/x$ for all $(x,y)$.
]

#proof[
  The level curves of $f$ at $l in RR$ are given by
  $
    S_f = {(x,y) : x^2+y^2 = l}.
  $
  Similarly,
  $
    S_g = {(x,y) : y/x = l}.
  $
  That is, we must verify if the curves
  $
    y^2 = l - x^2 quad y = l x
  $
  are orthogonal. Their derivatives (slopes) are
  $
    m_1 = (d y)/( d x) = -x/y, quad m_2 = (d y) / (d x) = l
  $
  and thus their product is
  $
    m_1 m_2 = -(l x) /y.
  $
  Substituting $y = l x$, we have
  $
    m_1 m_2 = - 1.
  $
  That is, the curves are orthogonal.
]



#question[
  Let $f,g,h$ be differentiable functions, with $f:RR^n -> RR^3$, $g:RR^n -> RR^3$ and $h:RR^n->RR$
  $
    h(bold(x)) = f(bold(x))^top g(bold(x))
  $
  show that
  $
    D h(bold(x)) = f(bold(x))^top D g(bold(x)) + g(bold(x))^top D f(bold(x)).
  $
]

#proof[
  We have that
  $
    lim_(norm(k)->0) abs(h(a+k) - h(a) - D h(a) k) / norm(k) &= lim_(norm(k)->0) abs(f(a+k)g(a+k) - f(a)g(a) - D f(a)g(a) k) / norm(k)\
  $<lim:diff>
  Now, note that
  $
    f(a+k)g(a+k) - f(a)g(a) & = f(a+k)g(a+k) + f(a)g(a+k) - f(a)g(a+k) - f(a)g(a) \
                            & = f(a)[g(a+k) - g(a)] + g(a+k)[f(a+k) - f(a)].
  $
  Thus, @lim:diff is equivalent to
  $
    lim_(norm(k)->0) abs(f(a)[g(a+k) - g(a)] + g(a+k)[f(a+k) - f(a)] - D f(a)g(a) k) / norm(k)
  $
  Since both $f,g$ are differentiable, we have
  $
    lim_(norm(k)->0) abs(f(a) D g(a) + g(a) D f(a) - D f(a)g(a) k) / norm(k)
  $

]

#question[
  Consider the *induced matrix norm*
  $
    norm(bold(A))_p = max_(bold(x) eq.not 0) norm(bold(A x))_p / norm(bold(x))_p
  $
  where $norm(dot)_p$ is the $ell_p$ norm, i.e.
  $
    norm(bold(x))_p = (abs(x_1)^p + abs(x_2)^p + dots.c + abs(x_n)^p)^(1/p)
  $
  Show that
  $
    norm(bold(A B))_p <= norm(bold(A))_p norm(bold(B))_p.
  $
]

#proof[
  We first note that by definition, $norm(bold(A x))_p / norm(bold(x))_p <= norm(bold(A))_p$ for all $bold(x) eq.not 0$. Therefore,
  $
    norm(bold(A x))_p <= norm(bold(A))_p norm(bold(x))_p, quad bold(x) eq.not 0.
  $
  Thus,
  $
    norm(bold(A B x))_p = norm(bold(A (B x)))_p <= norm(bold(A))_p norm(bold(B x))_p <= norm(bold(A))_p norm(bold(B))_p norm(bold(x))_p.
  $
  Equivalently,
  $
    norm(bold(A B x))_p/norm(bold(x))_p <= norm(bold(A))_p norm(bold(B))_p, quad bold(x) eq.not 0.
  $
  Since $bold(x) in RR^n backslash {0}$ was arbitrary, we may conclude that
  $
    norm(bold(A B))_p = max_(bold(x) eq.not 0) norm(bold(A B x))_p/norm(bold(x))_p <= norm(bold(A))_p norm(bold(B))_p.
  $
]

== Appendix

#theorem(title: "Product Rule")[
  Let $bold(f):RR^n -> RR^m$, $bold(g): RR^n -> RR^m$ be two differentiable functions. Define the function $h: RR^n -> RR$ by
  $h(bold(x)) = bold(f)(bold(x))^top bold(g)(bold(x))$ then
  $
    D_x h(bold(x)) = bold(f)(bold(x))^top D bold(g)(bold(x)) + bold(g)(bold(x))^top D bold(f)(bold(x))in RR^(1 times n),
  $
  where $D bold(g)(bold(x))$ and $D bold(f)(bold(x))$ are matrices (the Jacobian matrix). Equivalently,
  $
    nabla h(bold(x)) = nabla bold(f)(bold(x)) bold(g)(bold(x)) + nabla bold(g)(bold(x)) bold(f)(bold(x)) in RR^(n).
  $
]<thm:product-rule>
