#import "@preview/diatypst:0.9.3": *
#import "@preview/lovelace:0.3.1": *

#show: slides.with(
  title: "Meta-heuristic algorithms", // Required
  subtitle: "Harmonic Search",
  date: "11/06/2026",
  authors: "Pedro D. Llerenas",
)

= Introduction
== Optimization
We aim to find the best solution:
- Minimal cost
- Minimal error
- Maximal profit
- Maximal utility
Gradient-based methods rely on explicit computation of expensive operators such as the Hessian (or approximations of it).

== Music-Based Algorithms
Harmony Search @HeuristicOptimization2001 (2001) has been used in problems such as water distribution networks and groundwater modeling.

The algorithm is based on the idea of a musician improvising, searching for a
_perfectly pleasing_ harmony. This harmony in music is analogous to finding
optimality in an optimization process. The heuristic which determines _perfectly
pleasing_ harmony is given by the _audio aesthetic standard_ @HarmonySearch2010.

The aesthetic quality of a musical instrument is essentially determined by its
pitch (or frequency), timbre (or sound quality), and amplitude (or loudness).

#figure(
  image("images/waveform.webp"),
  caption: [Waveform (Adobe)],
)
#figure(
  image(width: 70%, "images/timbre.png"),
  caption: [Different timbres, a human evaluates what the combination of those frequencies equals. (Zong Woo Geem)],
)

== Harmony Search
We decompose it into 3 options:
- Play any famous piece of music (a series of pitches in harmony) exactly from their memory;
- Play something similar to a known piece (this is equivalent to adjusting the pitch slightly);
- Compose new or random notes.

== Pseudocode

#figure(
  kind: "algorithm",
  supplement: [Algorithm],

  pseudocode-list(booktabs: true, numbered-title: [Harmony Search])[
    + generate initial harmonics
    + *while* t < Max Iterations
      + generate new harmonics by accepting best harmonics
      + adjust pitch to get new harmonics (solutions)
      + *if* rand > $r_"accept"$ *then*
        + choose an existing harmonic randomly
      + *else if* rand > $r_"pa"$
        + adjust the pitch randomly within a bandwidth.
      + *else*
        + generate new harmonics via randomization
      + *end*
    + *end*
  ],
) <alg:hs>

= Implementation

== Data Structures

#{
  set text(
    size: 10pt,
  )
  figure(
    ```cpp
    using Point = std::vector<double>;
    using Func = std::function<double(const Point &)>;

    struct DataPoint {
        double i;       // Current density
        double VFC_i;   // Observed voltage
    };

    struct HParams {
      int ndim;
      int HS_size;
      double HMacceptRate;
      double PArate;
      int MaxAttempt;

      std::vector<double> lower_bound;
      std::vector<double> upper_bound;
      std::vector<double> pa_range;
    }
    ```,
  )
}

== Initialization
#figure(
  ```cpp
  #pragma omp parallel for default(none) shared(hp, HM, HMBest, r, f)
    for (int i = 0; i < hp.HS_size; ++i)
    {
      for (int j = 0; j < hp.ndim; j++)
      {
        HM[i][j] = r.GetRand(hp.lower_bound[j], hp.upper_bound[j]);
      }
      HMBest[i] = f(HM[i]);
    }
  ```,
)

== Step Update
#{
  set text(
    size: 10pt,
  )
  figure(
    ```cpp
    #pragma omp for
        for (int k = 0; k < hp.MaxAttempt; k++) {
          for (int j = 0; j < hp.ndim; ++j) {
            if (r.GetRand() >= hp.HMacceptRate) {
              // randomized search
              new_x[j] = r.GetRand(hp.lower_bound[j], hp.upper_bound[j]);
            } else {
              int random_hm_index = floor(hp.HS_size * r.GetRand());
              new_x[j] = HM[random_hm_index][j];

              // harmony memory accepting rate
              if (r.GetRand() <= hp.PArate) {
                // pitch adjusting
                double pa = (hp.upper_bound[j] - hp.lower_bound[j]) / hp.pa_range[j];
                new_x[j] = new_x[j] + pa * (r.GetRand() - 0.5);
              }
            }
          }
    ```,
  )

  figure(
    ```cpp
    // evaluate the newly generated candidate solution
    double fbest = f(new_x);

    #pragma omp critical {
      int HSmaxNum = 0;
      double HSmax = HMBest[0];
      // Find the worst current harmony
      for (int i = 1; i < hp.HS_size; ++i) {
        if (HMBest[i] > HSmax) {
          HSmax = HMBest[i];
          HSmaxNum = i;
        }
      }

      if (fbest < HSmax) {
        HM[HSmaxNum] = new_x;
        HMBest[HSmaxNum] = fbest;
      }
    }
    ```,
  )
}

