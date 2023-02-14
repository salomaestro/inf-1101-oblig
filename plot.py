import csv
import numpy as np
from matplotlib import pyplot as plt
from pathlib import Path

benchmarks = Path(".").resolve() / "benchmarks"

def read_csv_to_array(filepath:str) -> np.ndarray:
    """Read a file into a numpy array, file may contain strings and numerical
    values (which is why numpy's genfromtxt does not work for the task).

    Args:
        filepath (str): File with data.

    Returns:
        numpy.ndarray: Array of data.
    """
    data = []
    with open(filepath, newline="\n") as f:
        reader = csv.reader(f, delimiter=",")
        for row in reader:
            operations, operation, time_took = row
            data.append(np.array([int(operations), operation, float(time_took)]))

    return np.asarray(data)


timed_10_list = read_csv_to_array(benchmarks / "timed_10_list.csv")
timed_10_bst = read_csv_to_array(benchmarks / "timed_10_bst.csv")


union_list = timed_10_list[timed_10_list[:, 1] == "union"]
intersection_list = timed_10_list[timed_10_list[:, 1] == "intersection"]
difference_list = timed_10_list[timed_10_list[:, 1] == "difference"]

union_bst = timed_10_bst[timed_10_bst[:, 1] == "union"]
intersection_bst = timed_10_bst[timed_10_bst[:, 1] == "intersection"]
difference_bst = timed_10_bst[timed_10_bst[:, 1] == "difference"]

plt.boxplot(difference_list[difference_list[:, 0] == '100'][:, 2])
plt.show()

# plt.plot(difference_list, label="list")
# plt.plot(difference_bst, label="bst")
# plt.legend()
# plt.show()
