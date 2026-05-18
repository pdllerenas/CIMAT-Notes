================================================================================
MULTI-SECANT LIMITED-MEMORY BFGS (MS-LBFGS)
================================================================================

Authors: Gerardo Rocha, Pedro Llerenas
Language: Julia
Reference: "A Multi-Secant Limited-Memory BFGS Method" by F.V. Gubarev

DESCRIPTION
-----------
This repository contains a native Julia implementation of the Multi-Secant 
Limited-Memory BFGS (MS-LBFGS) optimization algorithm. The method generalizes 
the classic L-BFGS approach by adaptively maintaining multiple secant conditions 
to build a positive-definite Hessian approximation. 

To ensure numerical stability across highly ill-conditioned and pathological 
topologies, this implementation includes dynamic secant truncation, a symmetric 
1D damping scheme, and a mathematically rigorous compact representation to 
reduce computational overhead to O(m^3).

This project requires Julia and depends on several packages, most notably 
CUTEst.jl and Optim.jl (for baseline comparisons). 

An automated setup script is provided to construct the environment and 
download the necessary Fortran SIF problem collection.

RUNNING THE TESTS
-----------------
To verify the mathematical integrity of the solver, run the test suite:
julia --project=. test/runtests.jl

RUNNING THE BENCHMARKS
----------------------
To benchmark the MS-LBFGS solver against the standard L-BFGS-B (Optim.jl) 
baseline using the CUTEst dataset, run the performance profile script. 

The script will automatically select a randomized, time-seeded subset of 
unconstrained problems and generate a Dolan-Moré Performance Profile plot.

julia --project=. src/GenerateProfile.jl