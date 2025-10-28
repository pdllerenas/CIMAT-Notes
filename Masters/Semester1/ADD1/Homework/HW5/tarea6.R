# tarea 6 ex 6

library(ggplot2)
library(GGally)
library(plotly)
library(gganimate)

mu = c(0, 0, 0, 0)

Sigma = matrix(c(
 1.0, 0.8, 0.3, 0.0,
 0.8, 1.0, 0.2, 0.1,
 0.3, 0.2, 1.0, 0.5,
 0.0, 0.1, 0.5, 1.0
), nrow = 4, byrow = TRUE)

v <- runif(6)

Sigma = matrix(c(
 1.0, v[6], v[1], v[2],
 v[6], 1.0, v[4], v[5],
 v[1], v[4], 1.0, v[3],
 v[2], v[5], v[3], 1.0
), nrow = 4, byrow = TRUE)

L <- chol(Sigma)

Z <- matrix(rnorm(4 * 1000), ncol=4)

samples <- Z %*% L

samples <- sweep(samples, 2, mu, "+")

df <- as.data.frame(samples)
colnames(df) <-c("y1", "y2", "y3", "y4")
plot_ly(df, x = ~y1, y = ~y2, z = ~y3,
        type = "scatter3d", mode = "markers",
        marker = list(size = 2, color = ~y4))

head(df)

#pairs(df, pch=16, col=rgb(0,0,1,0.2))


