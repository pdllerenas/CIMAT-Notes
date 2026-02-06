import numpy as np
import matplotlib.pyplot as plt
from itertools import combinations

def f1(x, y):
    return x*x - y*y

def f2(x, y):
    return 2*x*y

def segment_intersection(p1, p2, p3, p4):
    x1, y1 = p1
    x2, y2 = p2
    x3, y3 = p3
    x4, y4 = p4

    den = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    if abs(den) < 1e-10:
        return None

    px = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4 - y3*x4)) / den
    py = ((x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4 - y3*x4)) / den

    def between(a, b, c):
        return min(a, b) - 1e-10 <= c <= max(a, b) + 1e-10

    if (between(x1, x2, px) and between(y1, y2, py) and
        between(x3, x4, px) and between(y3, y4, py)):
        return np.array([px, py])

    return None

def plot_levels_with_intersections(level_sets):
    plt.figure(figsize=(6, 6))

    x_ = np.linspace(-180, 180, 600)
    y_ = np.linspace(-180, 180, 600)
    x, y = np.meshgrid(x_, y_)

    contours = []

    for spec in level_sets:
        Z = spec["f"](x, y)
        cs = plt.contour(
            x, y, Z,
            levels=[spec["level"]],
            colors=spec["color"],
            linewidths=2
        )
        plt.clabel(cs, fmt=spec["label"])
        contours.append(cs)


    intersections = []

    for c1, c2 in combinations(contours, 2):
        for v1 in c1.allsegs[0]:
            for v2 in c2.allsegs[0]:
                for i in range(len(v1) - 1):
                    for j in range(len(v2) - 1):
                        pt = segment_intersection(
                            v1[i], v1[i+1],
                            v2[j], v2[j+1]
                        )
                        if pt is not None:
                            intersections.append(pt)


    if intersections:
        intersections = np.unique(np.round(intersections, 5), axis=0)
        intersections = np.array(intersections)
        plt.scatter(
            intersections[:,0],
            intersections[:,1],
            color="black",
            zorder=5,
            label="Intersections"
        )

    plt.axhline(0)
    plt.axvline(0)
    plt.axis("equal")
    plt.legend()
    plt.title("Level sets with intersections")
    plt.show()

def main():
    level_sets = [
        {"f": f1, "level": 12, "color": "blue", "label": "f1 = 12"},
        {"f": f2, "level": 16,  "color": "red",  "label": "f2 = 16"},
    ]

    plot_levels_with_intersections(level_sets)

if __name__ == "__main__":
    main()

