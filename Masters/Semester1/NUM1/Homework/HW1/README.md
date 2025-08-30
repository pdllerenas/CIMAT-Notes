# Homework 1

## How to execute (assuming gcc + Linux kernel 6.14.0-28-generic):

### Problem 1:
Binary representation system exercise consists of file `p1.c`. To compile, simply use `gcc p1.c -o p1` and run using `./p1`.
This will generate 2 `csv` files, one with a list of the integer number system named `binary.csv`, and another one presenting the fractional number system, named `fractions.csv`.

### Problem 2:
For the approximation of pi, we have 2 files: `p2.c` and `p2.py`. The former file is compiled using `gcc p2.c -lm -o p2`, where we have used `-lm` for the <math.h> library, and ran using `./p2 n_terms`, where `n_terms` are the number of terms to calculate. This will generate 2 csv files; one containing the sequence as-is, that is, a naive approach, named `naive_p2.csv`, and another one using a rewritten approach to the sequence, names `p2.csv`. We can plot these values using `p2.py`, which requires some libraries. To install them, (if needed, enter a virtual environment) use `pip install -r requirements.txt`, then use `python3 p2.py [naive | non-naive]` to plot the naive or non-naive sequence. 

### Problem 3:
To find the machine epsilon, first compile the file `p3.c` using `gcc p3.c -o p3` and execute using `./p3`. This will print the values of each step, and the last printed value is the machine epsilon.
