#import "@preview/unequivocal-ams:0.1.2": ams-article, definition, theorem
#import "@preview/lovelace:0.3.0": pseudocode-list

#set par(justify: true, first-line-indent: (amount: 2em, all: true))
#set math.equation(numbering: "(1)")

#let note(body) = block(
  fill: luma(240),
  inset: 8pt,
  radius: 4pt,
  width: 100%,
)[#text(size: 0.9em)[#body]]

#show: ams-article.with(
  title: [Multi-Secant Limited-Memory BFGS: \ Implementation in Julia],
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
  abstract: [
    We present an implementation in Julia of the Multi-Secant Limited-Memory
    BFGS (MS-LBFGS) method proposed by Gubarev (2024). The method extends the
    classical L-BFGS by simultaneously incorporating multiple secant conditions
    through a variational framework, preserving positive definiteness of the
    Hessian approximation without external parameters. The implementation is
    structured in five phases: data structure definitions, variational kernel
    evaluation, symmetric damping, compact representation, and a full optimizer
    with line search. Correctness is validated via unit tests and numerical
    experiments on the CUTEst benchmark collection.
  ],
)

// =============================================================================
= General Objective
// =============================================================================

The objective of this project is to implement, in the Julia programming
language, the MS-LBFGS algorithm described in Gubarev (2024), and to compare
its performance against standard L-BFGS on unconstrained optimization problems
from the CUTEst collection. The implementation covers everything from internal
data structures to a complete optimizer with line search, symmetric damping,
and limited-memory compact representation.

// =============================================================================
= Problem Description
// =============================================================================

Consider the unconstrained optimization problem

$
  min_(x in RR^n) f(x),
$ <eq:problem>

where $f : RR^n -> RR$ is twice continuously differentiable with gradient
$g = nabla f$. In the iterative context, matrices of recent displacements are
assembled as

$
  S_m = [s_(k-m+1), dots, s_k] in RR^(n times m), quad
  Y_m = [y_(k-m+1), dots, y_k] in RR^(n times m),
$

with $s_k = x_k - x_(k-1)$ and $y_k = g_k - g_(k-1)$, so that

$
  nabla^2 f med S_m = Y_m + cal(O)(norm(S_m)_F^2).
$

The *quasi-Newton search direction* is $p_k = -H_k g_k$, where $H_k$ is a
symmetric positive definite (SPD) approximation of the inverse Hessian.

== Motivation for the Multi-Secant Approach

The classical BFGS method enforces a single secant condition
$H_(k+1) y_k = s_k$, discarding the remaining curvature history. A natural
question is: can the full window of $m$ recent pairs $(s_i, y_i)$ be
exploited simultaneously?

The main obstacle is that the *overlap matrix*

$
  O_m = S_m^top Y_m in RR^(m times m)
$

is, in general, neither symmetric nor positive definite. Symmetry of $O_m$ is
necessary and sufficient for the existence of a symmetric linear operator
satisfying multiple exact secant conditions (Schnabel, 1983). Beyond $m=1$,
this symmetry is not guaranteed.

Gubarev (2024) resolves this tension through a *variational formulation*: one
seeks the SPD matrix $H$ closest to $H_0$ satisfying $H Y_m approx S_m$ in a
penalized sense, with the penalty weights eliminated analytically in the limit
$alpha -> +infinity$.

// =============================================================================
= Methodology, Algorithm and Procedure
// =============================================================================

The implementation is organized into five modular phases corresponding to the
files of the `MSLBFGS.jl` package.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],
  pseudocode-list(booktabs: true, numbered-title: [Package structure of MSLBFGS.jl])[
    + `State.jl` — Data structures and memory preallocation
    + `Kernel.jl` — Variational kernel evaluation: $K_R$, $K_L$
    + `Damping.jl` — Dynamic secant truncation and symmetric damping
    + `CompactRepresentation.jl` — Recursive factors $X$, $R$ and $H$-vector product
    + `Optimization.jl` — Line search and main optimizer loop
  ],
  caption: [Modular organization of the MS-LBFGS implementation.],
)

== Data Structures (`State.jl`)

The mutable struct `MSLBFGS_State{T}` centralizes the full algorithm state
and enables complete memory preallocation before the main iteration loop,
avoiding dynamic allocations in the inner loop.

