import numpy as np
import argparse


def solve_system(A_file: str, b_file: str):
    """
    Solve Ax = b where A and b are read from files.
    """
    # Load A and b
    A = np.loadtxt(A_file)
    b = np.loadtxt(b_file)

    # Solve the system
    x = np.linalg.solve(A, b)
    return x


def main():
    parser = argparse.ArgumentParser(
        description="Solve linear system Ax = b from files"
    )
    parser.add_argument("A_file", type=str, help="Path to file containing matrix A")
    parser.add_argument("b_file", type=str, help="Path to file containing vector b")
    args = parser.parse_args()

    solution = solve_system(args.A_file, args.b_file)
    print("Solution x:", solution)


if __name__ == "__main__":
    main()

# Load A and b from separate files
A = np.loadtxt("A.txt")
b = np.loadtxt("b.txt")

# Solve Ax = b
x = np.linalg.solve(A, b)

print("Solution x:", x)
