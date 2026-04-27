import numpy as np
from numpy.typing import NDArray
from backtracking import armijo_backtracking
from sample_functions import *
import matplotlib.pyplot as plt


def plot_optimization_results(f, history_x, history_gk, title, bounds=None):
    """
    Plots the convergence of the gradient norm and the optimization path.
    """
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))

    # --- Plot 1: Convergence (Gradient Norm vs Iteration) ---
    axes[0].plot(history_gk, marker=".", linestyle="-", color="b")
    axes[0].set_yscale("log")
    axes[0].set_xlabel("Iteration")
    axes[0].set_ylabel("Gradient Norm ||g(x)|| (Log Scale)")
    axes[0].set_title(f"{title} - Convergence")
    axes[0].grid(True, which="both", ls="--", alpha=0.5)

    # --- Plot 2: 2D Contour Path OR N-Dimensional Parameter Traces ---
    dims = history_x.shape[1]

    if bounds is not None and dims == 2:
        # Plot 2D Contour Map
        x_min, x_max, y_min, y_max = bounds
        x_grid = np.linspace(x_min, x_max, 400)
        y_grid = np.linspace(y_min, y_max, 400)
        X, Y = np.meshgrid(x_grid, y_grid)

        # Evaluate function over the grid
        Z = np.zeros_like(X)
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                Z[i, j] = f(np.array([X[i, j], Y[i, j]]))

        # Create contour levels (logarithmic spacing is usually best for optimization bowls)
        levels = np.logspace(np.log10(np.min(Z) + 1e-5), np.log10(np.max(Z)), 35)
        cp = axes[1].contour(X, Y, Z, levels=levels, cmap="viridis", alpha=0.8)
        fig.colorbar(cp, ax=axes[1])

        # Overlay the optimization path
        axes[1].plot(
            history_x[:, 0],
            history_x[:, 1],
            "r.-",
            markersize=6,
            linewidth=1.5,
            label="Path",
        )
        axes[1].plot(
            history_x[0, 0], history_x[0, 1], "go", markersize=8, label="Start"
        )
        axes[1].plot(
            history_x[-1, 0], history_x[-1, 1], "rX", markersize=10, label="End"
        )

        axes[1].set_xlabel("x_1")
        axes[1].set_ylabel("x_2")
        axes[1].set_title(f"{title} - Trajectory")
        axes[1].legend()

    else:
        # If dimensions > 2, plot the individual variable values over time
        for i in range(dims):
            axes[1].plot(history_x[:, i], label=f"x_{i+1}")

        axes[1].set_xlabel("Iteration")
        axes[1].set_ylabel("Parameter Value")
        axes[1].set_title(f"{title} - Parameter Trajectories")
        if dims <= 10:
            axes[1].legend()

    axes[1].grid(True, alpha=0.5)
    plt.tight_layout()
    plt.show()


def modifiedBFGS(f, g, x0, H0, tau=1e-15, N=10000, alpha=1, c1=0.1, rho=0.6):
    xk = np.array(x0, dtype=float)
    history_x = [xk]
    gk = g(xk)
    history_gk = [np.linalg.norm(gk)]
    fk = f(xk)
    print(f"k,f_k,||gk||")
    print(f"0,{fk},{history_gk[0]}")
    Hk = np.array(H0, dtype=float)
    I = np.identity(xk.shape[0])
    res = 0
    for k in range(N):
        if np.linalg.norm(gk) < tau:
            print(f"modifiedBFGS converged at {k} iterations.")
            res = 1
            break
        pk = -Hk @ gk
        pkgk = np.dot(pk, gk)
        sq_gk = np.dot(gk, gk)
        if pkgk > 0:
            l1 = 1e-5 + pkgk / sq_gk
            Hk = Hk + l1 * I
            pk = pk - l1 * gk

        ak = armijo_backtracking(f, g, xk, pk, alpha, rho, c1)
        if ak is None:
            print(f"Line search failed at iteration {k}.")
            res = 0
            break

        xk_new = xk + ak * pk
        history_x.append(xk_new)
        gk_new = g(xk_new)
        history_gk.append(np.linalg.norm(gk_new))
        print(f"{k},{f(xk_new)},{history_gk[-1]}")
        sk = xk_new - xk
        yk = gk_new - gk
        skyk = np.dot(sk, yk)
        sq_yk = np.dot(yk, yk)
        if skyk <= 0:
            l2 = 1e-5 - skyk / sq_yk
            Hk = Hk + l2 * I
        else:
            rhok = 1 / skyk
            term1 = I - rhok * np.outer(sk, yk)
            term2 = I - rhok * np.outer(yk, sk)
            Hk = term1 @ Hk @ term2 + rhok * np.outer(sk, sk)
        xk = xk_new
        gk = gk_new
    history_x_array = np.array(history_x)
    history_gk_array = np.array(history_gk)
    return xk, gk, k, res, history_x_array, history_gk_array


