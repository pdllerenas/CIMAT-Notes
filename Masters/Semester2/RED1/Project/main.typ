#import "@preview/touying:0.7.3": *
#import themes.university: *
#import "@preview/cetz:0.5.0"
#import "@preview/fletcher:0.5.8" as fletcher: edge, node
#import "@preview/numbly:0.1.0": numbly
#import "@preview/theorion:0.6.0": *
#import cosmos.clouds: *
#show: show-theorion

// cetz and fletcher bindings for touying
#let cetz-canvas = touying-reducer.with(reduce: cetz.canvas, cover: cetz.draw.hide.with(bounds: true))
#let fletcher-diagram = touying-reducer.with(reduce: fletcher.diagram, cover: fletcher.hide)

#show: university-theme.with(
  aspect-ratio: "16-9",
  align: horizon,
  // config-common(handout: true),
  config-common(frozen-counters: (theorem-counter,)), // freeze theorem counter for animation
  config-info(
    title: [One-Way Delay Estimation Over the Internet],
    subtitle: [],
    author: [Pedro D. Llerenas],
    date: datetime.today(),
    institution: [CIMAT],
  ),
)

#let OWD = "OWD"

#set heading(numbering: numbly("{1}.", default: "1.1"))

#title-slide()

== Outline <touying:hidden>

#components.adaptive-columns(outline(title: none, indent: 1em))

= Introduction

== One-Way Delay

_One-Way Delay (OWD)_ refers to the time it takes for a packet to be transmitted across a network from source to destination.

#pause

#figure(
  ```
    PING google.com (192.178.56.78) 56(84) bytes of data.
  64 bytes from pnqroa-ac-in-f14.1e100.net (192.178.56.78): icmp_seq=1 ttl=116 time=16.6 ms
  ```,
)

#pause

The ping command returns the _Round Trip Time (RTT)_, which is the time it takes
for a packet to travel back and forth.

#pause

OWD is usually approximated as $"RRT"/2$ (many assumptions made).

#speaker-note[
  + This is a speaker note.
  + You won't see it unless you use `config-common(show-notes-on-second-screen: right)`
]


= Derivation

== One-Way Transit Time

- _system clock_: $C(t)$,
- _absolute offset_: offset from a "true" clock, $C_A(t) - t$
- _relative offset_: offset from another clock, $theta(t) = C_A(t) - C_B(t)$,
- _frequency_: rate of progression, $C'(t) = (d C(t))/(d t)$,
- _clock ratio_: frequency ratio $alpha = C_A(t) / C_B(t)$,
- _resolution_: minimum time unit of update.

#pause

The relationship between two clocks is then:
$
  C_A(t) = theta(t) + alpha C_B(t).
$

= Clock Information Sharing

== Clock Information Sharing<touying:hidden>

#grid(
  columns: (2fr, 1fr),
  fletcher-diagram(
    node-stroke: .1em,
    node-fill: gradient.radial(blue.lighten(80%), blue, center: (30%, 20%), radius: 80%),
    spacing: 4em,
    node((0, 0), `A`, radius: 2em),
    edge((0, 0), (1, 0), $T_A^1(i)$, "--|>", bend: 45deg),
    node((1, 0), `B`, radius: 2em),
  ),
  [
    $T_A^1(i)$ --- message $i$ with timestamp.
  ],
)

== Clock Information Sharing<touying:hidden>

#grid(
  columns: (2fr, 1fr),
  fletcher-diagram(
    node-stroke: .1em,
    node-fill: gradient.radial(blue.lighten(80%), blue, center: (30%, 20%), radius: 80%),
    spacing: 4em,
    node((0, 0), `A`, radius: 2em),
    edge((1, 0), (1, 0), $T_B^2(i)$, "<|--", bend: 125deg),
    node((1, 0), `B`, radius: 2em),
  ),
  [
    $T_B^2(i)$ --- Add a timestamp of reception
  ],
)

== Clock Information Sharing<touying:hidden>

#grid(
  columns: (2fr, 1fr),
  fletcher-diagram(
    node-stroke: .1em,
    node-fill: gradient.radial(blue.lighten(80%), blue, center: (30%, 20%), radius: 80%),
    spacing: 4em,
    node((0, 0), `A`, radius: 2em),
    edge((0, 0), (1, 0), $T_B^3(i)$, "<|--", bend: 45deg),
    node((1, 0), `B`, radius: 2em),
  ),
  [
    $T_B^3(i)$ --- Add a timestamp when sent
  ],
)

== Clock Information Sharing<touying:hidden>

#grid(
  columns: (2fr, 1fr),
  fletcher-diagram(
    node-stroke: .1em,
    node-fill: gradient.radial(blue.lighten(80%), blue, center: (30%, 20%), radius: 80%),
    spacing: 4em,
    node((0, 0), `A`, radius: 2em),
    edge((0, 0), (0, 0), $T_B^4(i)$, "<|--", bend: 125deg),
    node((1, 0), `B`, radius: 2em),
  ),
  [
    $T_A^4(i)$ --- Add a timestamp when received
  ],
)

== Clock Information Sharing<touying:hidden>


#figure(
  image("OWD.png"),
)

== Measurements

