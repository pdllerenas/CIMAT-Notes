#import "@preview/unequivocal-ams:0.1.2": ams-article, definition, proof, theorem
#import "@preview/lovelace:0.3.0": *
#import "@preview/lilaq:0.5.0" as lq

#set par(justify: true, first-line-indent: (amount: 2em, all: true))
#set math.equation(numbering: "(1)")

#show: ams-article.with(
  title: [Multi-Secant Limited-Memory BFGS],
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
  bibliography: bibliography("refs.bib"),
  abstract: [We present an implementation in julia of the method of Multi-Secand
    Limited-Memory BFGS proposed by Gubarev in
    @gubarevMultiSecantLimitedMemoryBFGS. We perform tests on the CUTEst dataset
    and compare results with a normal BFGS.],
)

= Introduction
Consider a function $f:RR^n -> RR$ with continuous twice differentiable function
with gradient $g = nabla f$. The minimization problem $min_(x in RR^n) f(x)$
uses the following standard notation:
$
  s_k = x_k - x_(k-1), quad y_k = g_k - g_(k-1),
$
and we build the following matrices:
$
  S_m = [s_(k -m+1), ..., s_k], quad Y_m = [y_(k-m+1), ..., y_k],
$
which are simply the $m$-most recent coordinate displacements ($s_k$), and
the corresponding gradient differences.

The classical Newton direction approach to optimizing $p_k^N = -nabla^2 f_k^(-1) nabla f_k$
has a time complexity of $O(n^3)$. The BFGS methods are quasi-Newton, meaning that
the search direction $p_k = -B_k^(-1) nabla f_k$, where $B_k$ is symmetric
positive definite approximation of the Hessian.

The foundational constraint of this framework is the Secant Equation
$
  B_(k+1)s_k = y_k.
$

Compared to the single-secant algorithm which only takes into account the
product $s^top y$ to measure the curvature, we use the _overlap matrix_, defined
by
$
  O_m = S_m^top Y_m.
$
For any symmetric linear operator (a Hessian approximation) to  satisfy multiple
exact secant conditions ($B S_m = Y_m$), it is necessary and sufficient for
$O_m$ to be symmetric @schnabel1983quasi. However, in general, this cannot be
guaranteed, nor positive definiteness.

= Recursive Block-Factorization and Limited-Memory
== Two Loop Recursion
The standard BFGS method requires $O(n^2)$ storage, which can be a bottleneck
for large-scale systems. The L-BFGS variant circumvents this by "unrolling the
update", which is described by a two loop recursion found in
@nocedalLargeScaleUnconstrainedOptimization2006 and
@nocedalUpdatingQuasiNewtonMatrices.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [L-BFGS two loop recursion])[
    + $q <- nabla f_k$
    + *for* $i = k-1, k-2,...,k-m$
      + $alpha_i <- rho_i s_i^top q$
      + $q <- q - alpha_i y_i$
    + *end*
    + $r <- H_k^0 q$
    + *for* $i = k - m, k - m + 1, ..., k - 1$
      + $beta <- rho_i y_i^top r$
      + $r <- r + s_i (alpha_i - beta)$
    + *end*
    + *return* $H_k nabla f_k = r$
  ],
  caption: [
    The first loop computes $alpha_i = rho_i s_i^top q$ backwards, $r = r+s_i(alpha_i - beta)$ forwards. The time complexity is $O(n m)$.
  ],
)
== MS-LBFGS Compact Form
For enhanced numerical stability and block-processing, MS-LBFGS represents the Hessian as:
$
  B_k = delta_k I + [Y_k delta_k S_k]M_k[Y_k delta_k S_K]^top
$
where $M_k$ is a $2m times 2m$ kernel.

The Woodbury formula for matrix inversion @woodburyInvertingModifiedMatrices1950 allows us to invert
large Hessian approximations at a reduced cost of $O(m^3)$ instead of the full size $O(n^3)$.

= Eigenvalue-Bounding and Symmetric Damping Loop
Numerical stability requires that $B_k$ remains positive definite to ensure $p_k$ is a
descent direction. We must keep track of the *curvature condition* $s_k^top y_k > 0$.
== Damping and Update Skipping
We use strong Wolfe conditions @wolfeConditions during the line search to prevent the search
direction from becoming too positive. If $s_k^top y_k$ is not big enough, we skip the
update on $B_k$.
== Eigenvalue Modification
To satisfy the second-order sufficient conditions, we must bound the eigenvalues of $B_k$ away from zero.
This prevents the search direction from becoming nearly orthogonal to the gradient.

= Convergence Criteria and Computational Performance
The MS-LBFGS architecture is designed for both global reliability and local acceleration.
== Global Convergence
Due to the Zoutendijk Condition @nocedalLineSearchMethods2006, if the gradient is Lipschitz continuous,
and the search direction is not orthogonal to the gradient, then
$
  sum cos^2(theta_k) norm(nabla f_k)^2 < infinity,
$
and thus $norm(nabla f_k) -> 0$.

To achieve Q-superlinear convergence, the search direction must satisfy:
$
  lim_(k -> infinity) (norm((B_k - nabla^2 f(x^*))p_k))/norm(p_k) = 0
$

= Benchmarking