```julia
mutable struct MSLBFGS_State{T <: AbstractFloat}
    n::Int          # problem dimension
    L::Int          # maximum memory length
    m::Int          # current active window (m ≤ L)
    S::Matrix{T}    # coordinate displacements    (n × L)
    Y::Matrix{T}    # gradient differences         (n × L)
    O::Matrix{T}    # overlap  O = Sᵀ Y            (L × L)
    K_R::Matrix{T}  # right kernel  √(O Oᵀ)        (L × L)
    K_L::Matrix{T}  # left kernel   √(Oᵀ O)        (L × L)
    R::Matrix{T}    # upper triangular factor       (L × L)
    X::Matrix{T}    # effective overlap             (L × L)
end
```

The key parameters are $L$ (total memory) and $M <= L$ (maximum active
secants per step). Unlike L-BFGS, which stores only vectors, MS-LBFGS requires
the matrices $O$, $K_R$, $K_L$, $X$, and $R$ of size $L times L$.

== Variational Kernels (`Kernel.jl`)

In the limit $alpha -> +infinity$ of the variational formulation, the MS-BFGS
inverse Hessian update reduces to

$
  H = P_m^top H_0 P_m + S_m K_R^(-1) S_m^top,
$ <eq:update>

where

$
  P_m = I - Y_m O_m^(-1) S_m^top, quad
  K_R = sqrt(O_m O_m^top), quad
  K_L = sqrt(O_m^top O_m).
$

The function `evaluate_dense_kernels!` computes $O_m = S_m^top Y_m$ and then
$K_R$, $K_L$ via Julia's `sqrt(Symmetric(...))`, guaranteeing SPD results.
Optionally, if exactness on the most recent secant ($H y_m = s_m$) is
required, the modified kernels $tilde(K)_R$ and $tilde(K)_L$ are applied:

$
  tilde(K)_R = K_R
  - frac((K_R e_m)(K_R e_m)^top, e_m^top K_R e_m)
  + frac(o_m o_m^top, e_m^top o_m),
$

where $o_m = O_m e_m$ is the last column of $O_m$.

== Symmetric Damping (`Damping.jl`)

=== Dynamic Secant Truncation

Non-singularity of $O_m$ is enforced via the eigenvalue bounding conditions

$
  det(K_R) >= epsilon_s det(S_m^top B S_m),
  quad quad
  (op("Tr")[K_L^(-1)])^(-1) >= epsilon_y op("Tr")[Y_m^top H Y_m],
$ <eq:stability>

with defaults $epsilon_s = 10^(-2)$, $epsilon_y = 10^(-3)$. If either bound
fails, the oldest pair is dropped and the test is repeated, reducing $m$ down
to one if necessary.

=== Symmetric Damping at $m = 1$

When $m=1$, condition @eq:stability reduces to
$|s^top y| >= max{epsilon_s (s^top B s), med epsilon_y (y^top H y)}$.
Rather than Powell's asymmetric damping (which modifies only $y$), we adopt
the symmetric, affine-invariant deformation

$
  s arrow.l (1 - theta_s) s plus.minus theta_s H y, quad
  y arrow.l (1 - theta_y) y plus.minus theta_y B s,
$ <eq:damping>

with $theta_s, theta_y in [0, 1/2]$ found by solving the 2D subproblem

$
  min_(theta_s, theta_y) theta_s^2 + theta_y^2
  quad "s.t." quad
  a^top b >= epsilon max{|a|^2, |b|^2},
$

where $a = sqrt(epsilon_s) B s + theta_s Delta$,
$b = plus.minus sqrt(epsilon_y) H y - theta_y Delta$,
$epsilon = sqrt(epsilon_s epsilon_y)$. This is solved efficiently via 1D
bisection projected onto the diagonal $theta_s = theta_y$.

== Compact Representation (`CompactRepresentation.jl`)

The compact representation avoids forming the $n times n$ matrix explicitly,
storing only $X, R in RR^(L times L)$ and applying $H$ to vectors in
$O(L^2 n)$ operations. The final formula is

$
  H = Pi^top H_0 Pi + S R^(-top) R^(-1) S^top,
  quad Pi = I - Y X^(-1) S^top,
$ <eq:compact>

which generalizes the compact representation of Byrd et al. (1994) to the
multi-secant setting.

=== Recursive Update of $X$ (Effective Overlap)

The lower-left block encodes the interaction between displacements from
different active windows:

$
  X_[3,1] = O_[3,2] med O_[2,2]^(-1) med X_[2,1].
$

=== Recursive Update of $R$

$R$ is upper triangular with factor $r$ such that $K_R = r^top r$:

$
  R_(1:k-1, med k) = x_(2,3) dot r_(k,k), quad
  x_(2,3) = O_(1:k-1, med 1:k-1)^(-1) O_(1:k-1, med k),
