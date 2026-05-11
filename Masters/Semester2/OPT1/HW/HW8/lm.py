import numpy as np

def solve(R, J, z0, N, mu_r, tol):
    zk = np.array(z0, dtype=float)
    res = 0
    I = np.identity(zk.shape[0])
    
    Rk = R(zk)
    Jk = J(zk)
    fk = 0.5 * np.dot(Rk, Rk)
    
    A = Jk.T @ Jk
    g = Jk.T @ Rk
    mu = min(mu_r, np.max(A))
    
    for k in range(N):
        pk = np.linalg.solve(A + mu * I, -g)
        
        if np.linalg.norm(pk) < tol:
            res = 1
            break
            
        zk_next = zk + pk
        Rk_next = R(zk_next)
        fk_next = 0.5 * np.dot(Rk_next, Rk_next)
        
        denom = 0.5 * (mu * np.dot(pk, pk) - np.dot(pk, g))
        
        if denom > 1e-12:
            rho = (fk - fk_next) / denom
        else:
            rho = 0
            
        if rho > 0:
            zk = zk_next
            Rk = Rk_next
            fk = fk_next
            Jk = J(zk)
            
            A = Jk.T @ Jk
            g = Jk.T @ Rk
            
            if rho > 0.75:
                mu /= 3
            elif rho < 0.25:
                mu *= 2
        else:
            mu *= 2
            
    return zk, fk, k, res