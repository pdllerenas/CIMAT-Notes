import numpy as np
import sys


def matrix_system(matrix_file, vector_file):
    A = np.loadtxt(matrix_file)
    b = np.loadtxt(vector_file)
    x = np.linalg.solve(A, b)
    for xi in x:
        print(xi, end=" ")


def matrix_eigenvalues(filename, n, rev=True):
    A = np.loadtxt(filename)
    eigenvalues, eigenvectors = np.linalg.eig(A)

    sorted_pairs = sorted(
        zip(eigenvalues, eigenvectors.T), key=lambda pair: pair[0], reverse=rev
    )

    eigvals = [pair[0].real for pair in sorted_pairs[:n]]
    eigvecs = [pair[1].real for pair in sorted_pairs[:n]]

    for i in range(len(eigvals)):
        v = eigvecs[i]
        print(eigvals[i])
        print(v)


def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <matrix-file> <n-values> <reversed(0|1)>")
        sys.exit(1)
    matrix_eigenvalues(
        sys.argv[1], int(sys.argv[2]), False if int(sys.argv[3]) == 0 else True
    )
    # matrix_system("matrix.txt", "vector.txt")


if __name__ == "__main__":
    main()
