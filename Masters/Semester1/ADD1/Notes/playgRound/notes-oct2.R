data <- tibble(x = c(1,4,2), y=c(2,2,1), z=c(10,17, 8))

subset <- filter(data, y == 2, z < 15)
subset <- filter(data, (y == 2) & (z < 15))

sorted <- arrange(data, x)

x <- runif(1000)
y <- rgamma(1000, 0.4)
plot(x,y)
ggplot()

library(ggplot2)

ggplot(mpg, aes(displ, hwy, colour = class)) +
  geom_point(mpg, aes(hwy, displ, color=class))


coplot(weight ~ Time | Chick, data = ChickWeight,
       type = "b", show.given = FALSE)
ggplot(ChickWeight, aes(weight, time, color = Chick)) + geom_point()
