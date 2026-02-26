#import "@preview/unequivocal-ams:0.1.2": ams-article, definition, proof, theorem

#set par(justify: true, first-line-indent: (amount: 2em, all: true))
#set math.equation(numbering: "(1)")

#show: ams-article.with(
  title: [Addendum to Homework 3],
  authors: (
    (
      name: "Pedro Llerenas",
      email: "pedro.llerenas@cimat.mx",
    ),
  ),
)

= Concrete Contributions
I created the file `gradient.jl` along with the entirety of its content. The file contains the Armijo algorithm for determining the step size and the general gradient descent function.

= Technical Decisions
I decided to use Mersenne Twister as my pseudo random number generator for the random initialization due to some online suggestions mentioning its improvements over the generic pseudo random number generator.

I also decided to make the functions as general as possible for future use, such as passing in the algorithm for determining the step.

= Problems Encountered and Solutions
Initially, I had set the maximum iterations to a low number, without letting it converge. After some discussions with my peers, it was evident that I did not have enough iterations. Hence, once letting it iterate enough times, I found a more conclusive result.