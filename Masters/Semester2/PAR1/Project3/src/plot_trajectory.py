import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

# ==========================================
# 1. MATHEMATICAL FUNCTIONS FOR THE BACKGROUND
# ==========================================
def rosenbrock(x, y):
    return (1.0 - x)**2 + 100.0 * (y - x**2)**2

def michalewicz(x, y, m=10):
    term1 = np.sin(x) * (np.sin(1 * x**2 / np.pi)**(2 * m))
    term2 = np.sin(y) * (np.sin(2 * y**2 / np.pi)**(2 * m))
    return -(term1 + term2)

# ==========================================
# 2. PLOTTING LOGIC
# ==========================================
def plot_optimization_trajectory(csv_file, function_name):
    print(f"Loading data from {csv_file}...")
    
    # Read the verbose CSV output
    df = pd.read_csv(csv_file, encoding="UTF-16")
    
    # Ensure it's a 2D run
    if 'x1' not in df.columns or 'x2' not in df.columns:
        print("Error: The CSV must contain 'x1' and 'x2' columns (2D optimization).")
        return

    # Extract coordinates
    step = 100
    x1_traj = df['x1'].values[::step]
    x2_traj = df['x2'].values[::step]

    # Set up the plot
    fig, ax = plt.subplots(figsize=(10, 8))
    
    # Define boundaries and function based on user choice
    if function_name.lower() == 'rosenbrock':
        # Focus on the "banana" valley around the global minimum (1, 1)
        x_bounds = [-3.0, 3.0]
        y_bounds = [-2.0, 4.0]
        X, Y = np.meshgrid(np.linspace(x_bounds[0], x_bounds[1], 400),
                           np.linspace(y_bounds[0], y_bounds[1], 400))
        Z = rosenbrock(X, Y)
        # Log scale helps visualize the steep Rosenbrock valley
        levels = np.logspace(-1, 3, 20) 
        true_min = (1.0, 1.0)
        
    elif function_name.lower() == 'michalewicz':
        x_bounds = [0, np.pi]
        y_bounds = [0, np.pi]
        X, Y = np.meshgrid(np.linspace(x_bounds[0], x_bounds[1], 400),
                           np.linspace(y_bounds[0], y_bounds[1], 400))
        Z = michalewicz(X, Y)
        levels = np.linspace(-2, 0, 20)
        true_min = (2.20, 1.57) # Approximate 2D global minimum
        
    else:
        print("Function not recognized. Use 'rosenbrock' or 'michalewicz'.")
        return

    # 1. Plot the background contour map
    contour = ax.contourf(X, Y, Z, levels=levels, cmap='viridis', alpha=0.8)
    fig.colorbar(contour, ax=ax, label='Objective Value')

    # 2. Plot the trajectory
    # We use a colormap based on the iteration number so you can see time passing
    iterations = np.arange(len(x1_traj))
    scatter = ax.scatter(x1_traj, x2_traj, c=iterations, cmap='autumn', 
                         edgecolor='black', zorder=3, linewidths=0.5, s=80, label='Algorithm Steps')
    
    # Draw faint lines connecting the steps
    ax.plot(x1_traj, x2_traj, color='white', alpha=0.8, linewidth=1.5,zorder=2)

    # 3. Mark the Start, End, and True Minimum
    ax.scatter(x1_traj[0], x2_traj[0], color='cyan', marker='s', s=100, zorder=4, label='Start Point')
    ax.scatter(x1_traj[-1], x2_traj[-1], color='magenta', marker='X', s=150, zorder=4, label='End Point')
    ax.scatter(true_min[0], true_min[1], color='red', marker='*', s=250, zorder=5, label='Global Minimum')

    # Formatting
    ax.set_xlim(x_bounds)
    ax.set_ylim(y_bounds)
    ax.set_title(f"Harmony Search Trajectory: {function_name.capitalize()} Function", fontsize=14)
    ax.set_xlabel("x1")
    ax.set_ylabel("x2")
    ax.legend(loc='upper right')
    
    plt.tight_layout()
    plt.show()

# ==========================================
# 3. COMMAND LINE EXECUTION
# ==========================================
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python plot_trajectory.py <path_to_csv> <function_name>")
        print("Example: python plot_trajectory.py rosenbrock_traj.csv rosenbrock")
    else:
        csv_file = sys.argv[1]
        func_name = sys.argv[2]
        plot_optimization_trajectory(csv_file, func_name)