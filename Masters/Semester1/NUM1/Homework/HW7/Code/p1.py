import numpy as np
import sys

def solve_with_qr(filename_A, filename_b):
    A = np.loadtxt(filename_A)
    b = np.loadtxt(filename_b)
    x = np.linalg.solve(A, b)
    print(x)

def main():
    if (len(sys.argv) != 3):
        print("Invalid argument count.\n")
        sys.exit(1)
    fA = sys.argv[1]
    fb = sys.argv[2]
    solve_with_qr(fA, fb)
    

if __name__ == "__main__":
    main()

