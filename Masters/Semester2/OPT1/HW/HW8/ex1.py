import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
import scipy
from scipy import linalg

# Generate the true function and the data set
def gen_point():
    np.random.seed(10)
    true_function = lambda x1,x2: np.sin(x1) 

    x1 = np.array([i*np.pi/180 for i in range(60,300,20)])
    x2 = np.linspace(1,5,len(x1))

    x1_grid,x2_grid = np.meshgrid(x1,x2)
    y = true_function(x1_grid,x2_grid) + np.random.normal(0,0.15,(x1_grid.shape))

    x1_data = x1_grid.flatten()
    x2_data = x2_grid.flatten()
    y_data = y.flatten()
    return x1_data, x2_data, y_data

def main():
    return 0

if __name__ == "__main__":
    main()
