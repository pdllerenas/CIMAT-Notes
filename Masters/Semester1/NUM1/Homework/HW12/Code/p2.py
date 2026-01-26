import sys
import csv
import matplotlib.pyplot as plt
import numpy as np

def plot_csv_points(filenames: str):

    plt.figure()
    # Read CSV
    colors = ["red", "blue", "green", "pink", "black", "gray", "cyan"]
    for i, filename in enumerate(filenames):
        xs = []
        ys = []
        with open(filename, newline="") as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                xs.append(float(row["x"]))
                ys.append(float(row["y"]))

        # Plot
        plt.scatter(xs, ys, color=colors[i], label={filename})

    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.title(f"Scatter Plot")
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python p2.py <csv_files>")
        sys.exit(1)

    files = []
    for i in range(1, len(sys.argv)):
        files.append(sys.argv[i])

    plot_csv_points(files)

