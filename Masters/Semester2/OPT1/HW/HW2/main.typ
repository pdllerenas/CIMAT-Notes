#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Optimization I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 2",
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

#proof[
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
]

#question[
  Suppose that $f(bold(x)) = o(g(bold(x)))$. Show that $f(bold(x)) = O(g(bold(x)))$.
]
#proof[
  By definition of little $o$,
  $
    lim_(x->a) f(bold(x))/g(bold(x)) & = 0.
  $
  In particular,
  $
    lim_(x->a) f(bold(x))/g(bold(x)) < infinity,
  $
  which by definition, means $f in O(g(bold(x)))$.
]

#question[
  Show that if functions $f: RR^n -> RR$ and $g:RR^n-> RR$ satisfy $f(bold(x)) = -g(bold(x)) + o(g(bold(x)))$ and $g(bold(x)) > 0$ for all $bold(x) eq.not 0$, then for all $bold(x) eq.not bold(0)$ sufficiently small, we have $f(bold(x)) < 0$.
]
#proof[
  By definition of little $o$, for any $epsilon > 0$, $exists delta >0$ such that if $0 < norm(x) < delta$, then $abs(f(bold(x)) + g(bold(x))) < epsilon abs(g(bold(x)))$. Since $g(bold(x)) > 0$ for all $bold(x) eq.not 0$,
  $
    abs(f(bold(x)) + g(bold(x))) < epsilon abs(g(bold(x))) = epsilon g(x).
  $
  Equivalently,
  $
    -epsilon g(x)< f(bold(x)) + g(bold(x)) < epsilon g(x) <==> -(1+epsilon) g(x)< f(bold(x)) < (epsilon - 1) g(x).
  $
  Choosing $0 <epsilon < 1$ gives
  $
    f(x) < (epsilon - 1)g(x) < 0
  $
  by positivity of $g$.
]

#question[
  Compute the stationary points of $f(x,y) = (3x^4 - 4x^3 - 12x^2 + 18)/(12(1+4y^2))$ and determine their corresponding type.
]
#proof[
  The stationary points of a function are given by the system of equations resulting from equating the gradient to 0. Thus,
  $
    nabla f(x,y) = vec((12x^3 - 12x^2 - 24x)/(12(1+4y^2)), -((8y)(3x^4 - 4x^3 - 12x^2 + 18))/(12(1+4y^2))^2) = vec((x^3 - x^2 - 2x)/(1+4y^2), -((8y)(3x^4 - 4x^3 - 12x^2 + 18))/(12(1+4y^2))^2) equiv vec(0, 0)
  $
  From the first component, we have
  $
    x^3 - x^2 - 2x = 0 & <==> x(x+1)(x-2) = 0
  $
  Thus, the solutions to the first equation are $(x,y) in {0, -1, 2} times RR$. For the second equation, we have
  $
    (8y)(3x^4 - 4x^3 - 12x^2 + 18) = 0 <==> (x,y) in {1.2011, 2.5417} times {0}.
  $
  Since $x = 1.2011$ and $2.5417$ do not satisfy the first equation, they are not stationary points. Therefore, that only stationary points are $(0,0)$, $(-1,0)$ and $(2,0)$. To determine their type, we compute the Hessian matrix:
  $
    abs(nabla^2 f(x,y)) = mat(delim: "|", (3x^2 - 2x - 2)/(1+4y^2), (-64y)(3x^4 - 4x^3 - 12x^2 + 18)/(12(1+4y^2))^2; (-64y)(3x^4 - 4x^3 - 12x^2 + 18)/(12(1+4y^2))^2, (8(3x^4 - 4x^3 - 12x^2 + 18)(12y^2 - 1))/(12(1+4y^2))^3).
  $
  Evaluating at the stationary points, we have
  $
    abs(nabla^2 f(0,0)) = mat(delim: "|", -2, 0; 0,  -0.08 dash(3)) & > 0, \
                                abs(nabla^2 f(-1,0)) = mat(delim: "|", 3, 0; 0, -0.06) & < 0, \
                                abs(nabla^2 f(2,0)) = mat(delim: "|", 2, 0; 0, 0.06) & > 0.
  $
  Therefore, by the second derivative test, $(-1,0)$ is a saddle point, while $(0,0)$ is a local maximum due to $D_(x x) f(0,0) < 0$ and $(2,0)$ is a local minimum due to $D_(x x) f(2,0) > 0$.
]

