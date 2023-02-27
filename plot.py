from pathlib import Path

import numpy as np
# import matplotlib
# matplotlib.use('TkAgg')
from matplotlib import pyplot as plt


# Set all paths
root = Path(".").resolve()
data = root / "benchmarks"
bst = data / "2500_bst.csv"
ll = data / "2500_list.csv"
plots = root / "plots"


def ma_mask(x, lenght):
    return np.convolve(x, np.ones((lenght, )) / lenght, mode="same")


def compare_implementations(n, oper_bst, oper_ll, oper_name, filter_lenght=10):
    """Compares two implementations from data given in oper_bst or oper_ll.

    Args
    ----
        n (numpy.ndarray): x-axis representation.
        oper_bst (numpy.ndarray): y-axis from bst implementation.
        oper_ll (numpy.ndarray): y-axis from linked list implementation.
        oper_name (str): Name of operation.
        filter_length (int): Length of moving average filter.
    """

    fname = oper_name + "_compared.png"

    fig, axs = plt.subplots(2, 2, figsize=(8, 8), sharex=True)

    axs[0, 0].set_title("Comparison")
    axs[0, 0].plot(n, oper_bst, label="bst")
    axs[0, 0].plot(n, oper_ll, label="linked list")

    axs[0, 1].set_title("BST")
    axs[0, 1].plot(n, oper_bst, label="bst", linewidth=2.5)
    axs[0, 1].plot(n, ma_mask(oper_bst, filter_lenght),
                   label=f"$MA(bst, {filter_lenght})$",
                   color="C3", linewidth=1)

    axs[1, 0].set_title("Linked list")
    axs[1, 0].plot(n, oper_ll, color="C1", label="linked list", linewidth=2.5)
    axs[1, 0].plot(n, ma_mask(oper_ll, filter_lenght),
                   label=f"$MA(linked list, {filter_lenght})$",
                   color="C9", linewidth=1)

    axs[1, 1].set_title("Difference")
    axs[1, 1].plot(n, oper_bst - oper_ll, color="C2",
                   label="bst - linked list")
    axs[1, 1].grid()

    for ax in axs.flatten():
        ax.set_xlabel("n (Tries to add elements)")
        ax.set_ylabel("time (s)")
        ax.legend()

    fig.tight_layout()
    plt.savefig(plots / fname)


def best_fit(n, oper_data, oper_name, degree):

    coefficients = np.polyfit(n, oper_data, degree)
    polynomial = np.poly1d(coefficients)

    fname = oper_name + "_interp.png"
    poly_str = "+".join(f"{c:.2e}x^{len(coefficients) - i}" for i,
                        c in enumerate(coefficients, start=1))

    plt.title(oper_name + " with interpolation.")
    plt.plot(n, oper_data, label="True data")
    plt.plot(n, polynomial(n), label=f"Interpolation: $p(x) = {poly_str}$")
    plt.legend()
    # plt.savefig(plots / fname)
    plt.show()


params = {
    "skip_header": True,
    "delimiter": ","
}

# Read only headers from csv.
headers = np.genfromtxt(bst, max_rows=1, dtype=str, delimiter=",")

# Read data from csv's.
bst_data = np.genfromtxt(bst, **params)
ll_data = np.genfromtxt(ll, **params)

n = bst_data[:, 0]

for i, oper_name in enumerate(headers[1:], start=1):
    compare_implementations(n, bst_data[:, i], ll_data[:, i], oper_name)
    # best_fit(n, bst_data[:, i], oper_name, 2)
    # best_fit(n, ll_data[:, i], oper_name, 2)
