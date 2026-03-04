import numpy as np
import matplotlib.pyplot as plt

# Define the function
def f(x, y):
    return (x**2 + y**2 - 1)**2 + (y**2 - 1)**2

# Create grid
x = np.linspace(-2, 2, 400)
y = np.linspace(-2, 2, 400)
X, Y = np.meshgrid(x, y)

# Evaluate function
Z = f(X, Y)

# Create contour plot
plt.figure(figsize=(6, 5))
contours = plt.contour(X, Y, Z, levels=20)
plt.clabel(contours, inline=True, fontsize=8)

plt.xlabel("x")
plt.ylabel("y")
plt.title("Contour plot of f(x, y)")
plt.colorbar(contours)

plt.show()