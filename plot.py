import numpy as np
from matplotlib import pyplot as plt
from pathlib import Path

root = Path(".").resolve()
data = root / "benchmarks"
bst = data / "2500_bst.csv"
ll = data / "2500_list.csv"
plots = root / "plots"


def compare_implementations(n, oper_bst, oper_ll, oper_name, filter=None):
    if filter is not None:
        oper_bst = np.convolve(oper_bst, filter, mode="same")
        oper_ll = np.convolve(oper_ll, filter, mode="same")

    fname = oper_name + "_compared.png"

    plt.title(oper_name + " compared.")
    plt.plot(n, oper_bst, label="bst")
    plt.plot(n, oper_ll, label="linked list")
    plt.legend()
    plt.savefig(plots / fname)
    plt.show()


params = {
    "skip_header": True,
    "delimiter": ","
}

headers = np.genfromtxt(bst, max_rows=1, dtype=str, delimiter=",")

bst_data = np.genfromtxt(bst, **params)
ll_data = np.genfromtxt(ll, **params)

filter = np.ones(5)

n = bst_data[:, 0]

for i, oper_name in enumerate(headers[1:], start=1):
    compare_implementations(n, bst_data[:, i], ll_data[:, i], oper_name)
    # compare_implementations(n, bst[:, i], ll[:, i], oper_name, filter)
