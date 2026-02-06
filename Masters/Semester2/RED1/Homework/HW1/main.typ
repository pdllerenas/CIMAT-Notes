#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/rich-counters:0.2.1": *
#let question = homework.complex-question

#show: pset.with(
  class: "Optimization I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 1",
  date: datetime(year: 2026, month: 2, day: 6),
)
#show: great-theorems-init
#let proof = proofblock()
#let mathcounter = rich-counter(identifier: "mathblocks", inherited_levels: 1)
#let theorem = mathblock(
  blocktitle: "Theorem",
  counter: mathcounter,
)

#show link: set text(fill: blue)
#show link: underline

#set math.equation(
  numbering: "(1)",
  supplement: none,
)

#set par(
  justify: true,
  leading: 0.52em,
)

#show ref: it => {
  // provide custom reference for equations
  if it.element != none and it.element.func() == math.equation {
    // optional: wrap inside link, so whole label is linked
    link(it.target)[(#it)]
  } else {
    it
  }
}

#set enum(
  full: true, // necessary to receive all numbers at once, so we can know which level we are at
  numbering: (..nums) => {
    let nums = nums.pos() // just positional args
    let num = nums.last() // just the current level’s number
    let level = nums.len() // level is the amount of numbers available

    // format for current level (or stop at i. If going too deep)
    let format = ("a)", "i)", "i)").at(calc.min(2, level - 1))
    let result = numbering(format, num) // formatted number
    if level < 3 {
      // first 2 levels for example
      strong(result)
    } else {
      result
    }
  },
)


#question[
  The performance of a client-server system is strongly influenced by two major
  network characteristics: the bandwidth of the network (that is, how many
  bits/sec it can transport) and the latency (that is, how many seconds it
  takes for the first bit to get from the client to the server). Give an
  example of a network that exhibits high bandwidth but also high latency. Then
  give an example of one that has both low bandwidth and low latency.
]

- A high bandwidth, high latency network example is a WAN. A concrete example
  can be AWS Direct Connect, which is a direct connection over Ethernet
  fiber-optic cable to some (possibly far) location. fiber-optic cable allows for
  a high stream of data at once, but the distance traveled may be of the order of kilometers.
- A low bandwidth, low latency network example is device I/O, such as a mouse
  or a keyboard, where the data transmitted to the computer is of the order of
  bytes per second, and the distance traveled is just a few meters.

#question[
  Besides bandwidth and latency, what other parameter is needed to give a good
  characterization of the quality of service offered by a network used for
  1. digitized voice traffic?
  2. video traffic?
  3. financial transaction traffic?
]

1. End-to-end encryption, so that any intercepted calls cannot be easily spied
  on.
2. Buffers and caches, so that if the receiver loses the connection for a brief
  moment, some data is still present, and can wait for the next stream of
  bytes to arrive.
3. Encryption and reliability, to ensure no private information is stolen, and
  that it is guaranteed that all parties involved in a transaction coincide on
  the outcome of the service.

#question[
  A factor in the delay of a store-and-forward packet-switching system is how
  long it takes to store and forward a packet through a switch. If switching
  time is 10 $mu$sec, is this likely to be a major factor in the response of a
  client-server system where the client is in New York and the server is in
  California? Assume the propagation speed in copper and fiber to be 2/3 the
  speed of light in vacuum.
]

The distance between New York and California is around $4,000 k m = 4 times 10^6 m$. The
propagation speed is $2/3 c approx 2 times 10^8 m/s$. Therefore, the
propagation delay solely based on the distance and medium of transportation
is given by
$
  (4 times 10^6 m)/(2 times 10^8 m/s) = 2 times 10^(-2) s = 20 times 10^(-3) s = 20 m s.
$
In comparison, switching delay is 10 $mu$s = $10 times 10^(-6)s = 10^(-5) s =
0.01 m s$. This is a factor of 2000, which means switching delays will be
negligible.

#question[
  A client-server system uses a satellite network, with the satellite at a
  height of 40,000km. What is the best-case delay in response to a request?
]
In the best-case scenario, the request travels from the client to the
satellite, and from the satellite to the server, without any intermediate
delays. Assuming no response delay once the request was received by the server,
the server sends a response which goes to the satellite, and back to the
client. Therefore, assuming the average distance traveled by each signal sent is 40,000
km, and that the propagation of the signals is the speed of light, it would take
4 times the total distance traveled ($4times 4 times 10^7m$) over the speed of light:
$
  t = (1.6 times 10^8m)/(3 times 10^8 m/s) approx 0.5 s.
$

#question[
  Five routers are to be connected in a point-to-point subnet. Between each pair
  of routers, the designers may put a high-speed line, a medium-speed line, a
  low-speed line, or no line. If it takes 100 ms of computer time to generate and
  inspect each topology, how long will it take to inspect all of them?
]

#question[
  What are two reasons for using layered protocols? What is one possible disadvantage of using layered protocols?
]

#question[
  Two networks each provide reliable connection-oriented service. One of them
  offers a reliable byte stream and the other offers a reliable message stream.
  Are these identical? If so, why is the distinction made? If not, give an
  example of how they differ.
]

#question[
  What does "negotiation" mean when discussing network protocols? Give an example.
]

#question[
  In some networks, the data link layer handles transmission errors by
  requesting that damaged frames be retransmitted. If the probability of a
  frame's being damaged is p, what is the mean number of transmissions required
  to send a frame? Assume that acknowledgements are never lost.
]

#question[
  A system has an $n$-layer protocol hierarchy. Applications generate messages of length $M$  bytes. At each of the layers, an h-byte header is added. What fraction of the network bandwidth is filled with headers?
]

#question[
  What is the main difference between TCP and UDP?
]

#question[
  An image is 1600 × 1200 pixels with 3 bytes/pixel. Assume the image is
  uncompressed. How long does it take to transmit it over a 56-kbps modem
  channel? Over a 1-Mbps cable modem? Over a 10-Mbps Ethernet? Over 100-Mbps
  Ethernet? Over gigabit Ethernet?
]
