import numpy as np
import matplotlib.pyplot as plt

def f1(X, Y):
  return 8*X + 12 * Y + X*X - 2 * Y * Y

def f2(X,Y):
  return 100*(Y-X*X)**2 + (1-X)**2

# Create grid
x = np.linspace(-100, 100, 100)
y = np.linspace(-100, 100,100)
X, Y = np.meshgrid(x, y)

# Define function
Z = f2(X,Y)

# Plot contour lines
plt.contour(X, Y, Z)

plt.xlabel("x")
plt.ylabel("y")
plt.title("Contour plot of f(x,y)")
plt.show()
