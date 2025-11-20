import random
import matplotlib.pyplot as plt
import sys

random.seed(112025)

"""
@param p probability associated to the Bernoulli experiment
@param n number of experiments to process

@return list of outcomes
"""
def bernoulli(p: float, n: int) -> list[bool]:
    experiments = []
    for i in range(n):
        experiments.append(0 if random.random() < p else 1)
    return experiments

"""
@param chain list of booleans

@return tuple of changes from 0 -> 1 or 1 -> 0, and length of longest sub-chain of the same character
"""
def stats(chain: list[bool]) -> tuple[int, int]:
    if not chain:
        return 0,0

    changes = 0
    longest = 1
    count = 1

    for i in range(1, len(chain)):
        if chain[i-1] != chain[i]:
            changes += 1
            longest = max(count, longest)
            count = 1
        else:
            count += 1

    longest = max(longest, count)
    return (changes, longest)

def main():
    bits1 = [
    1,0,0,1,0,0,1,0,1,0,
    1,0,1,0,1,0,1,0,1,0,
    0,0,1,0,1,0,1,0,0,1,
    0,0,0,1,0,0,0,0,1,1,
    1,1,1,0,1,0,0,1,0,1,
    0,0,1,0,1,0,1,0,1,0,
    1,1,0,0,1,0,0,1,0,0,
    1,0,1,0,0,1,0,0,1,1,
    1,1,0,1,0,1,0,1,0,1,
    0,1,0,1,1,1,1,1,1,1,
]

    bits2 = [
    0,1,1,0,1,1,1,0,1,0,
    0,1,0,1,1,0,1,1,0,0,
    1,0,0,0,1,1,1,1,1,0,
    0,1,0,1,1,0,1,1,1,1,
    0,0,0,1,0,1,1,0,0,0,
    1,0,1,0,0,1,0,0,0,0,
    1,1,1,1,1,1,0,0,1,0,
    0,1,0,1,0,1,0,1,1,0,
    1,0,1,0,1,0,0,1,0,1,
    0,1,1,0,1,0,0,0,1,1,
]
    fig, ax = plt.subplots()
    dist = []
    for i in range(1000):
        chain = bernoulli(0.5, 100)
        x, y = stats(chain)
        dist.append((x,y))
        ax.scatter(x, y, color = "black", alpha = 0.1)

    x, y = stats(bits1)
    ax.scatter(x, y, color = "red", alpha = 1)

    p1 = sum(1 for a, b in dist if a == x and b == y) / len(dist)
    print("Probability of first string given:", p1)

    x, y = stats(bits2)
    ax.scatter(x, y, color = "blue", alpha = 1)

    p2 = sum(1 for a, b in dist if a == x and b == y) / len(dist)
    print("Probability of second string given:", p2)

    
    
    ax.set_xlabel("Number of swaps")
    ax.set_ylabel("Longest run length")
    ax.set_title("Swaps vs Longest Run for Bernoulli Chains")

    plt.show()

if __name__ == "__main__":
    main()