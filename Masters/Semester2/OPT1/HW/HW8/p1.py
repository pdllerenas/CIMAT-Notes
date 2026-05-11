import numpy as np
from numpy.typing import NDArray
from ex1 import gen_point
import matplotlib.pyplot as plt

x1_data, x2_data, y_data = gen_point()

y_noise = y_data + np.random.normal(0, 0.2, x1_data.size)


def build_design_matrix(x1, x2, deg):
    N = len(x1)
    p = (deg + 1) * (deg + 2) // 2

    X = np.zeros((N, p))
    col = 0

    for d in range(deg + 1):
        for n in range(d + 1):
            m = d - n
            X[:, col] = (x1**n) * (x2**m)
            col += 1
    return X


def solve(X, y):
    return np.linalg.lstsq(X, y, rcond=None)


def mse(y_actual, y_predicted):
    return np.mean((y_actual - y_predicted) ** 2)


def main():
    grid_res = 50
    x1_grid, x2_grid = np.meshgrid(
        np.linspace(-3, 3, grid_res), np.linspace(-3, 3, grid_res)
    )

    x1_grid_flat = x1_grid.flatten()
    x2_grid_flat = x2_grid.flatten()

    degrees_to_test = [1, 3, 4, 5, 6, 7]
    best_degree = None
    best_X = None
    lowest_true_mse = float('inf')

    print(f"{'Degree':<8} | {'MSE (Dataset / Noisy)':<25} | {'MSE (True Function)':<25}")
    print("-" * 65)

    for d in degrees_to_test:
        X_train = build_design_matrix(x1_data, x2_data, d)
        beta = solve(X_train, y_noise)[0]

        X_plot = build_design_matrix(x1_grid_flat, x2_grid_flat, d)
        y_fit_flat = X_plot @ beta
        y_fit_grid = y_fit_flat.reshape(grid_res, grid_res)


        fig = plt.figure(figsize=(10, 7))
        ax = fig.add_subplot(111, projection="3d")

        ax.plot_surface(x1_grid, x2_grid, y_fit_grid, alpha=0.7, color="orange")

        ax.scatter(x1_data, x2_data, y_data, color="blue", label="True Data", s=20)

        ax.scatter(
            x1_data,
            x2_data,
            y_noise,
            color="red",
            label="Noisy Training Data",
            s=10,
            alpha=0.5,
        )

        ax.set_title(f"Polynomial Fit - Degree {d}")
        ax.set_xlabel("x1")
        ax.set_ylabel("x2")
        ax.set_zlabel("y")
        ax.legend()

        y_train_pred = X_train @ beta
        ax.set_zlim(np.min(y_noise) - 1, np.max(y_noise) + 1)
        mse_dataset = mse(y_noise, y_train_pred)

        mse_true = mse(y_data, y_train_pred)

        if mse_true < lowest_true_mse:
            lowest_true_mse = mse_true
            best_degree = d
            best_X = X_train

        print(f"{d:<8} | {mse_dataset:<25.5f} | {mse_true:<25.5f}")

        plt.savefig(f"deg{d}.jpg")

    print("-" * 65)
    print(
        f"The polynomial that best approximates the true function is Degree {best_degree}."
    )
    return best_X


if __name__ == "__main__":
    main()
