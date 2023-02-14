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
            data.append(
                np.array([int(operations), operation, float(time_took)])
            )

    return np.asarray(data)


timed_100_list = read_csv_to_array(benchmarks / "timed_100_list.csv")
timed_100_bst = read_csv_to_array(benchmarks / "timed_100_bst.csv")


union_list = timed_100_list[timed_100_list[:, 1] == "union"]
intersection_list = timed_100_list[timed_100_list[:, 1] == "intersection"]
difference_list = timed_100_list[timed_100_list[:, 1] == "difference"]

union_bst = timed_100_bst[timed_100_bst[:, 1] == "union"]
intersection_bst = timed_100_bst[timed_100_bst[:, 1] == "intersection"]
difference_bst = timed_100_bst[timed_100_bst[:, 1] == "difference"]

fig, ax = plt.subplots(6, 1, figsize=(12, 10), sharex=True)

col = {
    "gr": (0, 1, 0, 0.5),
    "re": (1, 0, 0, 0.5),
    "bl": (0, 0, 1, 0.5)
}

uni100_list = union_list[union_list[:, 0] == "100"][:, 2].astype(float)
uni1000_list = union_list[union_list[:, 0] == "1000"][:, 2].astype(float)
uni10000_list = union_list[union_list[:, 0] == "10000"][:, 2].astype(float)

ax[0].set_title("Union list")
ax[0].hist(uni100_list, range=(0, uni100_list.max()), bins=50, density=True, fc=col["bl"], label="100")
ax[0].hist(uni1000_list, range=(0, uni1000_list.max()), bins=50, density=True, fc=col["re"], label="1000")
ax[0].hist(uni10000_list, range=(0, uni10000_list.max()), bins=50, density=True, fc=col["gr"], label="10000")
ax[0].legend()

intersect100_list = intersection_list[intersection_list[:, 0] == "100"][:, 2].astype(float)
intersect1000_list = intersection_list[intersection_list[:, 0] == "1000"][:, 2].astype(float)
intersect10000_list = intersection_list[intersection_list[:, 0] == "10000"][:, 2].astype(float)

ax[2].set_title("Intersection list")
ax[2].hist(intersect100_list, range=(0, intersect100_list.max()), bins=50, density=True, fc=col["bl"], label="100")
ax[2].hist(intersect1000_list, range=(0, intersect1000_list.max()), bins=50, density=True, fc=col["re"], label="1000")
ax[2].hist(intersect10000_list, range=(0, intersect10000_list.max()), bins=50, density=True, fc=col["gr"], label="10000")
ax[2].legend()

difference100_list = difference_list[difference_list[:, 0] == "100"][:, 2].astype(float)
difference1000_list = difference_list[difference_list[:, 0] == "1000"][:, 2].astype(float)
difference10000_list = difference_list[difference_list[:, 0] == "10000"][:, 2].astype(float)

ax[4].set_title("Difference list")
ax[4].hist(difference100_list, range=(0, difference100_list.max()), bins=50, density=True, fc=col["bl"], label="100")
ax[4].hist(difference1000_list, range=(0, difference1000_list.max()), bins=50, density=True, fc=col["re"], label="1000")
ax[4].hist(difference10000_list, range=(0, difference10000_list.max()), bins=50, density=True, fc=col["gr"], label="10000")
ax[4].legend()

uni100_bst = union_bst[union_bst[:, 0] == "100"][:, 2].astype(float)
uni1000_bst = union_bst[union_bst[:, 0] == "1000"][:, 2].astype(float)
uni10000_bst = union_bst[union_bst[:, 0] == "10000"][:, 2].astype(float)

ax[1].set_title("Union bst")
ax[1].hist(uni100_bst, range=(0, uni100_bst.max()), bins=50, density=True, fc=col["bl"], label="100")
ax[1].hist(uni1000_bst, range=(0, uni1000_bst.max()), bins=50, density=True, fc=col["re"], label="1000")
ax[1].hist(uni10000_bst, range=(0, uni10000_bst.max()), bins=50, density=True, fc=col["gr"], label="10000")
ax[1].legend()

intersect100_bst = intersection_bst[intersection_bst[:, 0] == "100"][:, 2].astype(float)
intersect1000_bst = intersection_bst[intersection_bst[:, 0] == "1000"][:, 2].astype(float)
intersect10000_bst = intersection_bst[intersection_bst[:, 0] == "10000"][:, 2].astype(float)

ax[3].set_title("Intersection bst")
ax[3].hist(intersect100_bst, range=(0, intersect100_bst.max()), bins=50, density=True, fc=col["bl"], label="100")
ax[3].hist(intersect1000_bst, range=(0, intersect1000_bst.max()), bins=50, density=True, fc=col["re"], label="1000")
ax[3].hist(intersect10000_bst, range=(0, intersect10000_bst.max()), bins=50, density=True, fc=col["gr"], label="10000")
ax[3].legend()

difference100_bst = difference_bst[difference_bst[:, 0] == "100"][:, 2].astype(float)
difference1000_bst = difference_bst[difference_bst[:, 0] == "1000"][:, 2].astype(float)
difference10000_bst = difference_bst[difference_bst[:, 0] == "10000"][:, 2].astype(float)

ax[5].set_title("Difference bst")
ax[5].hist(difference100_bst, range=(0, difference100_bst.max()), bins=50, density=True, fc=col["bl"], label="100")
ax[5].hist(difference1000_bst, range=(0, difference1000_bst.max()), bins=50, density=True, fc=col["re"], label="1000")
ax[5].hist(difference10000_bst, range=(0, difference10000_bst.max()), bins=50, density=True, fc=col["gr"], label="10000")
ax[5].legend()
ax[5].set_xlabel("Time (s)")
fig.tight_layout()
plt.show()
