#import "@preview/grape-suite:3.1.0": exercise
#import "@preview/lovelace:0.3.1": *

#import exercise: project, subtask, task

#set math.mat(delim: "[")
#set math.vec(delim: "[")
#set math.equation(numbering: "(1)")

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
- Play something similar to a known piece (this is equivalent to adjusting the pitch slightly);
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
$<eq:bw>
where rand has a uniform distribution in $[0,1]$, and $b_p$ controls the local
range of pitch adjustment. In essence, @eq:bw is a random walk. If the $PAR$ is too low,
rarely any changes will me made. If it is too high, the algorithm may never converge,
as any change will likely be accepted. Thus, $PAR = 0.1 tilde 0.5$ is the standard for most simulations.

The third component is randomization, which is to increaste the diversity of the solution. Although
the pitch selection contains randomization, it is restricted to a certain local pitch adjustment,
i.e., a local search. We define
$
  p_a = p_("lower limit") + p_"range" * "rand"
$
where $p_"range" = p_"upper limit" - p_"lower limit"$.

We thus summarize these three components for the Harmony Search into the following
pseudocode.

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Harmony Search])[
    + *input* Objective function $f(x)$, $x in RR^p$.
    + generate initial harmonics
    + define PAR and pitch limits
    + *while* t < Max Iterations
    + generate new harmonics by accepting best harmonics
    + adjust pitch to get new harmonics (solutions)
      + *if* rand > $HMCR$ *then*
      + choose an existing harmonic randomly
      + *else if* rand > $PAR$
        + adjust the pitch randomly within a bandwidth @eq:bw.
      + *else*
        + generate new harmonics via randomization
      + *end*
      + *else*
    + *end*
  ],
) <alg:hs>

We observe that the true probability of randomization is
$
  P_"random" = 1 - HMCR.
$
and the actual probability of pitch adjusting is
$
  P_"pitch" = HMCR * PAR.
$
According to @HarmonySearch2010, there is evidence that suggests that HS is less
sensitive to the chosen parameters.

= Implementation

We provide an adapted implementation from the one presented in @HarmonySearch2010, which
is written in MATLAB/Ocvate.

== Main

We divide our experiment in various files. The main function's concern is
to read command line arguments and set thread numbers. Then, to read
the experimental data and hyperparameter values which are to be
used in the Harmonic Search. Then, after running the search
for each objective function, we output the best obtained values
to different csv files. Note that the following snippet does not
represent the actual code. In the original source code, we use MPI
to send each objective function to a different processor, which
will then use the set threads by OpenMP and perform the search. There
is no communication whatsoever between the processes.

#figure(
  ```cpp
    int main(int argc, char *argv[])
  {
    vector<DataPoint> experiment_data = read_experimental_data(filename);

    vector<ObjectiveTask> tasks;

    tasks.push_back({"sse", [&experiment_data](const Point &p)
                     { return objective_SSE(p, experiment_data); }});
    tasks.push_back({"sae", [&experiment_data](const Point &p)
                     { return objective_SAE(p, experiment_data); }});
    tasks.push_back({"mae", [&experiment_data](const Point &p)
                     { return objective_MAE(p, experiment_data); }});

    string out_filename = tasks[rank].name + "_output.csv";
    ofstream out_file(out_filename);

    Point x = HarmonicSearch(tasks[rank].func, hp);

    for (size_t i = 0; i < x.size(); ++i)
    {
      out_file << x[i] << (i < x.size() - 1 ? ", " : "");
    }

    return 0;
  }
  ```,
  caption: [
    Extreme simplification of the main function. We define a `vector` of tasks, which contain an objective function
    to optimize.
  ],
)

== Search

The implementation of the search algorithm is found in `search.cpp`. There, we store
the current set of best candidate solutions in `HM`, and `HMBest` stores the
objective values of those solutions. The Random object is thread-safe (this is explained in the next
implementation snippet). We first initialize the harmony values randomly using the
hyperparameter values of lower and upper bounds. The solutions are evaluated and stored in `HMBest`.

