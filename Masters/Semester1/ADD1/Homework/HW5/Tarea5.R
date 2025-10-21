# Exercise 2
library(ggplot2)

inegi = read.csv("inegi.csv")

ggplot(data=inegi, aes(x=fn_mes,  group=estado, color=estado, label=factor(estado))) + geom_density(adjust=1.5, alpha=0.4)

ggplot(data=inegi, aes(x=fn_dia,  group=estado, color=estado,label=factor(estado))) + geom_density(adjust=1.5, alpha=0.4)

       
# Exercise 3

set.seed(103)
n <- 100
b <- rbinom(n, 1, 0.5)
cs <- data.frame(
  game = 1:n,
  outcome = b,
  cumulative = cumsum(b)
)

ggplot(cs, aes(x = game, y = cumulative)) +
  geom_abline(intercept = 0, slope = 0.5, color = "red", linetype = "dashed", size = 1) +
  geom_point(aes(y = outcome + cumsum(b) - b), alpha = 0.3) + 
  theme_minimal() +
  labs(title = "Cumulative Wins",
       x = "Game Number",
       y = "Cumulative Wins")



