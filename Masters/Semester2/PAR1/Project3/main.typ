#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lovelace:0.3.1": *


#import exercise: project, subtask, task

#set math.mat(delim: "[")
#set math.vec(delim: "[")

#show: project.with(
  title: "A Parallelization of the Harmony Search Meta-Heuristic Algorithm",

  university: [Centro de Investigación en Matemáticas],
  institute: [Maestría en Ciencias de la Computación],
  abstract: [
    We implement the Harmony Search @HeuristicOptimization2001 in `C++` as a
    sequential program and also another version using OpenMP and MPI. We
    perform some tests on the Rosenbrock and Michaelwicz funtions to prove
    correctness of the algorithm. Then, we apply the algorithm to the function
    $
      V(i) = E_0^* -b log(i) - R e dot i + C_1 log(1 - C_2 i)
    $
    and find that the parallelized version performs better in time compared
    to the sequential version.
  ],
  show-outline: true,

  author: "Pedro D. Llerenas",
  show-solutions: false,
)

= Introduction
Optimization problems can be found throughout nature, and thus human life. Any time we make a decision, we attempt to reduce
some type of cost. Sometimes it is time, money, tangible or intangible resources. This can be posed as:

#align(
  center,
  [_In my current situation (modeling the problem), what decision can I make to approach the best outcome (based on some previous observations or assumptions on the model)? _],
)

Optimization techniques date back to Newton, Bernoulli, Lagrange, Cauchy and
Gibbs. These techniques were developed from abstract mathematics, and their aim
is to find the best solution without concern for the amount of time it might
take to converge to a solution, or even the time complexity needed to calculate
an operator, such as the inverse of a Hessian matrix.

More recently, techniques such as linear programming, non-linear programming,
and dynamic programming were introduced to solve more complex problems. These
are also deterministic, and thus suffer the same flaw as the previously mentioned
algorithms.

On the other hand, meta-heuristic algorithms aim to obtain an approximation to the real solution in
a relatively short amount of time. In practice, this is often more than enough
when time is limited, and obtaining the best solution does not yield the best
time-efficiency metric. These differ from approximation algorithms in the sense
that here, we follow some intelligent (or a naturally predefined process) model,
and use randomization to follow the actions of what an intelligent model would
perform.

= Harmony Search
== Music-Based Algorithms
Harmony Search was first introduced in @HeuristicOptimization2001 in 2001, and has been used in
problems such as water distribution networks and groundwater modeling.

The algorithm is based on the idea of a musician improvising, searching for a _perfectly pleasing_ harmony.
This harmony in music is analogous to finding optimality in an optimization process. The heuristic which
determines _perfectly pleasing_ harmony is given by the _audio aesthetic standard_ @HarmonySearch2010.

The aesthetic quality of a musical instrument is essentially determined by its
pitch (or frequency), timbre (or sound quality), and amplitude (or loudness).

In music theory, _pitch_ $p$ in Musical Instrument Digital Interface (MIDI) is often represented by the
formula
$
  p = 69 + 12 log(f/440"Hz")
$
This means that the A4 notes have a pitch number of 69. In this scale, octaves
correspond to size 12 and semitone correspond to size 1. The ratio of frequencies of two
notes is doubled when raised by an octave. For example, A2 has a frequency of $110$ Hz, while A5
has a frequency of $880$ Hz.

One can easily see through the subjectiveness of these standards when combining various
frequencies. A standard estimation for harmony is used in this case. The frequency ratio
used by Pythagoras is an example of a standard estimation of musical harmony. For example,
a frequency ration of 1:2 is said to sound pleasant, and so does 2:3.


== Harmony Search
The core concept of _harmony search_ can be simplified to a musician improvising a piece,
they have either 3 of the following choices:
- Play any famous piece of music (a series of pitches in harmony) exactly from their memory;
- Play something similar to a know piece (this is equivalent to adjusting the pitch slightly);
- Compose new or random notes.

To formalize these choices for optimization, we have three corresponding components:
- Usage of harmony,
- Pitch adjusting,
- Randomization.

Harmony memory is important, as it allows us to choose the best fit notes for our harmony and future harmonies
Similar to genetic algorithms choosing the best fit individuals.

#let HM = "HM"
#let HMS = "HMS"
#let HMCR = "HMCR"
#let PAR = "PAR"
#let bw = "bw"

We thus define the following concepts:
- Harmony Memory ($HM$): A 2d-array that stores the best harmonies so far.
- Harmony Memory Size ($HMS$): The number of solution vectors stored in $HM$.
- Harmony Memory Considering Rate ($HMCR$): A probability between 0 and 1 that
  determines if the algorithm should pick a value from the existing $HM$. Typically in $(0.7,0.95)$.
- Pitch Adjusting Rate ($PAR$): A probability between 0 and 1 that determines if
  a value chosen from the $HM$ should be slightly tweaked.
- Bandwidth ($bw$): The maximum step size used when making a pitch adjustment.

We define the steps as
$
  x_(k+1) = x_k + b_p (2 "rand" - 1)
$
where rand has a uniform distribution in $[0,1]$, and $b_p$ controls the local
range of pitch adjustment.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Harmony Search])[
    + *input* Objective function $f(x)$, $x in R^p$.
    + generate initial harmonics
    + define PAR and pitch limits
    + *while* t < Max Iterations
    + generate new initial harmonics
      + *if* rand > r"SA" *then*
      Choose an existing harmonic melody
      + *else if* (rand, $p_a$)
        + Adjust the pitch randomly within a bandage.
      + *end*
      + *else* 
    + *end*
  ],
) <cool>

#bibliography("ref.bib")