We construct new candidates according to the pseudocode. Once all candidates are generated,
we evaluate and compare to the worst stored harmonies in `HM`. We then return the harmony
with the lowest evaluated objective value.

The use of OpenMP here is quite simple. We completely parallelize the initialization loop,
which uses a thread-safe Random class implementation to ensure true randomness.

Then, we parallelize on the number of iterations. Using it on the dimensions does
not yield a good result, as our main problem consists of 5 dimensions only.

#figure(
  ```cpp
  Point HarmonicSearch(Func f, HParams hp)
  {
    std::vector<std::vector<double>> HM(hp.HS_size, std::vector<double>(hp.ndim));
    std::vector<double> HMBest(hp.HS_size);

    Random r;

  #pragma omp parallel for default(none) shared(hp, HM, HMBest, r, f)
    for (int i = 0; i < hp.HS_size; ++i)
    {
      for (int j = 0; j < hp.ndim; j++)
      {
        HM[i][j] = r.GetRand(hp.lower_bound[j], hp.upper_bound[j]);
      }
      HMBest[i] = f(HM[i]);
    }

  #pragma omp parallel default(none) shared(hp, HM, HMBest, r, f)
    {
  #pragma omp for
      for (int k = 0; k < hp.MaxAttempt; k++)
      {
        for (int j = 0; j < hp.ndim; ++j)
        {
          if (r.GetRand() >= hp.HMacceptRate)
          {
            new_x[j] = r.GetRand(hp.lower_bound[j], hp.upper_bound[j]);
          }
          else
          {
            int random_hm_index = floor(hp.HS_size * r.GetRand());
            new_x[j] = HM[random_hm_index][j];

            if (r.GetRand() <= hp.PArate)
            {
              double pa = (hp.upper_bound[j] - hp.lower_bound[j]) / hp.pa_range[j];
              new_x[j] = new_x[j] + pa * (r.GetRand() - 0.5);
            }
          }
        }
      }
    }

    // find best index
    //  ...
    return HM[best_idx];
  }
  ```,
  caption: [Simplification of the implementation of Harmony Search.],
)

== Random
In `rand.h`, we implement a thread-safe @rudolphAnswerHowGenerate2014 wrapper of @StdMersenne_twister_engineCppreferencecom for generation of
pseudo-random numbers in a given range.
#figure(
  ```cpp
  class Random
  {
  public:
    Random() {}

    double GetRand(double lower, double upper)
    {
      static thread_local std::mt19937 generator(std::random_device{}() + std::hash<std::thread::id>{}(std::this_thread::get_id()));
      std::uniform_real_distribution<double> dis(lower, upper);
      return dis(generator);
    }

    double GetRand()
    {
      return GetRand(0.0, 1.0);
    }
  };
  ```,
  caption: [
    Implementation of pseudo-random generator.
  ],
)

== Models
Then, we have `model.h`, which contains all the functions we will be testing the search algorithm on.
We will first test it on the Rosenbrock @RosenbrockFunction and Michalewicz @MichalewiczFunction. We
implement them for $d = 2$.
$
  "Rosenbrock: " f(x,y) = (1-x)^2 + 100(y - x^2)^2
$

$
  "Michalewicz: " f(x,y) = -sin(x) sin(x)^(20) (x^2 / pi) - sin(y) sin^(20) ((2y^2)/pi).
$

#grid(
  columns: (1fr, 1fr),
  figure(
    image("images/rosen.png"),
  ),
  figure(
    image(
      "images/michal.png",
    ),
  ),
)
The primary function of concern describes cell voltage versus current density response of a liquid feed direct methanol fuel cell
presented in @argyropoulosSemiempiricalModelDirect2003. The equation proposed equation is
$
  E_"cell" = E_o^* - b log j - R_e j + C_1 ln (1- C_2 j).
