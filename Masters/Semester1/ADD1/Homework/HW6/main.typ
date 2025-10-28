#import "@preview/problemst:0.1.2": pset
#import "@preview/tyniverse:0.2.3": homework
#import "@preview/great-theorems:0.1.2": *
#import "@preview/equate:0.3.2": equate

#show: equate.with(breakable: true)
#set math.equation(numbering: "(1)")
#show: great-theorems-init

#let proof = proofblock()
#show: pset.with(
  class: "Análisis de Datos I",
  student: "Pedro D. Llerenas\npedro.llerenas@cimat.mx",
  title: "Tarea 6",
  date: datetime.today(),
)

#let question = homework.complex-question
#let Var = $upright(bold(V a r))$
#let Cov = $upright(bold(C o v))$
#let Cor = $upright(bold(C o r))$

#question[
  Pregunta de gráficas.
]

En mi opinión, la mejor es la de la NASA, ya que no solo es la más simple de
seguir, el mensaje que desea transmitir es claro: el aumento en temperatura es
significativo. Las demás me parecen más dificiles de analizar (por complejidad)
y entender qué es lo que quieren que la audiencia reciba.

#question[
  Verificar que para cualquier par de v.a. $X,Y$:
  $ Var(X+Y) = Var(X) + Var(Y) + 2Cov(X, Y). $
]

#proof[
  Recordemos que la definición de covarianza es la siguiente:
  $ Cov(X, Y) colon.eq EE[X Y] - EE[X]EE[Y]. $
  Entonces, obtenemos las siguientes igualdades:
  #align(
    center,
    $
      Var(X+Y) & = EE[(X+Y)^2] - EE[X+Y]^2 #<eq1> \
               & = EE[X^2 + 2X Y + Y^2] - (EE[X] + E[Y])^2#<eq2> \
               & = EE[X^2] + 2EE[X Y] + EE[Y^2] - (EE[X]^2 + 2EE[X]E[Y] + EE[Y]^2)#<eq3> \
               & = (EE[X^2] - EE[X]^2) + (EE[Y^2] - EE[Y]^2) + 2(EE[X Y] - EE[X]E[Y])#<eq4> \
               & = Var(X) + Var(Y) + 2Cov(X, Y).
    $,
  )
  @eq2 se sigue de la linealidad del valor esperado.
]
#set math.equation(numbering: none)

#question[
  Si $Y = a X + epsilon$, $X, epsilon$ v.a. independientes con promedio $0$ y $Var(X) = 1$: verifique que
  $ Cov(X, Y) = a, quad Cor(X, Y) = a/(sqrt(a^2 + Var(epsilon))) $
  Dibuja un conjunto de datos con $abs(Cov(X, Y))$ chico y $abs(Cor(X, Y))$ grande.
]

#proof[
  Recordemos que la definición de correlación es la siguiente:
  $ Cor(X, Y) = Cov(X, Y) / sqrt(Var(X)Var(Y)). $
  También tenemos la siguiente propiedad de la covarianza (sabiendo que es un
  producto interno, no es necesario demostrar lo siguiente):
  $ Cov(a X + c, b Y + d) = a b Cov(X, Y) #h(2em) forall a,b,c,d in RR. $
  Además, si $V,W,Z$ son v.a., tenemos
  $
    Cov(Z, V + W) & = EE[Z(V+W)] - EE[Z]EE[V+W] \
                  & = EE[Z V] + EE[Z W] - EE[Z]EE[V] - EE[Z]EE[W] \
                  & = Cov(Z, V) + Cov(Z, W).
  $
  Entonces, con $Y = a X + epsilon$ y $Var(X) = 1$, tenemos
  $
    Cov(X, a X + epsilon) & = Cov(X, a X) + Cov(X, epsilon) \
                          & = a Cov(X, X) + 0 \
                          & = a Var(X) = a.
  $
  Entonces, tenemos que
  $
    Cor(X, Y) & = Cor(X, a X + epsilon) \
              & = Cov(X, a X + epsilon) / sqrt(Var(X)Var(a X + epsilon)) \
              & = a / sqrt(1 dot (a^2 Var(X) + Var(epsilon))) \
              & = a / sqrt(a^2 dot 1 + Var(epsilon)) \
              & = a / sqrt(a^2 + Var(epsilon)).
  $
]

#question[
  Si $X = Z + V$, $Y = Z + W$, $Z, V, W$ v.a. independientes con promedio 0 y $V,W$ varianza 1:
  $ Cor(X, Y) = Var(Z)/(Var(Z) + 1). $
]

#proof[
  Como observamos en el ejericio anterior, y usando que son independientes, observamos que
  $
    Cov(Z + V, Z + W) & = Cov(Z, Z) + Cov(Z, W) + Cov(V, Z) + Cov(V, W) \
                      & = Var(Z) + 0 + 0 + 0 \
                      & = Var(Z).
  $
  por lo que tenemos, recordando que $Var(W) = Var(V) = 1$,
  $
    Cor(X, Y) & = Cor(Z+V, Z+W) \
              & = Cov(Z+V, Z+W) / sqrt(Var(Z+V)Var(Z+W)) \
              & = Var(Z) / sqrt((Var(Z) + Var(V))(Var(Z) + Var(W))) \
              & = Var(Z) / sqrt((Var(Z) + 1)(Var(Z) + 1)) \
              & = Var(Z) / (Var(Z) + 1).
  $
]

