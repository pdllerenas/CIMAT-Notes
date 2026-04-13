#import "@preview/unequivocal-ams:0.1.2": ams-article, proof, theorem
#import "@preview/lovelace:0.3.0": *
#import "@preview/lilaq:0.5.0" as lq

#set par(justify: true, first-line-indent: (amount: 2em, all: true))
#set math.equation(numbering: "(1)")

#show: ams-article.with(
  title: [],
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
  abstract: [We implement the Multi-Secant Limited-Memory BFGS (MS-LBFGS)
    algorithm proposed by Gubarev in @gubarev2026multisecant.],
  bibliography: bibliography("refs.bib"),
)

= Introduction
We provide a C++ implementation of the MS-LBFGS algorithm proposed by Gubarev
to verify its claimed performance advantages over standard single-secant
methods.

= Algorithm Implementation
The algorithm requires an approximation of the inverse Hessian matrix. The
limited-memory constraint necessitates a distinct, compact representation for
this matrix. Because numerical stability is a priority, we implement the
paper's proposed symmetric damping scheme. This involves dynamically dropping
older secant pairs when specific overlap conditions are not met. Finally, a
numerically stable Armijo line-search optimizer is integrated alongside the
quasi-Newton method.

= Benchmarks
A performance comparison with the classic L-BFGS algorithm will be conducted.
We will first test the algorithm on random quadratics with a high condition
number to observe the multi-secant behavior in an isolated environment.
Subsequently, tests using the CUTEst dataset will be performed to evaluate its
efficiency in practical, large-scale scenarios.

= Evaluation
We will evaluate the overall performance of the algorithm and cross-reference
our findings with the results proposed in the original paper. Additionally,
profiling of the algorithm's memory usage and computational overhead will be
performed.
