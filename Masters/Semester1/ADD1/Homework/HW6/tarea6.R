# tarea 6 ex 6

library(ggplot2)
library(GGally)
library(plotly)

mu = runif(4)

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

df <- as.data.frame(samples)

colnames(df) <-c("y1", "y2", "y3", "y4")
plot_ly(df, x = ~y1, y = ~y2, z = ~y3,
        type = "scatter3d", mode = "markers",
        marker = list(size = 4, color = ~y4))

pairs(df)
ggparcoord(df)
