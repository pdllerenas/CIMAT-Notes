#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#let question = homework.complex-question

#show: pset.with(
  class: "Programación de Redes 1",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Homework 1",
  date: datetime(year: 2026, month: 2, day: 6),
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


#question[
  A router can process 2 million packets/sec. The load offered to it is 1.5 million packets/sec on average. If a route from source to destination contains 10 routers, how much time is spent being queued and serviced by the router?
]

#question[
  An IP datagram using the Strict source routing option has to be fragmented. Do you think the option is copied into each fragment, or is it sufficient to just put it in the first fragment? Explain your answer.
]

#question[
  Convert the IP address whose hexadecimal representation is C22F1582 to dotted decimal notation.
]

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

#question[
  Both UDP and TCP use port numbers to identify the destination entity when delivering a message. Give two reasons why these protocols invented a new abstract ID (port numbers), instead of using process IDs, which already existed when these protocols were designed.
]

#question[
  What is a congestion window in TCP?
]