$

where $r_(k,k)$ is the diagonal element of the upper Cholesky factor of $K_R$.

=== Applying $H$ to a Vector

```julia
function apply_compact_hessian(state, v, H0_scale)
    Stv      = Sk' * v
    # Inhomogeneous term: S R⁻ᵀ R⁻¹ Sᵀ v
    w        = UpperTriangular(Rk) \ Stv
    z        = LowerTriangular(Rk') \ w
    inhomo_v = Sk * z
    # Homogeneous term: Πᵀ H₀ Π v
    coeff    = Xk \ Stv
    Pi_v     = v - Yk * coeff
    H0Piv    = H0_scale .* Pi_v
    coeff2   = Xk' \ (Yk' * H0Piv)
    homo_v   = H0Piv - Sk * coeff2
    return homo_v + inhomo_v
end
```

== Optimizer and Line Search (`Optimization.jl`)

#figure(
  kind: "algorithm",
  supplement: [Algorithm],
  pseudocode-list(booktabs: true, numbered-title: [optimize_mslbfgs — Main loop])[
    + Initialize state; compute $g_0$; set $tau = min(max(epsilon_g max{1, norm(g_0)_infinity}, epsilon_g^min), epsilon_g^max)$
    + *while* $norm(g)_infinity > tau$ *and* iter $<$ max\_iter *do*
      + Compute $gamma^* = |s^top y| \/ norm(y)^2$; obtain $d = -H g$ via `apply_compact_hessian`
      + If $g^top d >= 0$: revert to $d = -g$ (descent direction safeguard)
      + Line search with strong Wolfe conditions and guarded quadratic reduction
      + If line search fails: reset state ($m <- 0$) and continue
      + Compute $s_k = x_"new" - x$, $y_k = g_"new" - g$
      + Insert into circular memory buffer (shift if $m = L$)
      + Call `enforce_overlap_stability!`; call `update_compact_factors!`
      + Advance $x <- x_"new"$, $g <- g_"new"$; iter $<-$ iter $+ 1$
    + *end*
    + *return* $x$, $f(x)$, iter, $norm(g)_infinity <= tau$
  ],
  caption: [Main loop of the MS-LBFGS optimizer.],
)

=== Convergence Criterion

The algorithm terminates successfully when

$
  norm(g)_infinity <=
  min (
    max[epsilon_g max{1, norm(g_0)_infinity},;
      epsilon_g^min],;
    epsilon_g^max
  ),
$

with defaults $epsilon_g = 10^(-8)$, $epsilon_g^min = 10^(-4)$,
$epsilon_g^max = 1$.

=== Initial Scale $H_0$

Following Section 4.4 of the paper, the scalar scale is chosen as

$
  gamma^* = frac(norm(r_m^(-1) O_m)_F^2, norm(Y_m)_F^2),
$

which in the single-secant limit reduces to $(|s^top y|) / norm(y)^2$
(always positive, sign-invariant).

== Unit Tests

The test suite covers all five phases of the algorithm:

#figure(
  table(
    columns: (auto, 1fr),
    align: (center, left),
    stroke: 0.5pt,
    inset: 7pt,
    table.header(text(weight: "bold")[File], text(weight: "bold")[Coverage]),
    [`test_init.jl`], [Dimensions and initial values of `MSLBFGS_State`.],
    [`test_overlap.jl`], [Evaluation of $O_m$, $K_R$, $K_L$; standard mode and with enforced exactness.],
    [`test_damping.jl`], [Dynamic secant truncation; symmetric damping at $m=1$.],
    [`test_compact.jl`], [Update of $X$ and $R$ for $m=1,2,3$; `apply_compact_hessian`; upper triangularity of $R$.],
    [`test_optimization.jl`], [2D Rosenbrock; 100D convex quadratic; initial point at minimum; return types.],
  ),
  caption: [Coverage of the unit test suite (`runtests.jl`).],
) <tab:tests>

// =============================================================================
= Benchmarking and Analysis of Results
// =============================================================================

== Test Environment

Benchmarking is performed via the script `GenerateProfile.jl`, which:

+ Selects unconstrained CUTEst problems with at least 4 variables, excluding
  18 numerically unstable instances (blacklist from the original paper).
+ For each problem runs two passes: default initial point and a randomized one,
  $x_(0,i) + 0.5 med xi_i med max{1, |x_(0,i)|}$ with $xi_i tilde cal(U)[-1,1]$.
+ Compares two methods: MS-LBFGS, and L-BFGS-B, both with $L = M = 5$. We
  attempt various combinations with iterations and dimensions.
