#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/fletcher:0.5.8" as fletcher: diagram, edge, node
#let question = homework.complex-question

#show: pset.with(
  class: "Programación y Algoritmos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 7",
  date: datetime.today(),
)

Consideremos la siguiente máquina de estado finito.

#align(center, rect(inset: -5pt, outset: 10pt, diagram(
  node-stroke: 1pt,
  node((0, 0.5)),
  edge("-|>", label-pos: -50%, label-side: center, label-anchor: "north", label-size: 8pt, [Input]),
  node((0, 0), $q_0$, name: <q0>),
  edge(<q0>, "-", <q2>,label-size: 8pt, bend: -45deg, [D]),
  edge("-", label-size: 8pt, $+ slash -$),
  node((1, 0), $q_1$, name: <q1>),
  edge("-", label-size: 8pt, [D]),
  node((2, 0), $q_2$, name: <q2>),
  edge(<q2>, "-|>", <q2>, bend: 130deg, loop-angle: 90deg, label-size: 8pt, [D]),
  edge(<q2>, "-|>", <q10>, label-side: left, label-size: 8pt, [Integer]),
  node((2, 1), $q_10$, shape: circle, name: <q10>, extrude: (-1, -3)),
  edge(<q2>, "-", <q3>, [.]),
  node((3, 0), $q_3$, name: <q3>),
  edge(<q3>, "-", <q4>, label-size: 8pt, [D]),
  node((4, 0), $q_4$, name: <q4>),
  edge(<q4>, "-|>", <q4>, label-size: 8pt, bend: 130deg, loop-angle: 90deg, [D]),
  edge(<q4>, "-", <q5>, label-size: 8pt, [E/e]),
  node((5, 0), $q_5$, name: <q5>),
  edge(<q5>, "-|>", <q2>, label-size: 8pt, bend: -65deg, [E/e]),
  edge(<q5>, "-", <q6>, label-size: 8pt, $+ slash -$),
  edge(<q5>, "-", <q9>, label-size: 8pt, [Floating point]),
  node((5, 1), $q_9$, shape: circle, name: <q9>, extrude: (-1, -3)),
  node((6, 0), $q_6$, name: <q6>),
  edge(<q6>, "-", <q7>, label-size: 8pt, [D]),
  node((7, 0), $q_7$, name: <q7>),
  edge(<q7>, "-|>", <q7>, label-size: 8pt, bend: 130deg, loop-angle: 90deg, [D]),
  edge(<q7>, "-", label-size: 8pt, <q5>, bend: 45deg, loop-angle: 90deg, [D]),
  edge(<q7>, "-", label-size: 8pt, <q8>, label-side: left, [Scientific Notation]),
  node((7, 1), $q_8$, shape: circle, name: <q8>, extrude: (-1, -3)),
)))
/*
states
s0
s1
s2
s3
s4
s5
s6
s7
s8
s9
s10
initial
s0
accepting
s8
s9
s10
alphabet
d
.
e
+
transitions
s0:+>s1
s1:d>s2
s2:d>s2
s2:.>s3
s1:$>s10
s3:d>s4
s4:d>s4
s4:$>s9
s4:e>s5
s5:e>s2
s5:+>s6
s5:d>s7
s6:d>s7
s7:d>s7
s7:$>s8
*/
