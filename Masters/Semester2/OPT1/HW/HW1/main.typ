#import "@preview/adaptable-pset:0.2.0": *
#import "@preview/ctheorems:1.1.3": *
#show: thmrules.with(qed-symbol: $square$)

#let proof = thmproof("proof", "Proof")

// Feel free to omit any of the below, just set it to "" and it won't show
#let title = "Tarea 1"
#let author = "Pedro D. Llerenas"
#let collaborators = []
#let course-id = "OPT1: Optimizacion"
#let instructor = "Dr. Oscar Susano\nDr. Dalmau Cedeño\nDr. Thomas Batard"
#let semester = "Enero-Julio 2026"
#let due-time = "Enero 14 1:00"

#set text(lang: "es")

#show: homework.with(
  title: title,
  author: author,
  collaborators: collaborators,
  course-id: course-id,
  instructor: instructor,
  semester: semester,
  due-time: due-time,

  // Optional setting to change the paper size depending on region
  // (Defaults to A4)
  // paper-size: "us-letter",
)

// Numbering
#set enum(numbering: "a)")

#prob(title: "2.1")[
  Prove that the effective domain of a convex function $f$ (that is, the set of
  points $x in RR^n$ such that $f(x) < infinity$) is a convex set.
]
#proof[
  Let $E$ denote the effective domain of $f$. That is,
  $
    E colon.eq { x in RR^n: f(x) < infinity}.
  $
  We must prove that
  $
    x,y in E => (1-alpha)x + alpha y in E quad forall alpha in [0,1].
  $
  By convexity of $f$, we have
  $
    f((1-alpha) x + alpha y) <= (1-alpha)f(x)+alpha f(y)
  $
  for all $x, y in E$ and all $alpha in [0,1]$. Since $x,y in E$, we have
  $
    f((1-alpha) x + alpha y) < infinity.
  $
  Therefore,
  $
    (1-alpha) x + alpha y in E
  $
  for all $x, y in E$ and all $alpha in [0,1]$, so $E$ is convex.
]

#prob(title: "2.2")[
  Prove that $"epi" f$ is a convex subset of $RR^n times RR$ for any convex
  function $f$.
]

#proof[
  Recall that
  $
    "epi" f colon.eq {(x,t) in RR^n times RR | t >= f(x)}.
  $
  We must prove that
  $
    (x, t_1), (y, t_2) in "epi" f => (1-alpha)(x, t_1) + alpha (y, t_2) in "epi" f.
  $
  Rewriting this last expression we obtain
  $
    ((1-alpha) x + alpha y, (1-alpha)t_1 + alpha t_2) in "epi" f.
  $
  Therefore, if we have
  $
    t_1 >= f(x), quad t_2 >= f(y),
  $
  by convexity of $f$, it follows that
  $
    f((1-alpha)x + alpha y) <= (1-alpha)f(x) + alpha f(y) <= (1-alpha)t_1 + alpha t_2
  $
  for all $(x, t_1), (y, t_2) in "epi f"$ and all $alpha in [0,1]$. This proves
  the statement.
]

#prob(title: "2.3")[
  Suppose that $f:RR^n -> RR$ is convex and concave. Show that $f$ must be an
  affine function.
]
#proof[
  If $f$ is convex, we have
  $
    f((1-alpha)x + alpha y) <= (1-alpha)f(x) + alpha f(y).
  $
  If $f$ is concave, we have
  $
    f((1-alpha)x + alpha y) >= (1-alpha)f(x) + alpha f(y).
  $
  Therefore,
  $
    f((1-alpha)x + alpha y) = (1-alpha)f(x) + alpha f(y),
  $
]


#prob(title: "2.4")[
  Suppose that $f:RR^n -> RR$ is convex and upper-bounded. Show that $f$ must
  be a constant function.
]

#proof[
  Suppose $f$ is convex and non-constant. Then, there exists $x,y in RR^n$ with
  $f(x) > f(y)$. By convexity, we have
  $
    f(x) <= (1-alpha)f((x-alpha y)/(1-alpha)) + alpha f(y),
  $
  which implies
  $
    (f(x) - alpha f(y))/(1-alpha) <= f((x-alpha y)/(1-alpha)).
  $
  Since $f(x) > f(y)$,
  $
    lim_(alpha -> 0^+)(f(x) - alpha f(y))/(1-alpha) = infinity,
  $
  so $f$ is not bounded above, contradicting our assumption. Thus, $f$
  must be constant.

]

