import numpy as np

def solve(R, J, z0, N, mu_r, tol):
    zk = np.array(z0, dtype=float)
    res = 0
    I = np.identity(zk.shape[0])
    R0 = R(z0)
    J0 = J(z0)
    f0 = 0.5 * np.dot(R0, R0)
    A = J0.T @ J0
    g = J0.T @ R0
    mu = min(mu_r, np.max(A))
    return zk, fk, k, res

