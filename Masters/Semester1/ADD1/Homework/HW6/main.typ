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
    Cov(Z + V, Z + W) & = Cov(Z, Z) + Cov(Z, W) + Cov(V, Z) + Cov(V, W)\
    &= Var(Z) + 0 + 0 + 0\
    &= Var(Z).
  $
  por lo que tenemos, recordando que $Var(W) = Var(V) = 1$, 
  $
  Cor(X,Y) &= Cor(Z+V, Z+W)\
  &= Cov(Z+V, Z+W) / sqrt(Var(Z+V)Var(Z+W))\
  &= Var(Z) / sqrt((Var(Z) + Var(V))(Var(Z) + Var(W)))\
  &= Var(Z) / sqrt((Var(Z) + 1)(Var(Z) + 1))\
  &= Var(Z) / (Var(Z) + 1).
  $
]

#set enum(numbering: "a)")
#question[
  + Verifique que la información mutua está bien definida. Es decir, $I(X,Y) = I(Y,X)$.
  + Una manera para medir dependencia entre dos v.a. $X$ y $Y$ es _medir_ la
    distancia entre la distribución conjunta y el producto de las marginales
    (que corresponde a la distribución bajo independencia). Aunque no cimple
    con todas las propiedades que debe cumplir una de distancia, se una mucho
    la divergencia de Kullback Leibler (KL). Para el caso de dos distrubiciones
    discretas, se define como: $ K L(P|Q) = - sum_x p(x) log q(x)/p(x) = -E_p
    lr(log q(X)/p(X)) $
    Observa que $K L (P|P) = 0$. Verifica que $K L(P|Q) >= 0$.
  + Verifica que $I(X,Y) = K L(P(X,Y) | P(X)P(Y))$.
]

#proof(of: "a")[
Definimos la información mutua como:
$ I(X,Y) colon.eq H(Y) - H_X (Y). $ 
Entonces, queremos demostrar que $H(Y) - H_X (Y) = H(X) - H_Y (X)$. Recordemos que
$ H(Y) colon.eq -sum_y  P(Y=y)log(P(Y=y)) $ $ H_X (Y) colon.eq -sum_x lr(sum_y P(Y=y|X=x) log(P(Y=y|X=x))P(X=x)) $ 
Entonces, por Bayes, tenemos
$ 
H_X (Y) &= -sum_x sum_y P(X = x|Y=y) P(Y=y)/P(X=x)log(P(X=x|Y=y) P(Y=y)/P(X=x))P(X=x)\
&= -sum_x sum_y P(X = x|Y=y) lr(log(P(X=x| Y=y))+ log(P(Y=y) - log(P(X=x))))

$ 
]