$
       "RTT"(i) & = T_A^4(i) - T_A^1(i), \
  pause
  OWD_(A->B)(i) & = T_B^2(i) - T_A^4(i), \
  pause
  OWD_(B->A)(i) & = T_A^4(i) - T_B^3(i), \
          pause
          OWD   & = T_"recv" - T_"sent",
$

== Sampling

For $n$ samples, each $OWD_i$, $1 <= i <= n$ is represented by
$
  OWD_i = theta + xi + delta_i + epsilon_i,
$
#pause
- $theta$: offset between clocks,
#pause
- $xi$: network propagation plus transmission delay,
#pause
- $delta$: queuing delay observed by packets,
#pause
- $epsilon$: random error with mean 0.
#pause

== Sampling

$
  OWD_i = T_"recv" - alpha T_"sent",
$
#pause
$alpha approx 1$ for short time probes and small packet sizes.

= Previous models

== Previous models
$
  hat(theta)_(A <-> B) = min_(i | 0 < i <= k) (min_j (OWD_(A -> B_k) (j) - OWD_(B_k -> A)(j))/ 2)
$
#pause
$
  hat(theta)_(A<-> B) = (min (OWD_(A->B_k) - OWD_(B_k->A)))/2
$
#pause
$
  sum_(i = 1)^n nu_i (X_((i)) - Y_((i)))
$

== Previous models
$
  hat(theta)_(A<->B) = 1/2{max_(1<=i<=n) {S_i - alpha U_i} + min_(1 <= i <= n){T_i - alpha V_i}}
$

= Proposed model
== Clock offset estimation
- Independent one-way offset estimation
- Collaborative offset estimation
#pause
The independent one-way offset will serve as a model for the OWD, and then
estimate the long-run minimum delay (when $delta_i$ = 0).
#pause

In the collaborative process, nodes share they approximation to calculate a
refined offset approximation taking asymmetries into account.
#pause

In general, the distribution of $delta$ cannot be succinctly characterized. It is affected by
the network topology and its possible rearrangements.
#pause
Over a fixed path, it can be _shown_ that it follows a gamma distribution.

== Gamma Distribution

#figure(
  image("Gammapdf252.png"),
)

== Experiment settings
#figure(
  image(
    "QueueDelay.png",
  ),
)

== Experiment settings
A week of experimentation for each link, 48-bytes UDP with ${10, 20, 30, 40, 80,
  160, 500, 1000} m s$ inter-packet separations.

#pause

Every 2 hours,
- 1/2 hr. of packets being sent from $A$ to $B$,
- 1/2 hr. of packets being sent from $B$ to $A$.

#pause

A preprocess is done to remove offset and skewness.

== Resulting Distribution
A maximum likelihood estimate yielded
$
  f(chi, rho, beta, gamma) = (((chi - gamma)/(beta))^(rho-1)e^(-(chi - gamma)/beta))/(beta Gamma(rho)), quad (rho > 0, beta >0; chi > gamma)
$

#figure(
  image(
    "Dist.png",
  ),
)

== Scaling considerations
Instead of incrementing the number of links (which was seen to have a similar
distribution), only the traffic condition was considered.

== One-Way Offset Estimation
- Use a predefined gamma distribution
- Assume that the queuing delays ${delta_i}_(i = 1)^n$ are i.i.d.
- $A$ sends $n$ 48-byte UDP packets at a rate of $1/d$ with timestamp info from
  sender.
- $A$ prepares to receive $n$ packets.
- $B$ sends back the packets at the same rate $1/d$.
- $B$ estimates the parameters of the gamma distribution.

#figure(
  image("offset.png"),
)

== Gamma parameter estimation
$
   rho & = (overline(chi)/s)^2, \
  beta & = s^2/overline(chi)
$
Let $x(i) = T_"recv"(i) - T_"send"(i)$, $tilde(x)^((j)) = "sort"_j(x(i))$,
$y^((i))$ the theoretical ordered random values of the gamma distribution.
#pause
Now, we calculate either
$
  gamma = "offset" + "propagation delay" + "transmission delay"
$
or eliminate $delta$.

== Estimate
$
  hat(theta) = gamma = theta + xi
$
$
  hat(OWD)_i = tilde(x)^i - hat(theta)
$

#figure(
  image("lr.png"),
)

== Collaborative offset estimation
We average the end-node estimates of the offset:
$
  hat(theta)_(A<->B) & = (hat(theta)_(A->B)- hat(theta)_(A<-B))/2 \
                     & = ([theta + xi_(A->B) - (-theta + xi_(A<-B))])/2 \
                     & = theta + (xi_(A->B) - xi_(A<-B))/2
$
#pause
Estimator is unbiased when $xi_(A->B) approx xi_(A<-B)$.

== Inter-Packet Separation

Since we need the packets to be independent, we want the rate $d$ to be such that
no two packets are in a router at the same time. It is recommended to have $d in (10,30)$ ms.

= Results
== Performance

#figure(
  image("f11.png"),
)

== Conclusion <touying:hidden>
_The proposed one way estimation of the queue delay is good
enough to be used in robust multimedia communications protocols
for ahead of time reaction before and during transmission._

== References <touying:hidden>
*E. Mota-García, R. Hasimoto-Beltran (2013)*, A new model-based clock-offset approximation over IP networks.