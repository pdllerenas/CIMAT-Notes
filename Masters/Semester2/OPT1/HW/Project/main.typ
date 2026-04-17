#import "@preview/unequivocal-ams:0.1.2": ams-article, proof, theorem
#import "@preview/lovelace:0.3.0": *
#import "@preview/lilaq:0.5.0" as lq

#set par(justify: true, first-line-indent: (amount: 2em, all: true))
#set math.equation(numbering: "(1)")

#show: ams-article.with(
  title: [A Review on the Multi-Secant Limited-Memory BFGS Model],
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
Consider the optimization problem $min_(x in RR) f(x)$ with $f in C^2$. Define $g colon.eq nabla f$. Denote by
$
  s_k = x_k - x_(k-1), quad y_k = g_k - g_(k-1)
$
that is, the vector difference of an iteration with its previous iteration. Define the matrices
$
  S_m = [s_(k - m +1), dots, s_k], quad Y_m = [y_(k - m + 1) , dots, y_k]
$
Thus, we write
$
  nabla^2 f S_m = Y_m + O(norm(S_m)^2_F).
$ 


