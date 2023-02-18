import pandas as pd
import matplotlib.pyplot as plt

params = {
    "header": None,
    "names": ("operation", "time"),
    "index_col": 0
}

bst = pd.read_csv("./benchmarks/1500_bst.csv", **params)
linkedlist = pd.read_csv("./benchmarks/1500_list.csv", **params)

# Pivot the data so that each operation has its own column
pivoted_bst = bst.pivot(columns='operation', values='time')
pivoted_linkedlist = linkedlist.pivot(columns='operation', values='time')

# Plot each operation in a separate subplot
ax_bst = pivoted_bst.plot(subplots=True, figsize=(8, 10))
ax_ll = pivoted_linkedlist.plot(subplots=True, figsize=(8, 10))

# Set titles and labels
for i, operation in enumerate(pivoted_bst.columns):
    line = ax_ll[i].lines[0]
    print(line.get_xdata())
    ax_bst[i].set_title(operation)
    ax_bst[i].set_xlabel('n')
    ax_bst[i].set_ylabel('time (s)')

# Show the plot
plt.savefig("./list_test.png")
plt.show()
