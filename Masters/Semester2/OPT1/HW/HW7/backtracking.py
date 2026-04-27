import numpy as np

def armijo_backtracking(f, g, xk, pk, alpha=1.0, rho=0.5, c=1e-4):
    fk = f(xk)
    gk = g(xk)

    dir_deriv = np.dot(gk, pk)

    if dir_deriv >= 0:
        print("Error: Search direction is not a descent direction.")
        return None

    while f(xk + alpha * pk) > fk + c * alpha * dir_deriv:
        alpha *= rho

        if alpha < 1e-14:
            return None

    return alpha