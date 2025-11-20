import numpy as np
import random

native = np.array([8.50, 9.48, 8.65, 8.16, 8.83, 7.76, 8.63])
cauc   = np.array([8.27, 8.20, 8.25, 8.14, 9.00, 8.10, 7.20, 8.32, 7.70])

pool = np.concatenate([native, cauc])
n1 = len(native)
perm = 20000

med = np.median(native) - np.median(cauc)
count = 0
random.seed(123)
for _ in range(perm):
    np.random.shuffle(pool)
    g1 = pool[:n1]
    g2 = pool[n1:]
    stat = np.median(g1) - np.median(g2)
    if abs(stat) >= abs(med):
        count += 1
p_perm = (count + 1) / (perm + 1)
print("sample median diff:", med)
print("permutation p-value:", p_perm)