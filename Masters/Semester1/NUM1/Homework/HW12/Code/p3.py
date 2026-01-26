from plot import plot_csv_points
import matplotlib.pyplot as plt
import sys
import csv


colors = ["purple", "cyan"]
colors2 = ["red", "blue"]
def main():
    if len(sys.argv) < 2:
        print("Usage: python p2.py <csv_file>")
        sys.exit(1)

    filename = sys.argv[1]
    filename2 = "ex/lynx-rabbit.txt"

    with open(filename, newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        rows = list(reader)


# Extract original years
    years_raw = [float(row["year"]) for row in rows]

# Reindex years to start at 0
    year0 = min(years_raw)
    years = [y - year0 for y in years_raw]

# Identify y-columns (rabbit, lynx, etc.)
    y_columns = [col for col in rows[0].keys() if col != "year"]

    plt.figure()

# Always plot these columns in red
    for i, col in enumerate(y_columns):
        values = [float(row[col]) for row in rows]
        plt.scatter(years, values, color=colors[i], label={"modeled " + col})
        plt.plot(years, values, color=colors[i])

    with open(filename2, newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        rows = list(reader)
# Extract original years
    years_raw = [float(row["year"]) for row in rows]

# Reindex years to start at 0
    year0 = min(years_raw)
    years = [y - year0 for y in years_raw]

# Identify y-columns (rabbit, lynx, etc.)
    y_columns = [col for col in rows[0].keys() if col != "year"]
    for i, col in enumerate(y_columns):
        values = [float(row[col]) for row in rows]
        plt.scatter(years, values, color=colors2[i], label={"observed " + col})
    plt.legend()
    plt.xlabel("year")
    plt.ylabel("population")
    plt.title("Method approximation of population dynamic")
    plt.show()


if __name__ == "__main__":
    main()
