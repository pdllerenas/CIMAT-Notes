import numpy as np
from scipy.linalg import cho_factor, cho_solve, LinAlgError
from backtracking import armijo_backtracking

def newtonGradientDescent(f, g, H, x0, N, tau, alpha=1.0, c=1e-4, rho=0.5):
    xk = np.array(x0, dtype=float)
    res = 0

    for m in range(N):
        gk = g(xk)

        if np.linalg.norm(gk) < tau:
            print(f"newtonGradientDescent converged at {m} iterations.")
            return xk, gk, 1

        Bk = H(xk)

        try:
            c_and_lower = cho_factor(Bk)
            pk = cho_solve(c_and_lower, -gk)
        except LinAlgError:
            pk = -gk

        ak = armijo_backtracking(f, g, xk, pk, alpha=alpha, rho=rho, c=c)

        if ak is not None:
            xk = xk + ak * pk
        else:
            print(
                f"Line search failed at iteration {m}. Step size fell below precision threshold."
            )
            break

    return xk, gk, res


def main():
    return


if __name__ == "__main__":
    main()