+ Generates a Dolan-Moré performance profile on gradient evaluation counts.

== Performance Metrics

The performance profile $P_g (tau)$ measures the probability that method $s$
requires at most $tau$ times more gradient evaluations than the best method on
problem $p$:

$
  P_g (tau) = op("Prob")_(p in tilde(P)) [N_(s,p) <= tau med N_p^min],
  quad tau >= 1.
$

A method with a higher curve is more efficient and robust; $P_g (1)$ gives the
fraction of problems on which the method is fastest.

== Results

Now, randomizing the selected problems, we find a different story when comparing to the L-BFGS-B method.
The following graphs show the Dolan-Moré graph for a random selection of 30 problems from the CUTEst set (excluding some).

#figure(
  image("profile_it10000_dim100.png"),
  caption: [
    Profiling for 10000 max iterations and max dimension of 100. We observe
    a clear performance advantage of the baseline method with 30 problems.
  ]
)
#figure(
  image("profile_it10000_dim100_2.png"),
  caption: [
    Profiling for 10000 max iterations and max dimension of 100. We observe
    a clear performance advantage of the baseline method with 30 problems.
  ]
) 
#figure(
  image("profile_it10000_dim100_3.jpeg"),
  caption: [
    Profiling for 10000 max iterations and max dimension of 1000. We observe
    a clear performance advantage of the baseline method with 30 problems.
  ]
) 
#figure(
  image("profile_it10000_dim100_4.jpeg"),
  caption: [
    Profiling for 10000 max iterations and max dimension of 1000. We observe
    a clear performance advantage of the baseline method with 30 problems.
  ]
)
#figure(
  image("profile_it_10000_dim100_5.png"),
  caption: [
    Profiling for 10000 max iterations and max dimension of 1000. We observe
    a clear performance advantage of the baseline method with 30 problems.
  ]
)
#figure(
  image("profile_it10000_dim100_6.jpeg"),
  caption: [
    Profiling for 10000 max iterations and max dimension of 1000. We observe
    a clear performance advantage of the baseline method with 149 problems.
  ]
)
On average, our implementation of MS-LBFGS using strong Wolfe-Conditions performed worse than the baseline.

// =============================================================================
= Conclusions
// =============================================================================
While Gubarev originally concluded that MS-LBFGS broadly outperformed the
L-BFGS-B baseline, our empirical replication suggests a different narrative when
evaluated under strictly symmetric conditions. Gubarev explicitly notes that his
comparison utilized an Armijo line search for MS-LBFGS, which avoids
intermediate gradient evaluations, while the baseline was subjected to Strong
Wolfe conditions. By implementing a Strong Wolfe line search for both algorithms
in our replication, we eliminated this metric bias.

The obtained results indicate that our implementation of the MS-LBFGS method
does not outperform the baseline L-BFGS-B. Further implementation revisions
may be done to ensure consistency of the method.

// =============================================================================
= References
// =============================================================================

#set par(first-line-indent: 0em)

#v(0.3em)
*Gubarev, F. V.* (2026). A Multi-Secant Limited-Memory BFGS Method.
_pSeven SAS Technical Report_.

#v(0.3em)
*Byrd, R. H., Nocedal, J., and Schnabel, R. B.* (1994). Representations of
quasi-Newton matrices and their use in limited memory methods.
_Mathematical Programming_, 63, 129–156.

#v(0.3em)
*Schnabel, R. B.* (1983). Quasi-Newton Methods Using Multiple Secant Equations.
_University of Colorado at Boulder_, Report CU-CS-247-83.

#v(0.3em)
*Nocedal, J.* (1980). Updating quasi-Newton matrices with limited storage.
_Mathematics of Computation_, 35(151), 773–782.

#v(0.3em)
*Nocedal, J. and Wright, S. J.* (2006). _Numerical Optimization_ (2nd ed.).
Springer.

#v(0.3em)
*Powell, M. J. D.* (1978). Algorithms for nonlinear constraints that use
Lagrangian functions. _Mathematical Programming_, 14, 224–248.

#v(0.3em)
*Gould, N. I. M., Orban, D., and Toint, Ph. L.* (2015). CUTEst: a constrained
and unconstrained testing environment with safe threads for mathematical
optimization. _Computational Optimization and Applications_, 60(3), 545–557.

#v(0.3em)
*Dolan, E. and Moré, J.* (2002). Benchmarking Optimization Software with
Performance Profiles. _Mathematical Programming_, 91(2), 201–213.
