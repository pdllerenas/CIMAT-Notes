import numpy as np
import matplotlib.pyplot as plt

# 1. Load the experimental data
# Assuming 'datos_358.dat' has two columns separated by spaces or tabs: i and VFC_i
data = np.loadtxt('datos_358.csv', delimiter=',')
i_exp = data[:, 0]
v_exp = data[:, 1]

# 2. Define the mathematical model (Equation 5) in Python
def calculate_V(i, theta):
    Eo, b, Re, C1, C2 = theta
    
    # Safety checks to prevent log of zero or negative numbers
    safe_i = np.where(i <= 0.0, 1e-9, i)
    ln_arg = 1.0 - C2 * safe_i
    ln_arg = np.where(ln_arg <= 0.0, 1e-9, ln_arg)
    
    # E_cell = Eo - b*log10(i) - Re*i + C1*ln(1 - C2*i)
    return Eo - b * np.log10(safe_i) - Re * safe_i + C1 * np.log(ln_arg)

# 3. Input your estimated parameters here!
theta_SSE = np.loadtxt('sse_output.csv', delimiter=',')
theta_SAE = np.loadtxt('sae_output.csv', delimiter=',')
theta_MAE = np.loadtxt('mae_output.csv', delimiter=',')

# Generate a smooth array of current densities (i) for drawing the lines
# We start slightly above 0 to avoid log(0) domain errors
i_smooth = np.linspace(1e-5, max(i_exp), 200)

# Calculate the estimated voltage curves
v_sse = calculate_V(i_smooth, theta_SSE)
v_sae = calculate_V(i_smooth, theta_SAE)
v_mae = calculate_V(i_smooth, theta_MAE)

# 4. Create the plot matching the project document's style
plt.figure(figsize=(8, 5))

# Plot the smooth estimation curves
plt.plot(i_smooth, v_sse, label='Estimation with SSE', color='darkblue', linestyle='-', linewidth=2.5)
plt.plot(i_smooth, v_sae, label='Estimation with SAE', color='forestgreen', linestyle='--', linewidth=2.5)
plt.plot(i_smooth, v_mae, label='Estimation with MAE', color='purple', linestyle=':', linewidth=2.5)

# Plot the experimental data as red scatter dots
plt.scatter(i_exp, v_exp, label='Experimental data', color='red', marker='o', zorder=5)

# Formatting the plot
plt.title('DMFC Voltage vs. Current Density')
plt.xlabel('Current density (A $cm^{-2}$)', fontsize=12)
plt.ylabel('Voltage (V)', fontsize=12)
plt.xlim(0, max(i_exp) * 1.05) # Keep x-axis origin at 0
plt.ylim(0, 1.0)               # Match the 0.0 to 1.0 y-axis from the PDF
plt.legend(fontsize=10)
plt.grid(True)

# Show the final graph
plt.tight_layout()
plt.show()