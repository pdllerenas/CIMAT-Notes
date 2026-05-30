import numpy as np
import matplotlib.pyplot as plt

# Load the CSV data into a 2D numpy array
# Rows are time steps, columns are spatial nodes
data = np.loadtxt('results.csv', delimiter=',')

# Create the heatmap
plt.figure(figsize=(10, 6))
plt.imshow(data, aspect='auto', cmap='inferno', origin='lower')

# Add labels and styling
plt.colorbar(label='Temperature')
plt.title('1D Heat Equation Solution over Time')
plt.xlabel('Spatial Node (x)')
plt.ylabel('Time Step (t)')

# Save the plot or show it interactively
plt.tight_layout()
plt.savefig('heat_plot_102.png', dpi=300)
plt.show()