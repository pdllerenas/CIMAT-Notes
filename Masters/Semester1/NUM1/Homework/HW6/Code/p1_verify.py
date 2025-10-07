import numpy as np
import sys


def matrix_eigenvalues(filename, n, rev=True):
    A = np.loadtxt(filename)
    eigenvalues, eigenvectors = np.linalg.eig(A)

    sorted_pairs = sorted(
        zip(eigenvalues, eigenvectors.T), key=lambda pair: pair[0], reverse=rev
    )

    eigvals = [pair[0].real for pair in sorted_pairs[:n]]
    eigvecs = [pair[1].real for pair in sorted_pairs[:n]]

    np.savetxt("py_eigenvalues.txt", eigvals, fmt="%.15f")
    np.savetxt("py_eigenvectors.txt", eigvecs, fmt="%.15f")
    residuals = []
    for i in range(len(eigvals)):
        v = eigvecs[i]
        print(eigvals[i])
        print(v)
        lam = eigvals[i]
        Av = A @ v
        res = Av - lam * v
        residuals.append(res)

    residuals = np.array(residuals)
    np.savetxt("py_verify.txt", residuals, fmt="%.15e")


def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <matrix-file> <n-values> <reversed(0|1)>")
        sys.exit(1)
    matrix_eigenvalues(
        sys.argv[1], int(sys.argv[2]), False if int(sys.argv[3]) == 0 else True
    )


if __name__ == "__main__":
    main()
