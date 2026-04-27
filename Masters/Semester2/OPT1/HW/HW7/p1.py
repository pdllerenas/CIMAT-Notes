import numpy as np
from sample_functions import *
from scipy.sparse.linalg import eigsh


def getExtremeEigenvalues(A: NDArray[np.float64]):
    largest = eigsh(A, k=1, which="LA", return_eigenvectors=False)[0]
    smallest = eigsh(A, k=1, sigma=0, which="LM", return_eigenvectors=False)[0]
    if largest and smallest:
        return largest, smallest
    else:
        return 0, 0


def isDefinite(l1, ln):
    print(f"Smallest: {l1}\nLargest: {ln}")
    if l1 > 0:
        print("Positive definite matrix")
    elif ln < 0:
        print("Negative definite matrix")
    else:
        print("Indefinite matrix")


def main():
    l1, l2 = getExtremeEigenvalues(beale_hessian(0, 0))
    print("Beale hessian:")
    isDefinite(l1, l2)
    print()
    l1, l2 = getExtremeEigenvalues(himmelblau_hessian(0, 0))
    print("Himmelblau hessian:")
    isDefinite(l1, l2)
    print()
    l1, l2 = getExtremeEigenvalues(hartmann_hessian(np.array([0, 0, 0, 0, 0, 0])))
    print("hartmann hessian:")
    isDefinite(l1, l2)
    print()
    l1, l2 = getExtremeEigenvalues(rosenbrock_hessian(np.array([1, 1, 1, 1, 1])))
    print("rossenbrock hessian:")
    isDefinite(l1, l2)
    print()


if __name__ == "__main__":
    main()
