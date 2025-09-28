import numpy as np
import sys


def matrix_eigenvalues(filename, n, rev=True):
    A = np.loadtxt(filename)
    eigenvalues, eigenvectors = np.linalg.eig(A)

    sorted_pairs = sorted(
        zip(eigenvalues, eigenvectors.T), key=lambda pair: pair[0], reverse=rev
    )

    print("Sorted eigenvalues of the matrix:")
    for eigenvalue, eigenvectors in sorted_pairs[:n]:
        # for x in eigenvectors:
        #     print(f"{x:.2f}", end=" ")
        # print()
        if np.iscomplex(eigenvalue):
            print(f"{eigenvalue.real:.6f} + {eigenvalue.imag:.6f}i")
        else:
            print(f"{eigenvalue.real:.6f}")


def main():
    if len(sys.argv) != 4:
        print(
            f"Invalid argument count. Usage: {
                sys.argv[0]
            } <matrix-file> <n-values> <reversed(0 | 1)>"
        )
        sys.exit(1)
    matrix_eigenvalues(
        sys.argv[1], int(sys.argv[2]), False if int(sys.argv[3]) == 0 else True
    )


if __name__ == "__main__":
    main()
