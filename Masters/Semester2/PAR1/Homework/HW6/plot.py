import sys
import pandas as pd
import matplotlib.pyplot as plt


def plot(filename, title, xname, yname, outfile):
  df = pd.read_csv(filename)
  plt.plot(df.iloc[:,0], df.iloc[:, 1], marker='o', linestyle='-', color='b')

  plt.title(title)
  plt.xlabel(xname)
  plt.ylabel(yname)
  plt.grid(True)

  plt.tight_layout()
  plt.savefig(outfile) # Uncomment to save the file

def main():
  if len(sys.argv) != 6:
    print("Invalid argument count")
    return 1
    
  plot(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])

if __name__ == "__main__":
  main()