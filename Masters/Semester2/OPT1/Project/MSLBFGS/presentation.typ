#import "@preview/touying:0.7.3": *
#import "@preview/gh-minimal-slides:0.1.0" as gh

#show: gh.register.with(
  theme: "light", // "light" | "dark"
  accent: "orange", // "blue" | "green" | "purple" | "pink" | "orange" | "mono"
  density: "comfy", // "comfy" | "compact"
)

#gh.cover-slide(
  kicker: "Optimization I - CIMAT",
  title: [Multi-Secant Limited-Memory BFGS],
  badges: (),
  footer-left: "Gerardo Rocha, Pedro Llerenas · 2026",
  footer-right: "",
)

#gh.content-slide(title: [Objective])[
  Understand and implement the Multi-Secant Limited-Memory BFGS method and
  perform tests to prove its efficiency over a simple BFGS method and Newton methods.
]

#gh.section-slide(
  number: "01",
  kicker: "Chapter",
  title: [Previous Algorithms],
)

#gh.content-slide(title: [Newton Method])[
  Usage of the step:
  $
    p_k^N = -nabla^2 f_k^(-1) nabla f_k.
  $
  Problem: computing $nabla^2 f_k^(-1)$ is expensive.
  Suggestion: Approximate it.
]

#gh.content-slide(title: [Quasi-Newton: Broyden-Fletcher-Goldfarb-Shanno Algorithm])[
  Usage of the step:
  $
    p_k = -B_k^(-1) nabla f_k.
  $
  for a symmetric positive definite approximation $B_k$.
  Problem: $O(n^2)$ storage.
  Suggestion: $$
]

#gh.section-slide(
  number: "02",
  kicker: "Chapter",
  title: [Limited-Memory BFGS],
)

#gh.content-slide(title: [L-BFGS])[

]

#gh.section-slide(
  number: "03",
  kicker: "Chapter",
  title: [Multi-Secant Limited-Memory BFGS],
)

#gh.content-slide(title: [MS-LBFGS])[
  // insert introduction to limited memory multi-secant BFGS
]

#gh.content-slide(title: [Main algorithm])[
  ```julia

  ```
]

#gh.table-slide(
  title: [Endpoints],
  headers: ("Method", "Path", "Status", "Description"),
  rows: (
    ("GET", "/api/users", "200", "Returns user list"),
    ("POST", "/api/users", "201", "Creates new user"),
    ("PATCH", "/api/users/:id", "200", "Updates existing user"),
    ("DELETE", "/api/users/:id", "204", "Removes a user"),
  ),
)

#gh.two-col-slide(
  title: [Before & after],
  left: (
    "Before",
    "danger",
    "12m 04s",
    [Sequential builds took upwards of 12 minutes on every push.
      Developers context-switched out of flow.],
  ),
  right: (
    "After",
    "success",
    "1m 47s",
    [Parallelised pipeline with shared caches. Builds land before
      the PR description is written.],
  ),
)

#gh.image-slide(
  title: [Figure 1 — system overview],
  placeholder-text: "[ architecture diagram — 1680 × 520 ]",
  caption: [Figure 1. Dataflow through the ingest → transform → serve pipeline. Source: #gh.gh-link[docs/architecture.md]],
)

#gh.stats-slide(
  title: [Q1 metrics],
  stats: (
    ("1.4M", "weekly downloads", "12%"),
    ("4,208", "github stars", "340"),
    ("97", "contributors", "6"),
    ("98.2%", "test coverage", "0.4"),
  ),
)

#gh.task-slide(
  title: [Task list — v1 milestone],
  tasks: (
    (true, [Draft initial API spec], "closed · 12 days ago"),
    (true, [Wire up authentication middleware], "closed · 8 days ago"),
    (true, [Add pagination to list endpoints], "closed · 3 days ago"),
    (false, [Rate-limiting for public routes], "open · 2 reviewers"),
    (false, [Migrate to streaming responses], "open · blocked"),
    (false, [Publish v1.0 to registry], "open · milestone"),
  ),
)

#gh.alert-slide(
  title: [Alert callouts],
  alerts: (
    ("NOTE", "accent", [Highlights information that users should take into account, even when skimming.]),
    ("TIP", "success", [Optional guidance that helps a user get more out of a feature.]),
    ("WARNING", "warning", [Urgent info that needs immediate user attention to avoid problems.]),
    ("CAUTION", "danger", [Advises about risks or negative outcomes of certain actions.]),
  ),
)

// ------------------------------------------------------------
// 13 — Closing
// ------------------------------------------------------------
#gh.closing-slide(
  kicker: "## Thanks",
  title: [Questions?],
  links: ("docs/", "issues/", "contact.md"),
)