$
We implement a safeguard on the values of $j$ and $1-C_2 j$ to prevent ill-valued evaluations.

We will optimize various objective functions which aim to minimize the error
from the model equation and the empirical data provided in `datos_358.txt`.
The objective is to find the best values of $theta = (E_0^*, b, R_e, C_1, C_2)$
that minimize the SSE, SAE or MAE.

#let data = csv(
  "src/datos_358.csv",
)

#figure(
  table(
    stroke: none,
    columns: 2,
    table.hline(),
    table.header([*Current Density*], [*Cell Voltage*]),
    table.hline(),
    ..data.flatten(),
    table.hline(),
  ),
)

== I/O
This last model requires some input data, so we implemented a custom data insertion function and hyperparameter insertion function.
The output is written directly to some predefined files, containing the parameters which best find the empirical data. Note
that this is not parallelized, as synchronization is expensive and would yield little to no benefit, as it is not intrinsically part
of the search algorithm.

= Tests
== Rosenbrock and Michalewicz functions
It is well-known that the point that minimizes the Rosenbrock function is $(1,1)$ and evaluates to 0. Therefore, we will verify that our implementation
of the harmony search correctly converges to this value. The Michalewicz function converges to $f(x^*) = -1.8013...$ at $(2.20, 1.57)$. For higher dimensions, these value change.

#figure(image("images/rosenbrock.png"), caption: [$f(x,y) = (1-x)^2 + 100(y-x^2)^2$])

We can notice that the accumulation of points is around the global minimum. Although it does not reach it perfectly, a good enough approximation was reached in a small amount of time.

#figure(image("images/michalewicz.png"), caption: [$f(x,y) = -sin(x) sin^(20) (x^2/pi) - sin(y) sin^(20) ((2y^2)/pi)$])

=== Performance
#figure(
  table(
    columns: 7,
    table.hline(),
    stroke: none,
    table.header([*Processes*], [*Threads*], [*Function*], [*Time(s)*], [*Total  (s)*], [*Speedup*], [*Efficiency*]),
    table.hline(),
    [1], [1], [Rosenbrock], [0.45], [], [], [],
    [], [1], [Michalewicz], [0.51], [0.96], [1], [1],
    table.hline(),
    [2], [1], [Rosenbrock], [0.50], [], [], [],
    [], [1], [Michalewicz], [0.50], [0.5], [1.6], [0.8],
    table.hline(),
    [2], [2], [Rosenbrock], [0.85], [], [], [],
    [], [2], [Michalewicz], [0.81], [0.85], [1.18], [0.29],
    table.hline(),
    [2], [4], [Rosenbrock], [0.67], [], [], [],
    [], [4], [Michalewicz], [0.68], [0.68], [1.41], [0.17],
    table.hline(),
    [2], [8], [Rosenbrock], [0.69], [], [], [],
    [], [8], [Michalewicz], [0.68], [0.69], [1.39], [0.08],
    table.hline(),
    [2], [16], [Rosenbrock], [0.91], [], [], [],
    [], [16], [Michalewicz], [0.93], [0.93], [1.03], [0.03],
    table.hline(),
  ),
)
== Direct Methanol Fuel Cell
We want to analyze the function
$
  E_"cell" = E_o^* - b log j - R_e j + C_1 ln (1- C_2 j).
$
which models the DMFC. We do not know the parameters $theta = (E_0, b, R_e, C_1, C_2)$. Therefore, we aim to minimize the functions
#let SSE = "SSE"
#let SAE = "SAE"
#let MAE = "MAE"
$
  SSE(theta) & = sum_(i = 1)^m abs(V_i^"FC" - V(i))^2, \
  SAE(theta) & = sum_(i = 1)^m abs(V_i^"FC" - V(i)), \
  MAE(theta) & = "median"{abs(V_i^"FC" - V(i))}, \
$

== Results
#figure(
  image("images/Fitted.png"),
)

