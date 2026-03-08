import pandas as pd

import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('divisions.csv')

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(df['C'], df['S(C)'], label='S(C)', linewidth=2)
plt.plot(df['C'], df['D(C)'], label='D(C)', linewidth=2)

# Customize the plot
plt.xlabel('C')
plt.ylabel('Value')
plt.title('Plot of S(C) and D(C) vs C')
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()

# Display the plot
plt.show()