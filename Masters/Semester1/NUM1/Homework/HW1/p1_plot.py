import matplotlib.pyplot as plt
import numpy as np
import csv
import sys


def plot(n):
    points = []

    with open("binary.csv", "r") as csvfile:
        spamreader = csv.reader(csvfile)
        next(spamreader)

        for row in spamreader:
            for value in row:
                points.append(int(value))

    points = np.unique(np.concatenate([points[:-n], -np.array(points[:n])]))

    plt.figure(figsize=(12, 2))
    plt.scatter(points, np.zeros_like(points), color="blue", s=30, marker="|")

    plt.axhline(0, color="black", linewidth=0.8)

    plt.xticks(
        [points.min(), 0, points.max()], [str(points.min()), "0", str(points.max())]
    )

    plt.yticks([])
    plt.xlabel("Values")
    plt.title(f"16 bit system (only {n} values are shown)")
    plt.show()


def main():
    if len(sys.argv != 3):
        print("Invalid arguments. Usage: python3 p1_plot.py (number_of_points)")
        sys.exit(1)
    if sys.argv[2].isdigit():
        plot(sys.argv[2])
    return 0


if __name__ == "__main__":
    main()