def main():
    tau_val = 1e-8

    print("=== Beale Function ===")
    f_beale = lambda x: beale(x[0], x[1])
    g_beale = lambda x: beale_gradient(x[0], x[1])
    x0_beale = np.array([2.0, 3.0])
    H0_beale = np.eye(2)
    x_opt, g_opt, iters, res, hx, hg = modifiedBFGS(
        f_beale, g_beale, x0_beale, H0_beale, tau=tau_val
    )
    if x0_beale.shape[0] <= 6:
        print(f"Final x: {x_opt}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n")
    else:
        print(
            f"Final x: {x_opt[:3]} ,..., {x_opt[-3:]}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n"
        )

    plot_optimization_results(
        f_beale, hx, hg, "Beale Function", bounds=[-1.5, 4.5, -1.5, 4.5]
    )

    print("=== Himmelblau Function ===")
    f_himm = lambda x: himmelblau(x[0], x[1])
    g_himm = lambda x: himmelblau_gradient(x[0], x[1])
    x0_himm = np.array([2.0, 4.0])
    H0_himm = np.eye(2)
    x_opt, g_opt, iters, res, hx, hg = modifiedBFGS(
        f_himm, g_himm, x0_himm, H0_himm, tau=tau_val
    )
    if x0_himm.shape[0] <= 6:
        print(f"Final x: {x_opt}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n")
    else:
        print(
            f"Final x: {x_opt[:3]} ,..., {x_opt[-3:]}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n"
        )
    plot_optimization_results(
        f_himm, hx, hg, "Himmelblau Function", bounds=[-6.0, 6.0, -6.0, 6.0]
    )

    print("=== Rosenbrock Function (2D) ===")
    N = 2
    x0_rosen = np.array([-1.2 if i % 2 == 0 else 1.0 for i in range(N)])
    H0_rosen = np.eye(N)
    x_opt, g_opt, iters, res, hx, hg = modifiedBFGS(
        rosenbrock, rosenbrock_gradient, x0_rosen, H0_rosen, tau=tau_val
    )
    if x0_rosen.shape[0] <= 6:
        print(f"Final x: {x_opt}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n")
    else:
        print(
            f"Final x: {x_opt[:3]} ,..., {x_opt[-3:]}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n"
        )
    plot_optimization_results(
        rosenbrock, hx, hg, "Rosenbrock (2D)", bounds=[-6.0, 6.0, -6.0, 6.0]
    )

    print("=== Rosenbrock Function (200D) ===")
    N = 200
    x0_rosen = np.array([-1.2 if i % 2 == 0 else 1.0 for i in range(N)])
    H0_rosen = np.eye(N)
    x_opt, g_opt, iters, res, hx, hg = modifiedBFGS(
        rosenbrock, rosenbrock_gradient, x0_rosen, H0_rosen, tau=tau_val
    )
    if x0_rosen.shape[0] <= 6:
        print(f"Final x: {x_opt}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n")
    else:
        print(
            f"Final x: {x_opt[:3]} ,..., {x_opt[-3:]}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n"
        )
    plot_optimization_results(rosenbrock, hx, hg, "Rosenbrock (200D)")

    print("=== Rosenbrock Function (600D) ===")
    N = 600
    x0_rosen = np.array([-1.2 if i % 2 == 0 else 1.0 for i in range(N)])
    H0_rosen = np.eye(N)
    x_opt, g_opt, iters, res, hx, hg = modifiedBFGS(
        rosenbrock, rosenbrock_gradient, x0_rosen, H0_rosen, tau=tau_val
    )
    if x0_rosen.shape[0] <= 6:
        print(f"Final x: {x_opt}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n")
    else:
        print(
            f"Final x: {x_opt[:3]} ,..., {x_opt[-3:]}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n"
        )
    plot_optimization_results(rosenbrock, hx, hg, "Rosenbrock (600D)")

    print("=== Hartmann 6D Function ===")
    x0_hart = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
    H0_hart = np.eye(6)
    x_opt, g_opt, iters, res, hx, hg = modifiedBFGS(
        hartmann, hartmann_gradient, x0_hart, H0_hart, tau=tau_val
    )
    if x0_hart.shape[0] <= 6:
        print(f"Final x: {x_opt}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n")
    else:
        print(
            f"Final x: {x_opt[:3]} ,..., {x_opt[-3:]}\nFinal gradient norm: {np.linalg.norm(g_opt):.2e}\n"
        )
    plot_optimization_results(hartmann, hx, hg, "Hartmann 6D")


if __name__ == "__main__":
    main()
