# Homework 9

## How to execute (assuming gcc + Linux kernel 6.14.0-28-generic):

### Problem 1:
To compile and run the first problem, use `make run-p1 ARGS="0.34
ex/hermite/x.txt ex/hermite/fx.txt ex/hermite/dfx.txt"`. This generates a csv
table with the interpolated results of `sin(x)`.

### Problem 2:
To compile and run the second problem, use `make run-p2
ARGS="ex/spline-nat/x.txt ex/spline-nat/fx.txt"`. This generates a csv table
with the interpolated results of the duck.

### Problem 3:
To compile and run the third problem, use `make run-p3
ARGS="ex/spline-cond/x1.txt ex/spline-cond/fx1.txt 1.0 -0.67 ex/
spline-cond/table1.csv"`. This generates a csv table with the interpolated
data of first curve for the dog.
