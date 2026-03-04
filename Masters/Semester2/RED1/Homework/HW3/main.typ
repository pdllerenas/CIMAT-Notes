#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Programación de Redes 1",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 3",
  date: datetime(year: 2026, month: 3, day: 4),
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

#question[
  Give two example computer applications for which connection-oriented service is appropriate. Now give two examples for which connectionless service is best.
]
Connection-oriented service is better suited when we want reliability, where the links are unreliable, such as satellite channel or long-distance telephone circuits. Therefore, applications such as Whatsapp or a bank application need a reliable connection. On the other hand, connectionless services are suited best for fast end-to-end information sharing, without worrying about reliability. For example, Twitch livestreaming or Zoom conference calls.

#question[
  A router can process 2 million packets/sec. The load offered to it is 1.5 million packets/sec on average. If a route from source to destination contains 10 routers, how much time is spent being queued and serviced by the router?
]
According to @Tanenbaum2011ComputerN5, we may calculate it via
$
  T = 1/mu times 1/(1-rho)  
$
where $rho = lambda / mu$ is the CPU utilization, where $lambda$ is the random rate of packet arrival, with  Poisson distribution, and $1/mu$ is the service with absence of competition. In this example, $mu = 2,000,000 "packets/sec"$ and $lambda = 1,500,000 "packets/sec"$. Thus, we have $rho = #{1500000/2000000}$.
$
  T = 1/(2000000) times 1/(1-0.75)   = 4/(2000000) = #{4/2000000} "s" = #{4*1000/2000000} "ms" = 2 mu s
$
Since this is for each router, in total it is $#{4*1000*1000*10/2000000} mu$s  of delay.



#question[
  An IP datagram using the strict source routing option has to be fragmented. Do
  you think the option is copied into each fragment, or is it sufficient to just
  put it in the first fragment? Explain your answer.
]

It has to be copied into each fragment. Otherwise, the fragments other than the first one will follow arbitrary paths to its destination.

#question[
  Convert the IP address whose hexadecimal representation is C22F1582 to dotted decimal notation.
]

From right to left:
$
  2 & -> 0010 \
  8 & -> 1000 \
  5 & -> 0101 \
  1 & -> 0001 \
  F & -> 1111 \
  2 & -> 0010 \
  2 & -> 0010 \
  C & -> 1100 \
$
Therefore, the dotted decimal notation is
$
  11000010.00101111.00010101.10000010 = 194.47.21.130
$

#question[
  Use the traceroute (UNIX) or tracert (Windows) programs to trace the route from
  your computer to various universities on other continents. Make a list of transoceanic
  links you have discovered. Some sites to try are

  - www.berkeley.edu (California)
  - www.mit.edu (Massachusetts)
  - www.vu.nl (Amsterdam)
  - www.ucl.ac.uk (London)
  - www.usyd.edu.au (Sydney)
  - www.u-tokyo.ac.jp (Tokyo)
  - www.uct.ac.za (Cape Town)
]

We will only explain one example. We take www.berkeley.edu as the example.

```
  1   172.24.144.1  0.184ms  0.130ms  0.113ms
  2   192.168.100.1  11.408ms  9.904ms  8.885ms
  3   10.21.160.1  21.967ms  9.620ms  8.899ms
  4   10.180.25.25  16.080ms  10.234ms  21.120ms
  5   10.180.25.27  16.431ms  10.426ms  18.007ms
  6   10.180.25.57  11.797ms  10.180.25.27  17.685ms  9.436ms
  7   64.76.240.89  17.190ms  23.730ms  19.249ms
  8   200.189.212.26  44.436ms  24.295ms  29.280ms
  9   64.76.27.198  30.798ms  200.189.212.26  42.294ms  51.139ms
  10   23.209.169.22  21.636ms  64.76.27.198  28.716ms  29.845ms
  11   *  *  *
  12   23.207.239.35  78.532ms  76.943ms  78.703ms
  13   23.203.157.31  81.217ms  23.207.239.37  74.818ms  78.078ms
  14   *  23.203.157.29  80.064ms  88.484ms
  15   *  *  *
  16   *  *  *
  17   *  *  *
  18   *  *  *
  19   *  *  *
  20   23.218.151.233  76.034ms  79.326ms  76.471ms
  21   23.215.28.134  77.651ms  78.882ms  77.178ms
  22   172.238.203.58  81.002ms  76.963ms  78.383ms
```

The index represents the TTL of the packet sent to www.berkeley.edu. Therefore,
it takes 22 routers to arrive to berkeley, which corresponds to the last ip
address. The first one corresponds to our own router. The \* signify no
response,
 and three continuous ones usually mean that the router receiving the
packet
 does not allow responses to the type of packet sent. Every other router
responding will essentially say that the TLL was exceeded. We may view this using `tcpdump` and `wireshark`:
#let data = csv("ws.csv").slice(1,10)
#figure(
  table(
    columns: 7,
    table.header([*No.*], [*Time*], [*Source*], [*Destination*], [*Protocol*], [*Length*], [*Info*]),
    ..data.flatten()
  )
  , caption: [Snippet of network traffic recorded in my router.]
)

#question[
  Both UDP and TCP use port numbers to identify the destination entity when delivering a message. Give two reasons why these protocols invented a new abstract ID (port numbers), instead of using process IDs, which already existed when these protocols were designed.
]

Multiple processes may live inside the same computer. Moreover, these process ID's may overlap with those of another computer. A process may also expose various ports to receive communication from various sources.


#question[
  What is a congestion window in TCP?
]
It is a mechanism which regulates the amount of data a sender may transmit into the network before receiving an acknowledgment.

#bibliography("ref.bib")