#question[
  Show that the function $f(bold(x)) = 8x_1 + 12 x_2 + x_1^2 -2x_2^2$ has only one stationary points, and that is neither the maximum or minimum, but a saddle point. Plot the contour lines of $f$.
]

#proof[
  The gradient of $f$ is given by
  $
    nabla f(bold(x)) = vec(8 + 2x_1, 12 - 4x_2).
  $
  Equating to zero, we get $x_1 = -4$ and $x_2 = 3$. Since these were linear polynomials, the solutions are obviously unique. However, we note that
  $
    abs(nabla^2 f(-4,3)) = mat(delim: "|", 2, 0; 0, -4) = -8 < 0,
  $
  so by the second derivative test, this is a saddle point.
  #figure(
    image("p5.png"),
    caption: [Contour plot of the function $f(bold(x)) = 8x_1 + 12 x_2 + x_1^2 -2x_2^2$.],
  )
]

#question[
  Compute the gradient $nabla f(bold(x))$ and Hessian $nabla^2 f(bold(x))$ of the Rosenbrock function
  $
    f(bold(x)) = sum_(i=1)^(N-1) [100(x_(i+1) - x_i^2)^2 + (1-x_i)^2]
  $
  where $bold(x) = [x_1, ..., x_N]^top in RR^N$.
  If $n = 2$ show that $bold(x)^* = [1 , 1]^top$ is the only local minimizer of this function, and that the Hessian matrix at that point is positive definite. Plot the contour lines of $f$.
]
#proof[
  A component-wise differentiation gives
  $
    (partial f)/(partial x_1) & = -400(x_2 - x_1^2)x_1 - 2(1-x_1), \
    (partial f)/(partial x_j) & = 200(x_(j) - x_(j-1)^2) - 400(x_(j+1) - x_j^2)x_j - 2(1- x_j), quad "for " 1 < j < N \
    (partial f)/(partial x_N) & = 200(x_(N) - x_(N-1)^2).
  $
  For the gradient, we have
  $
          (partial^2 f)/(partial x_1^2) & = -400(x_2 - x_1^2) +800x_1^2 + 2, \
    (partial^2 f)/(partial x_j x_(j-1)) & = -400 x_(j-1) \
          (partial^2 f)/(partial x_j^2) & = 200 - 400(x_(j+1) - x_j^2) + 800x_j^2 + 2, quad "for " 1 < j < N \
    (partial^2 f)/(partial x_j x_(j+1)) & = -400 x_j \
          (partial^2 f)/(partial x_N^2) & = 200,
  $
  In the case $N = 2$, we prove that the local minimizer is unique. The Rosenbrock function simplifies to
  $
    f(x,y) = 100(y - x^2)^2 + (1-x)^2.
  $
  Equating the gradient to zero result in
  $
    cases(-400(y - x^2)x - 2(1-x) &= 0, 200(y-x^2) &= 0)
  $
  The second equation gives $y = x^2$. Plugging this into the first equation gives
  $x = 1$, and thus $y = 1$. This is the only stationary point, so it is a unique local minimizer.

  Now, we verifiy $nabla^2 f(1,1) succ 0$. We have
  $
    abs(nabla^2 f(1,1)) & = mat(
                            delim: "|", -400(1 - 1^2) + 800 dot 1^2 + 2, -400
                            dot 1; -400 dot 1, 200
                          ) \
                        & = mat(delim: "|", 802, -400; -400, 200) \
                        & =802 dot 200 + 400 dot 400 \
                        & =401 dot 400 + 400 dot 400 \
                        & = 400 > 0.
  $
  By Sylverster's criterion, since both leading principal minors are positive
  ($802 > 0$, $400 > 0$), the Hessian is positive definite.

  #figure(
    image(
      "p6.png",
    ),
    caption: [Contour plot of the Rosenbrock function for $N = 2$.],
  )
]


