import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from lm import solve
from ex1 import gen_point
import matplotlib.pyplot as plt
from matplotlib import cm

x1_data, x2_data, y_data = gen_point()


def f(beta, x1, x2):
    return beta[0] + beta[1] * np.sin(beta[2] * x1) * np.cos(beta[3] * x2)


def residual(beta):
    """
    Computes the residual vector for all data points.
    Returns an array of shape (m,).
    """
    return f(beta, x1_data, x2_data) - y_data


def jacobian(beta):
    """
    Computes the Jacobian matrix J of the residuals with respect to beta.
    J[i, j] = d(residual_i) / d(beta_j)
    Returns a matrix of shape (m, 4).
    """
    m = len(x1_data)
    J = np.zeros((m, 4))

    # partial derivatives w.r.t beta_0, beta_1, beta_2, beta_3, using vectorized assignment
    J[:, 0] = 1.0
    J[:, 1] = np.sin(beta[2] * x1_data) * np.cos(beta[3] * x2_data)
    J[:, 2] = beta[1] * x1_data * np.cos(beta[2] * x1_data) * np.cos(beta[3] * x2_data)
    J[:, 3] = -beta[1] * x2_data * np.sin(beta[2] * x1_data) * np.sin(beta[3] * x2_data)

    return J


def mse(y_actual, y_predicted):
    return np.mean((y_actual - y_predicted) ** 2)


def true_function(x1, x2):
    return np.sin(x1)


def plot_results(beta, title, saveas):
    """Plots the dataset, the true function surface, and the model surface."""
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection="3d")

    # Create a grid for the surfaces
    x1_vals = np.linspace(min(x1_data), max(x1_data), 50)
    x2_vals = np.linspace(min(x2_data), max(x2_data), 50)
    x1_grid, x2_grid = np.meshgrid(x1_vals, x2_vals)

    # Generate Z values for the surfaces
    z_true = true_function(x1_grid, x2_grid)
    z_model = f(beta, x1_grid, x2_grid)

    # Plot true function surface (Blue)
    surf1 = ax.plot_surface(x1_grid, x2_grid, z_true, alpha=0.4, cmap=cm.Blues)

    # Plot model surface (Orange)
    surf2 = ax.plot_surface(x1_grid, x2_grid, z_model, alpha=0.5, cmap=cm.Oranges)

    # Plot the noisy dataset points (Black dots)
    ax.scatter(x1_data, x2_data, y_data, color="black", s=15, label="Noisy Dataset")

    ax.set_xlabel("X1")
    ax.set_ylabel("X2")
    ax.set_zlabel("Y")
    ax.set_title(title)

    plt.savefig(saveas + ".jpg")


def run_experiment(beta0, experiment_name, saveas):
    """Runs the LM solver, prints MSE, and plots the results for a given beta0."""
    print(f"\n{'='*50}")
    print(f"Running {experiment_name}")
    print(f"Initial beta0: {beta0}")
    print(f"{'='*50}")

    muref = 0.001
    tau = np.sqrt(beta0.size) * (np.finfo(float).eps ** (1 / 3))
    N = 200

    # Solve
    zk, fk, k, res = solve(residual, jacobian, beta0, N, muref, tau)

    y_model_pred = f(zk, x1_data, x2_data)
    y_true_vals = true_function(x1_data, x2_data)

    mse_model = mse(y_data, y_model_pred)
    mse_true = mse(y_data, y_true_vals)

    print(f"res = {res}")
    print(f"Iterations completed: {k}")
    print(f"Final fitted parameters (beta): {zk}")
    print(f"Final f(xk): {fk}")
    print(f"MSE (Model vs Dataset):         {mse_model:.6f}")
    print(f"MSE (True Function vs Dataset): {mse_true:.6f}")

    plot_results(zk, f"{experiment_name} (beta0={beta0})", saveas)


def main():
    beta0_1 = np.array([1.0, 1.0, 0.75, 0.5], dtype=float)
    run_experiment(beta0_1, "Experiment 5", "5")

    beta0_2 = np.array([1.0, 0.5, 0.75, 0.5], dtype=float)
    run_experiment(beta0_2, "Experiment 6", "6")


if __name__ == "__main__":
    main()
