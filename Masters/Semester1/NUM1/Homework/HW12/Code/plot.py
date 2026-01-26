import sys
import csv
import matplotlib.pyplot as plt

colors = ["red", "blue", "purple", "cyan"]

def plot_csv_points(filename: str):
    # Read CSV
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

    # Always plot these columns in red
    for i, col in enumerate(y_columns):
        values = [float(row[col]) for row in rows]
        plt.scatter(years, values, color=colors[i], label=col)

    plt.xlabel("year")
    plt.ylabel("population")
    plt.title("Scatter Plot of Lynx-Rabbit Populations")
    plt.legend()
    plt.grid(True)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python p2.py <csv_file>")
        sys.exit(1)

    filename = sys.argv[1]
    plot_csv_points(filename)

    # NOTE: do not call plt.show() here  
    # so user can overlay more plots before showing

