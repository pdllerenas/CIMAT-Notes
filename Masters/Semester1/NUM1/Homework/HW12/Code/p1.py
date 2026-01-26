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
            xs.append(float(row["x"]))
            ys.append(float(row["y"]))

    # Plot
    x = np.linspace(-1, 5, 500)
    y = np.exp(x)
    plt.figure()
    plt.plot(x,y, label="e^x")
    plt.legend()
    plt.scatter(xs, ys, color="red")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(f"Scatter Plot: {filename}")
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_csv.py <csv_file>")
        sys.exit(1)

    csv_file = sys.argv[1]
    plot_csv_points(csv_file)

