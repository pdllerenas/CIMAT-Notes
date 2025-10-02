# Problem 1
sum(seq(1,99,2)^2)

# verification
n<-50
n*(2*n+1)*(2*n-1)/3

# Problem 2
p <- 1000
x<-runif(p, min=-1, max=1)
y<-runif(p, min=-1, max=1)

xc = x / abs(x)
yc = y / abs(y)

dx = x - xc
dy = y - yc

d<-sqrt(dx^2+dy^2)

plot(x,y)
hist(d)
rug(d)

# Problem 3

ar = USArrests
mean(ar$Rape)

# https://www.rdocumentation.org/packages/base/versions/3.6.2/topics/row%2Bcolnames
# https://www.rdocumentation.org/packages/base/versions/3.6.2/topics/which.min
rownames(ar)[which.max(ar$Assault)]
