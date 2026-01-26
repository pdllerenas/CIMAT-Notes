import sys
import csv
import matplotlib.pyplot as plt
import numpy as np

def plot_csv_points():
    xs = np.linspace(0,10,500)
    ys = 1 + 11.5512 * xs - 2.14286 * xs ** 2 + 0.134524 * xs ** 3

    # Plot
    x = np.linspace(0, 10, 500)
    y = 1.0/(0.0462715 + 0.954729*np.exp(-3.0*x))
    plt.figure()
    plt.plot(x,y, label="c(t)", color="blue")
    plt.plot(xs, ys, label="1 + 11.5512 * x - 2.14286 * x^2 + 0.134524 * x^3", color="red")
    plt.legend()
    plt.xlabel("t")
    plt.ylabel("c")
    plt.title(f"Lagrange vs Logistic")
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    plot_csv_points()

