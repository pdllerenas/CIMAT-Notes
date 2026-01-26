import sys
import csv
import matplotlib.pyplot as plt
import numpy as np

def plot_csv_points(filename: str):
    xs = []
    ys = []

    # Read CSV
    with open(filename, newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            xs.append(float(row["t"]))
            ys.append(float(row["x"]))

    # Plot
    x = np.linspace(0, 2, 500)
    y = 1.5*np.exp(x) - 0.5 * np.sin(x)
    plt.figure()
    plt.plot(x,y, label="1.5 e^t - 0.5 sin(t)", color="blue")
    plt.legend()
    plt.scatter(xs, ys, color="red", alpha=1)
    plt.xlabel("t")
    plt.ylabel("u_1(t)")
    plt.title(f"Scatter Plot: {filename}")
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_csv.py <csv_file>")
        sys.exit(1)

    csv_file = sys.argv[1]
    plot_csv_points(csv_file)