#question[
  Show, without using the optimality conditions, that $f(bold(x)) > f(bold(x)^*)$ for all $bold(x) eq.not bold(x)^*$ if
  $
    f(bold(x)) = 1/2 bold(x)^top bold(Q) bold(x) - bold(b)^top bold(x)
  $
  $bold(Q) = bold(Q)^top succ 0$ and $bold(Q) bold(x)^* = bold(b)$.
]
#proof[
  Note that, by symmetry of $bold(Q)$, we have $bold(b)^top = bold(x^*)^top bold(Q)$ and
  $
    bold(x^*)^top bold(Q) bold(x) = bold(x)^top bold(Q) bold(x)^*
  $
  Therefore, we may conclude the following equations:
  $
    f(bold(x)) - f(bold(x)^*) &= 1/2 bold(x)^top bold(Q) bold(x) - bold(b)^top bold(x) - 1/2 bold(x^*)^top bold(Q) bold(x)^* + bold(b)^top bold(x)^* \
    &= 1/2 bold(x)^top bold(Q) bold(x) - bold(x^*)^top bold(Q) bold(x) - 1/2 bold(x^*)^top bold(Q) bold(x)^* + bold(x^*)^top bold(Q) bold(x)^* \
    &= 1/2 bold(x)^top bold(Q) bold(x) - bold(x^*)^top bold(Q) bold(x) + 1/2 bold(x^*)^top bold(Q) bold(x)^*\
    &= 1/2 bold(x)^top bold(Q) bold(x) - (1/2 bold(x^*)^top bold(Q) bold(x) + 1/2 bold(x)^top bold(Q) bold(x)^*) + 1/2 bold(x^*)^top bold(Q) bold(x)^*\
    &= 1/2 (bold(x)-bold(x)^*)^top bold(Q) (bold(x)- bold(x)^*)\
    &>0.
  $
  It follows that $f(bold(x)) > f(bold(x)^*)$.
]