#let datasse = csv("src/sse_output.csv")
#let datasae = csv("src/sae_output.csv")
#let datamae = csv("src/mae_output.csv")
#figure(
  table(
    columns: 7,
    stroke: none,
    table.hline(),
    table.header([*Parameters*], [$E_0^*$], [$b$], [$R_e$], [$C_1$], [$C_2$], [$f_"obj"$]),
    table.hline(),
    [*Reported*], [0.432], [0.123], [0.0413], [0.108], [29.4], [...],
    table.hline(),
    [*SSE*], ..datasse.flatten(), [0.060891],
    [*SAE*], ..datasae.flatten(), [0.541628],
    [*MAE*], ..datamae.flatten(), [0.0352766],
    table.hline(),
  ),
  caption: [Results from an execution. (Randomness plays a part of the results shown)],
)
=== Performance

#figure(
  table(
    columns: 7,
    table.hline(),
    stroke: none,
    table.header(
      [*Processes*],
      [*Threads*],
      [*Function*],
      [*Time (s)*],
      [*Total
      Time (s)*],
      [*Speedup*],
      [*Efficiency*],
    ),
    table.hline(),
    [1], [1], [SSE], [0.65], [], [], [],
    [], [1], [SAE], [0.73], [], [], [],
    [], [1], [MAE], [0.80], [2.19], [1], [1],
    table.hline(),
    [3], [1], [SSE], [0.81], [], [], [],
    [], [1], [SAE], [0.81], [], [], [],
    [], [1], [MAE], [0.83], [0.83], [2.63], [0.88],
    table.hline(),
    [3], [2], [SSE], [0.65], [], [], [],
    [], [2], [SAE], [0.65], [], [], [],
    [], [2], [MAE], [0.66], [0.66], [3.31], [0.55],
    table.hline(),
  ),
)
#figure(
  table(
    columns: 7,
    stroke: none,
    table.hline(),
    table.header(
      [*Processes*],
      [*Threads*],
      [*Function*],
      [*Time (s)*],
      [*Total
      Time (s)*],
      [*Speedup*],
      [*Efficiency*],
    ),
    table.hline(),
    [3], [4], [SSE], [0.88], [], [], [],
    [], [4], [SAE], [0.88], [], [], [],
    [], [4], [MAE], [0.88], [0.88], [2.48], [0.2],
    table.hline(),
    [3], [8], [SSE], [0.85], [], [], [],
    [], [8], [SAE], [0.85], [], [], [],
    [], [8], [MAE], [0.85], [0.85], [2.57], [0.1],
    table.hline(),
    [3], [16], [SSE], [0.95], [], [], [],
    [], [16], [SAE], [0.95], [], [], [],
    [], [16], [MAE], [0.95], [0.95], [2.3], [0.04],
    table.hline(),
  ),
)

= Code Compilation and Execution
To execute the code, we first `cd` into `src`. Then, use
#figure(
  ```sh
  mpicxx -fopenmp -O3 main.cpp search.cpp read.cpp -o main
  ```,
)
And to execute, we use

#figure(
  ```sh
  mpiexec -n 3 ./main 4 ./datos_358.txt ./params.txt
  ```,
)
These output 3 files, `sse_output.csv`,`sae_output.csv`,`mae_output.csv`, which contain the parameter estimation values for the methanol model.

To test the Rosenbrock and Michalewicz functions, use
#figure(
  ```sh
  mpicxx -fopenmp -O3 benchmark.cpp search.cpp read.cpp -o main
  ```,
)
and execute using
#figure(
  ```sh
  mpiexec -n 2 ./main 2 ./bench_params.txt
  ```,
)

= Conclusions
We successfully implemented a parallelized version of the Harmony Search and recreated the results from @argyropoulosSemiempiricalModelDirect2003 (plus or minus some random error). The observed speedup was at most 3.31 for 3 processes with 2 threads each, and the greatest efficiency was 0.88, for 3 processes with 1 thread each.

#bibliography("ref.bib")