#set enum(numbering: "a)")
#question[
  + Una manera para medir dependencia entre dos v.a. $X$ y $Y$ es _medir_ la
    distancia entre la distribución conjunta y el producto de las marginales
    (que corresponde a la distribución bajo independencia). Aunque no cimple
    con todas las propiedades que debe cumplir una de distancia, se una mucho
    la divergencia de Kullback Leibler (KL). Para el caso de dos distrubiciones
    discretas, se define como: $ K L(P|Q) = - sum_x p(x) log q(x)/p(x) = -EE_p
    [log q(X)/p(X)] $
    Observa que $K L (P|P) = 0$. Verifica que $K L(P|Q) >= 0$.
  + Verifica que $I(X,Y) = K L(P(X,Y) | P(X)P(Y))$.
]

#proof(of: "a")[
  Por definición,
  $
    K L(P|P) & = -EE_p [log p(X)/p(X)] \
             & = - EE_p [log 1] \
             & = - EE_p [0] \
             & = 0.
  $
  Por otro lado, por definición y la desigualdad de Jensen (sabiendo que $log$ es convexa),
  $
    K L(P|Q) & = -sum_x p(x) log q(x)/p(x) \
             & >= -log sum_x p(x) q(x)/p(x)#h(2em) ("Jensen") \
             & = -log sum_x q(x) \
             & = -log 1 \
             & = 0.
  $
]

#proof(of: "b")[
  Por definición y ley de probabilidad total,
  $
    I(X,Y) & = H(Y) - H_X (Y) \
           & = sum_x sum_y P(X=x) P(Y=y|X=x) log P(Y=y|X=x)-sum_y P(Y = y) log P(Y=y) \
           & = sum_x sum_y P(X=x,Y=y)log P(Y=y|X=x)-sum_y sum_x P(X=x,Y=y) log P(Y=y) \
           & = sum_(x,y) P(X=x,Y=y) log P(Y=y|X=x) - P(X = x, Y=y)log P(Y=y) \
           & = sum_(x,y) P(X=x,Y=y) log P(Y=y|X=x)/P(Y=y) \
           & = sum_(x,y) P(X=x,Y=y) log P(X=x,Y=y)/(P(X=x)P(Y=y)) \
           & = -sum_(x,y) P(X=x,Y=y) log (P(X=x)P(Y=y))/P(X=x,Y=y) \
           & = K L (P(X,Y) | P(X)P(Y)). \
  $
  Con esto, es claro que
  $
    I(X,Y) = K L(P(X,Y) | P(X)P(Y)) = K L (P(Y,X) | P(Y) P(X)) = I(Y,X).
  $
]

#question[
  Un robot viaja de posición $p_1$ a $p_2$, de $p_2$ a $p_3$, y de $p_3$ a
  $p_4$. El tiempo para viajar de $p_i$ a $p_(i+1)$ es $cal(N) (i,i)$ horas.
  Calcula la probabilidad que el viaje dure más de 6 horas si sabes que tardó
  menos de 1 hora para viajar de $p_1$ a $p_2$.
]

Sea $T_i$ el timepo que se tarda de $p_i$ a $p_(i+1)$, entonces el timepo total
$T = T_1 + T_2 + T_3$. Como son variables independientes, su suma es nuevamente
una distribución normal, con $T~cal(N)(1+2+3, 1+2+3) = cal(N)(6,6)$. Como
sabemos que $T_1 < 1$, nuestro problema se reduce a calcular la probabilidad
condicional $P(T > 6 | T_1 <1)$. Ahora, notemos que como $T_2 + T_3$ es independiente de $T_1$,
por lo que podemos reemplazarlo de la siguiente manera:
$
  P(T_2 + T_3 > 6 - T_1 | T_1 < 1) = P(T_2 + T_3 > 6 - 1) = P(T_2 + T_3 > 5).
$
Ahora, como $T_2+T_3 ~ cal(N)(5,5)$, la noramalizamos:
$
  Z = (T_2 + T_3 -5)/sqrt(5) ~ cal(N)(0,1).
$
Ahora, tenemos
$
  P(T_2 + T_3 > 5) & = P(sqrt(5)Z + 5 > 5) \
                   & = P(Z > 0) \
                   & = 0.5.
$

#question[
  En este ejercicio simulamos muestras de distribución multivariadas Gaussianas
  y buscamos visualizaciones informativas. Las tienes que generar a partir de
  muestras de estándar normales usando la propiedad que si
  $
    X = AA Y + mu " con " Y = (Y_1,dots, Y_d) " y " {Y_i} ~ cal(N)(0,1) " independientes "
  $
  entonces $X~cal(N)(mu,AA AA^T)$.

  Simula muestras de $X = (X_1, dots, X_4) ~ cal(N)(mu, CC)$ y busca
  visualizaciones informativas que vimos en clase para diferentes elecciones de
  $CC$.
]

#figure(
  image("p1.png", width: 80%),
  caption:[Pair plot de las 4 variables]
)

#figure(
  image("p2.png", width: 80%),
  caption:[Ejes horizontales, no se distingue nada (normalmente distribuida)]
)

#figure(
  image("p3.png", width: 100%),
  caption:[En 3d, con y4 como el color (ver plt.html)]
)

#figure(
  image("p4.png", width: 100%),
  caption:[En 3d, con y4 como el color (ver plt.html)]
)

#figure(
  image("p5.png", width: 100%),
  caption:[En 3d, con y4 como el color (ver plt.html)]
)
