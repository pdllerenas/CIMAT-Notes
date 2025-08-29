import csv
import sys

import matplotlib.pyplot as plt
import numpy as np


def plot(method):
    with open(method + ".csv", "r") as f:
        reader = csv.reader(f)
        # points = [value for row in reader for value in row]
        points = [row[1] for row in reader]

    rg = np.arange(len(points))
    plt.figure(figsize=(12, 2))
    plt.scatter(rg, points, color="blue")
    plt.yticks([5, 6, 7, 8, 9, 11, 17, 19, 49, 98])

    plt.show()


def main():
    if len(sys.argv) != 3:
        print("Invalid arguments.\n Correct usage: python[3] p2.py [naive | non-naive]")
        sys.exit(1)
    if sys.argv[2] == "naive":
        plot("naive_p2.csv")
    if sys.argv[2] == "non-naive":
        plot("p2.csv")
    else:
        print("Invalid method.\n Correct usage: python[3] p2.py [naive | non-naive]")
        sys.exit(1)

    return 0


if __name__ == "__main__":
    main()
