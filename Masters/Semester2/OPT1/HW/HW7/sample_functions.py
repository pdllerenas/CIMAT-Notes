import numpy as np
from numpy.typing import NDArray


def beale(x: float, y: float) -> float:
    return (
        (1.5 - x + x * y) ** 2
        + (2.25 - x + x * y**2) ** 2
        + (2.625 - x + x * y**3) ** 2
    )


def beale_hessian(x: float, y: float) -> NDArray[np.float64]:
    r1 = 1.5 - x + x * y
    r2 = 2.25 - x + x * y**2
    r3 = 2.625 - x + x * y**3

    J = np.array([[y - 1, x], [y**2 - 1, 2 * x * y], [y**3 - 1, 3 * x * y**2]])

    H_r1 = np.array([[0.0, 1.0], [1.0, 0.0]])
    H_r2 = np.array([[0.0, 2.0 * y], [2.0 * y, 2.0 * x]])
    H_r3 = np.array([[0.0, 3.0 * y**2], [3.0 * y**2, 6.0 * x * y]])

    H = 2.0 * (J.T @ J) + 2.0 * (r1 * H_r1 + r2 * H_r2 + r3 * H_r3)
    return H


def himmelblau(x: float, y: float) -> float:
    return (x**2 + y - 11) ** 2 + (x + y**2 - 7) ** 2


def himmelblau_hessian(x: float, y: float) -> NDArray[np.float64]:
    H = np.zeros((2, 2))
    H[0, 0] = 12 * x**2 + 4 * y - 42
    H[0, 1] = 4 * (x + y)
    H[1, 0] = 4 * (x + y)
    H[1, 1] = 12 * y**2 + 8 * x * y - 26
    return H


def hartmann(x: NDArray[np.float64]) -> float:
    alpha = np.array([1, 1.2, 3, 3.2])
    A = np.array(
        [
            [10, 3, 17, 3.5, 1.7, 8],
            [0.05, 10, 17, 0.1, 8, 14],
            [3, 3.5, 1.7, 10, 17, 8],
            [17, 8, 0.05, 10, 0.1, 14],
        ]
    )
    P = (
        np.array(
            [
                [1312, 1696, 5569, 124, 8283, 5886],
                [2329, 4135, 8307, 3736, 1004, 9991],
                [2348, 1451, 3522, 2883, 3047, 6650],
                [4047, 8828, 8732, 5743, 1091, 381],
            ]
        )
        * 1e-4
    )
    inner_sum = np.sum(A * (x - P) ** 2, axis=1)
    return -(1.0 / 1.94) * (2.58 + np.sum(alpha * np.exp(-inner_sum)))


def hartmann_hessian(x: NDArray[np.float64]) -> NDArray[np.float64]:
    alpha = np.array([1.0, 1.2, 3.0, 3.2])
    A = np.array(
        [
            [10, 3, 17, 3.5, 1.7, 8],
            [0.05, 10, 17, 0.1, 8, 14],
            [3, 3.5, 1.7, 10, 17, 8],
            [17, 8, 0.05, 10, 0.1, 14],
        ]
    )
    P = (
        np.array(
            [
                [1312, 1696, 5569, 124, 8283, 5886],
                [2329, 4135, 8307, 3736, 1004, 9991],
                [2348, 1451, 3522, 2883, 3047, 6650],
                [4047, 8828, 8732, 5743, 1091, 381],
            ]
        )
        * 1e-4
    )

    c = 1.0 / 1.94
    diff = x - P
    inner_sum = np.sum(A * diff**2, axis=1)
    E = np.exp(-inner_sum)

    H = np.zeros((6, 6))

    for j in range(6):
        for k in range(6):
            delta_jk = 1.0 if j == k else 0.0

            term = (
                alpha
                * A[:, j]
                * E
                * (delta_jk - 2.0 * A[:, k] * diff[:, j] * diff[:, k])
            )
            H[j, k] = 2.0 * c * np.sum(term)

    return H


def rosenbrock(x: NDArray[np.float64]) -> float:
    return np.sum(100.0 * (x[1:] - x[:-1] ** 2) ** 2 + (1 - x[:-1]) ** 2)


def rosenbrock_hessian(x: NDArray[np.float64]) -> NDArray[np.float64]:
    n = x.size
    H = np.zeros((n, n))

    H[0, 0] = 1200.0 * x[0] ** 2 - 400.0 * x[1] + 2.0
    for i in range(1, n - 1):
        H[i, i] = 1200.0 * x[i] ** 2 - 400.0 * x[i + 1] + 202.0
    H[n - 1, n - 1] = 200.0

    for i in range(n - 1):
        val = -400.0 * x[i]
        H[i, i + 1] = val
        H[i + 1, i] = val

    return H


def beale_gradient(x: float, y: float) -> NDArray[np.float64]:
    # Formulate as 2 * J^T * r to mirror the Hessian structure
    r = np.array([1.5 - x + x * y, 2.25 - x + x * y**2, 2.625 - x + x * y**3])

    J = np.array(
        [[y - 1.0, x], [y**2 - 1.0, 2.0 * x * y], [y**3 - 1.0, 3.0 * x * y**2]]
    )

    return 2.0 * (J.T @ r)


def himmelblau_gradient(x: float, y: float) -> NDArray[np.float64]:
    df_dx = 4.0 * x * (x**2 + y - 11.0) + 2.0 * (x + y**2 - 7.0)
    df_dy = 2.0 * (x**2 + y - 11.0) + 4.0 * y * (x + y**2 - 7.0)

    return np.array([df_dx, df_dy])


def hartmann_gradient(x: NDArray[np.float64]) -> NDArray[np.float64]:
    alpha = np.array([1.0, 1.2, 3.0, 3.2])
    A = np.array(
        [
            [10.0, 3.0, 17.0, 3.5, 1.7, 8.0],
            [0.05, 10.0, 17.0, 0.1, 8.0, 14.0],
            [3.0, 3.5, 1.7, 10.0, 17.0, 8.0],
            [17.0, 8.0, 0.05, 10.0, 0.1, 14.0],
        ]
    )
    P = (
        np.array(
            [
                [1312.0, 1696.0, 5569.0, 124.0, 8283.0, 5886.0],
                [2329.0, 4135.0, 8307.0, 3736.0, 1004.0, 9991.0],
                [2348.0, 1451.0, 3522.0, 2883.0, 3047.0, 6650.0],
                [4047.0, 8828.0, 8732.0, 5743.0, 1091.0, 381.0],
            ]
        )
        * 1e-4
    )

    c = 1.0 / 1.94
    diff = x - P
    inner_sum = np.sum(A * diff**2, axis=1)
    E = np.exp(-inner_sum)

    g = np.zeros(6)

    for j in range(6):
        term = alpha * E * A[:, j] * diff[:, j]
        g[j] = 2.0 * c * np.sum(term)

    return g


def rosenbrock_gradient(x: NDArray[np.float64]) -> NDArray[np.float64]:
    n = x.size
    g = np.zeros(n)

    g[0] = -400.0 * x[0] * (x[1] - x[0] ** 2) - 2.0 * (1.0 - x[0])

    for i in range(1, n - 1):
        g[i] = (
            200.0 * (x[i] - x[i - 1] ** 2)
            - 400.0 * x[i] * (x[i + 1] - x[i] ** 2)
            - 2.0 * (1.0 - x[i])
        )

    g[n - 1] = 200.0 * (x[n - 1] - x[n - 2] ** 2)

    return g