#prob(title: "2.5")[
  Suppose $f:RR^n -> RR$ is strongly convex and Lipschitz. Show that no such $f$
  exists.
]
#proof[
  If $f$ is strongly convex, we have
  $
    f((1-alpha)x + alpha y) <= (1-alpha)f(x) + alpha f(y) - 1/2 m alpha(1-alpha)norm(x-y)^2_2,
  $
  for some $m > 0$ and all $x,y$ in the domain of $f$. If it is Lipschitz, we have
  $
    abs(f(x) - f(y)) <= L norm(x-y).
  $

]

#prob(title: "2.6")[
  Show rigorously how (2.19) is derived from (2.18) when $f$ is continuously
  differentiable.
]
#proof[
  We must show that
  $
    f((1-alpha)x + alpha y) <= (1-alpha)f(x) + alpha f(y) - 1/2 m alpha(1-alpha)norm(x-y)^2_2,
  $
  implies
  $
    f(y) >= f(x) + nabla f(x)^top (y-x) + m/2 norm(y-x)^2.
  $
  when $f$ is differentiable.

  Indeed, by Taylor's theorem, we have
  $
    f(x + alpha (y-x)) & = f(x) + alpha nabla f(x)^top (y-x) + o(alpha) \
                       & <=(1-alpha)f(x) + alpha f(y) - 1/2 m alpha(1-alpha)norm(x-y)^2_2.
  $
  Canceling the $f(x)$ term, rearranging and dividing by $alpha$ yields
  $
    f(y) >= f(x)+ nabla f(x)^top (y-x) + 1/2 m (1-alpha)norm(x-y)^2_2 + o(1),
  $
  and when $alpha -> 0^+$, the $o(1)$ term vanishes, while $1-alpha -> 1$.
  Therefore, we obtain
  $
    f(y) >= f(x)+ nabla f(x)^top (y-x) + 1/2 m norm(x-y)^2_2.
  $
]

#prob(title: "2.7")[
  Suppose that $f:RR^n -> RR$ is a convex function with $L$-Lipschitz gradient
  and a minimizer $x^*$ with function value $f^* = f(x^*)$.
  1. Show (by minimizing both sides of (2.9) with respect to $y$) that for any
    $x in RR^n$, we have
  $
    f(x) - f^* >= 1/(2L) norm(nabla f(x))^2
  $
  2. Prove the following _co-coercivity_ property: For any $x,y in RR^n$, we have
  $
    [nabla f(x) - nabla f(y)]^top (x-y) >= 1/L norm(nabla f(x) - nabla f(y))^2.
  $
]

#proof[

]

#prob(title: "2.8")[
  Suppose that $f:RR^n->RR$ is an $m$-strongly convex function with
  $L$-Lipschitz gradient and (unique) minimizer $x^*$ with function value $f^*
  = f(x^*)$.
  1. Show that the function $q(x) colon.eq f(x) - m/2 norm(x)^2$ is convex with
    L---$m$-Lipschitz continuous gradients.
  2. By applying the co-coercivity property of the previous question to this
    function $q$, show that the following property holds:
    $
      [nabla f(x) - nabla f(y)]^top (x-y) \
      >= (m L)/(m+L) norm(x-y)^2 + 1/(m+L) norm(nabla f(x) - nabla f(y))^2.
    $
]
#proof[
  The following calculation proves convexity.
  $
    q((1-alpha)x + alpha y) & = f((1-alpha)x + alpha y) - m/2 norm((1-alpha)x + alpha y)^2 \
                            & <= (1-alpha)f(x) + alpha f(y) - m/2 alpha (1-alpha) norm(x-y)_2^2 - m/2 norm(x+alpha(y-x))^2 \
                            & = (1-alpha)f(x)+alpha f(y) -m/2[norm(alpha (1-alpha) (x-y))^2 + norm(x+alpha(y-x))^2] \
                            & = 
  $

]


#pagebreak(weak: true)
