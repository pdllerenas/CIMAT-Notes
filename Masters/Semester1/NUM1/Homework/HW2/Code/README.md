# Homework 2

## How to execute (assuming gcc + Linux kernel 6.14.0-28-generic):

### Problem 1:
To compile the code for the first problem, use `gcc p1.c -lm -o p1` and execute using `./p1`. This will print to `stdout` a list of terms approximating the constant `e`. Also, `error.csv` is created, which contains the absolute and relative errors for each of the terms calculated.

### Problem 2:
To compile the code for the second problem, we use `gcc p2.c -o p2` and execute using `./p1`. This will print out the number of iterations needed to reach convergence (tolerance dependent), and the root that was found.

### Problem 3:
To compile the code for the third problem, we use `gcc p3.c -lm -o p3` and execute using `./p3`. This will print to `stdout` if the fixed point iteration method was able to find a fixed point for each given function. If the method failed, an error will be printed out. Otherwise, the root found is printed. Moreover, the file `p3.csv` is created, where the first 20 iterations of the method applied to each function is displayed.

### Problem 4:
To compile the code for the fourth problem, we use `gcc p4.c -lm -o p4` and execute using `./p4`. This will print to `stdout` if each of the three methods applied failed. Otherwise, it prints the root found.
