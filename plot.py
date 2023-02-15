import pandas as pd
import matplotlib.pyplot as plt

bst = pd.read_csv("./benchmarks/1500_bst.csv", header=None, names=("operation", "time"), index_col=0)

# Pivot the data so that each operation has its own column
pivoted_df = bst.pivot(columns='operation', values='time')

# Plot each operation in a separate subplot
ax = pivoted_df.plot(subplots=True, figsize=(8, 10))

# Set titles and labels
for i, operation in enumerate(pivoted_df.columns):
    ax[i].set_title(operation)
    ax[i].set_xlabel('n')
    ax[i].set_ylabel('time (s)')

# Show the plot
plt.show()
