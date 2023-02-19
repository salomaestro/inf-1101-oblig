from pathlib import Path

import numpy as np
from matplotlib import pyplot as plt

# Set all paths
root = Path(".").resolve()
data = root / "benchmarks"
bst = data / "2500_bst.csv"
ll = data / "2500_list.csv"
plots = root / "plots"


def compare_implementations(n, oper_bst, oper_ll, oper_name, filter=None):
    """Compares two implementations from data given in oper_bst or oper_ll.

    Args
    ----
        n (numpy.ndarray): x-axis representation.
        oper_bst (numpy.ndarray): y-axis from bst implementation.
        oper_ll (numpy.ndarray): y-axis from linked list implementation.
        oper_name (str): Name of operation.
        filter (numpy.ndarray): Optional, filter (use numpy.ones(filter_size)
                                                  for moving average filter.)
    """

    # optional filter of signal.
    if filter is not None:
        oper_bst = np.convolve(oper_bst, filter, mode="same")
        oper_ll = np.convolve(oper_ll, filter, mode="same")

    fname = oper_name + "_compared_masked.png"

    plt.title(oper_name + " compared.")
    plt.plot(n, oper_bst, label="bst")
    plt.plot(n, oper_ll, label="linked list")
    plt.legend()
    # plt.savefig(plots / fname)
    plt.show()


def best_fit(n, oper_data, oper_name, degree):

    coefficients = np.polyfit(n, oper_data, degree)
    polynomial = np.poly1d(coefficients)

    fname = oper_name + "_interp.png"

    plt.title(oper_name + " with interpolation.")
    plt.plot(n, oper_data, label="True data")
    plt.plot(n, polynomial(n), label=f"Interpolation: $\deg(p(x)) = {degree}$")
    plt.legend()
    # plt.savefig(plots / fname)
    plt.show()

# def prompt_for_interpolation()

params = {
    "skip_header": True,
    "delimiter": ","
}

# Read only headers from csv.
headers = np.genfromtxt(bst, max_rows=1, dtype=str, delimiter=",")

# Read data from csv's.
bst_data = np.genfromtxt(bst, **params)
ll_data = np.genfromtxt(ll, **params)

# Optional filter.
filter = np.ones(5)

n = bst_data[:, 0]

for i, oper_name in enumerate(headers[1:], start=1):
    compare_implementations(n, bst_data[:, i], ll_data[:, i], oper_name)
    # compare_implementations(n, bst_data[:, i], ll_data[:, i], oper_name, filter)