#question[
  Let $bold(A) in RR^(n times n)$ be a real symmetric matrix. Show that
  $
    max {bold(x)^top bold(A) bold(x): norm(bold(x)) = 1} = lambda_(max)(bold(A)).
  $
]
#proof[
  Since $bold(A)$ is real and symmetric, its adjoint is itself, and thus it is a
  normal matrix. Since it is self-adjoint, the Spectral Theorem @axler2015ladrr4e
  says that we may use a singular value decomposition, and rewrite $bold(A)$ as
  $
    bold(A) = bold(U) bold(Lambda) bold(U)^(top)
  $
  where $bold(Lambda) = {lambda_i}_(i, i)$ is the diagonal matrix of eigenvalues of $bold(A)$, and
  $bold(U)$ is an orthogonal matrix. For any $bold(x) in RR^n$ with $norm(bold(x)) = 1$, we have
  $
    bold(x)^top bold(A) bold(x) = bold(x)^top bold(U Lambda U^(top)) bold(x),
  $
  and let $bold(y) = bold(U)^(top) bold(x)$. By orthogonality of $bold(U)$, $norm(bold(y))^2 = bold(x)^top bold(U)bold(U)^top bold(x) = bold(x)^top bold(x) = norm(bold(x))^2 = 1$. Therefore,
  $
    bold(x)^top bold(A) bold(x) = bold(y^top Lambda y) = sum_i lambda_i y_i^2 <= lambda_max (bold(A)) sum_i y_i^2 = lambda_max (bold(A)) norm(bold(y))^2 = lambda_max (bold(A)).
  $
  To prove equality with the maximum, notice that the function $f(bold(x)) =
  bold(x)^top bold(A) bold(x)$ is continuous and the set $S = {bold(x) in RR^n :
  norm(bold(x)) = 1}$ is compact. Therefore, $f$ attains its maximum on $S$ for some $bold(x) in S$.

  Taking the correponding eigenvector $bold(v)_max$ of $lambda_max(bold(A))$ and normalizing it, we have
  $
    bold(v)_max^top bold(A) bold(v)_max = lambda_max(bold(A)) norm(bold(v)_max)^2 = lambda_max(bold(A)).
  $
  That is, the maximum is attained at the eigenvector corresponding to the largest eigenvalue, and thus
  $
    max {bold(x)^top bold(A) bold(x): norm(bold(x)) = 1} = lambda_(max)(bold(A)).
  $
]

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
    bold(y)(t) in B_r (bold(x)) quad <==> quad t a(t)norm(bold(d)) < r.
  $
  2. Using only the fact that $0 < a(t) < 1$ for all $t>0$, derive a sufficient condition on $t$ (independent of the explicit form of $g$) that guarantees
  $
    bold(y)(t) in B_r (bold(x)).
  $
  3. Give one explicit choice of $t$ that always works when $bold(d) eq.not 0$.
]
#proof[
  1. Using $g(t) > 0$, one has
  $
    t a(t) norm(bold(d)) & = g(t) norm(bold(d)) \
                         & = norm(bold(x) - bold(x) + g(t)bold(d)) \
                         & = norm(bold(y)(t) - bold(x)).
  $
  Therefore, if $bold(y)(t) in B_r (bold(x))$, it follows that $t a(t)norm(bold(d)) < r$ by the equalities above.

  Similarly, if $t a(t)norm(bold(d)) < r$, we conclude that $bold(y)(t) in B_r (bold(x))$. This proves the equivalence.

  2. Since $0 < a(t) < 1$, we have
  $
    t a(t) norm(bold(d)) < t norm(bold(d)),
  $
  so for $d eq.not 0$, if we choose $0< t < r / norm(bold(d))$, we guarantee $bold(y)(t) in B_r (bold(x))$.
  3. We can choose $t = r/(2 norm(d))$.
]
#question[
  Let $f:RR^n -> RR$ be of class $C^1$, and let $bold(x)^* in RR^n$ be a local
  minimizer of $f$. Without using the first-order necessary condition for
  optimality of the theorem, prove the following:
  1. For every $bold(v) in RR^n$,
  $
    nabla f(bold(x)^*)^top bold(v) >= 0.
  $
  2. Deduce that
  $
    nabla f(bold(x)^*) = 0.
  $
]

#proof[
  1. By contradiction, suppose there exists $bold(v) in RR^n$ with $bold(v)^top nabla f(bold(x)^*) =
    nabla f(bold(x)^*)^top bold(v) <0$. By continuity of $nabla f$ near $bold(x)^*$ (because $f$ is $C^1$), there exists a scalar $tau>0$ such that
  $
    bold(v)^top nabla f(bold(x)^* + t bold(v)) < 0 quad "for all " t in [0, tau]
  $
  For any $hat(t) in (0, tau]$, we have by Taylor's theorem that
  $
    f(bold(x)^*+hat(t) bold(v)) = f(bold(x)^*) + hat(t)bold(v)^top nabla f(bold(x)^* +t bold(v)) quad "for some " t in (0, hat(t)).
  $
  Therefore, since $hat(t) > 0$ and $bold(v)^top nabla f(bold(x)^* +t bold(v)) < 0$,
  $
    f(bold(x)^* + hat(t)bold(v)) < f(bold(x)^*)
  $
  for all $hat(t) in (0, tau]$. That is, we have found a direction pointing away from $bold(x)^*$ which decreases $f$. Thus, $bold
  (x)^*$ cannot be a minimizer, contradicting our initial assumption. We then conclude that such $bold(v)$ cannot exist, and thus
  $
    nabla f(bold(x)^*)^top bold(v) >= 0.
  $
  for all $bold(v) in RR^n$.

  2. By the previous result, if we take $bold(v) = - nabla f(bold(x)^*)$, then
  $
    0<=nabla f(bold(x)^*)^top bold(v) & = -nabla f(bold(x)^*)^top nabla f(bold(x)^*) \
                                      & = -norm(nabla f(bold(x)^*))^2 <= 0.
  $
  That is, $norm(nabla f(bold(x)^*))^2 = 0$. By positive-definiteness of the norm, we have
  $
    nabla f(bold(x)^*) = 0.
  $
]


#bibliography("ref.bib")
