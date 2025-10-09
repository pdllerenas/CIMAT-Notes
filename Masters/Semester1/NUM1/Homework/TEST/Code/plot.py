import matplotlib.pyplot as plt
import sys


def read_points(filename):
    x_vals = []
    y_vals = []
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            if " " in line:
                parts = line.split(" ")
            else:
                parts = line.split()

            if len(parts) != 2:
                continue
            x, y = map(float, parts)
            x_vals.append(x)
            y_vals.append(y)
    return x_vals, y_vals


def plot_points(filename):
    x, y = read_points(filename)
    plt.figure(figsize=(8, 6))
    plt.scatter(x, y, color="red", marker="x")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    if (len(sys.argv) != 2):
        print("Insufficient arguments.")
        sys.exit(1)
    filename = sys.argv[1]
    plot_points(filename)