== Main Function

#figure(
  ```cpp
  vector<DataPoint> experiment_data = read_experimental_data(filename);

  vector<ObjectiveTask> tasks;

  tasks.push_back({"sse", [&experiment_data](const Point &p)
                   { return objective_SSE(p, experiment_data); }});
  tasks.push_back({"sae", [&experiment_data](const Point &p)
                   { return objective_SAE(p, experiment_data); }});
  tasks.push_back({"mae", [&experiment_data](const Point &p)
                   { return objective_MAE(p, experiment_data); }});
  ```,
)

#pagebreak()

We use 3 processes to compute each call via MPI.

#grid(
  columns: 1,
  rows: 2,
  figure(
    ```cpp
    Point x = HarmonicSearch(tasks[rank].func, hp);
    ```,
  ),
  figure(
    image(
      width: 50%,
      "images/processes.png",
    ),
  ),
)

= Tests
== Rosenbrock

#figure(image("images/rosenbrock.png"), caption: [$f(x,y) = (1-x)^2 + 100(y-x^2)^2$. Best objective value: $0.0005$])


== Michalewicz
#figure(
  image("images/michalewicz.png"),
  caption: [$f(x,y) = -sin(x) sin^(20) (x^2/pi) - sin(y) sin^(20) ((2y^2)/pi)$. Best objective value: $-1.8013$],
)

== Performance

#figure(
  table(
    columns: 7,
    stroke: none,
    table.hline(),
    table.header([*Processes*], [*Threads*], [*Function*], [*Time(s)*], [*Total  (s)*], [*Speedup*], [*Efficiency*]),
    table.hline(),
    [1], [1], [Rosenbrock], [0.45], [], [],[],
    [], [1], [Michalewicz], [0.51], [0.96], [1],[1],
    table.hline(),
    [2], [1], [Rosenbrock], [0.50], [],[],[],
    [], [1], [Michalewicz], [0.50], [0.5],[1.6],[0.8],
    table.hline(),
    [2], [2], [Rosenbrock], [0.85], [],[],[],
    [], [2], [Michalewicz], [0.81], [0.85],[1.18],[0.29],
    table.hline(),
    [2], [4], [Rosenbrock], [0.67], [],[],[],
    [], [4], [Michalewicz], [0.68], [0.68],[1.41],[0.17],
    table.hline(),
    [2], [8], [Rosenbrock], [0.69], [],[],[],
    [], [8], [Michalewicz], [0.68], [0.69],[1.39],[0.08],
    table.hline(),
    [2], [16], [Rosenbrock], [0.91], [],[],[],
    [], [16], [Michalewicz], [0.93], [0.93],[1.03],[0.03],
    table.hline(),
  ),
)

= Direct Methanol Fuel Cell
== Model
Model function:
$
  E_"cell" = E_o^* - b log j - R_e j + C_1 ln (1- C_2 j).
$
Objective functions to minimize:
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

#figure(
  table(
    columns: 7,
    table.hline(),
    stroke:none,
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

== Conclusions
We recreated the results in the original paper (up to some random error), and
successfully provided a parallelized implementation using OpenMP and MPI. The observed speedup was at most 3.31 for 3 processes with 2 threads each, and the greatest efficiency was 0.88, for 3 processes with 1 thread each.

== References

#bibliography("ref.bib")
