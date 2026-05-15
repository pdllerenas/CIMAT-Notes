#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "===================================================="
echo "    MS-LBFGS Automated Benchmark Runner"
echo "===================================================="

# 1. Check if Julia is installed; if not, install it silently
if ! command -v julia &> /dev/null; then
    echo "[1/4] Julia is not installed. Installing via Juliaup..."
    # The -s -- -y flags force a silent installation without user prompts
    curl -fsSL https://install.julialang.org | sh -s -- -y
    
    # Add Julia to the PATH for the current shell session
    export PATH="$HOME/.juliaup/bin:$PATH"
    echo "Julia installation complete."
else
    echo "[1/4] Julia is already installed."
fi

julia --version

# 2. Configure the local Julia environment and install packages
echo "[2/4] Installing required Julia libraries..."
julia --project=. -e '
    using Pkg
    
    dependencies = [
        "CUTEst", "NLPModels", "Optim", 
        "BenchmarkProfiles", "Plots", "Revise"
    ]
    
    println("Adding packages to local environment...")
    Pkg.add(dependencies)
'

# 3. Apply the CUTEst patch and download the Fortran SIF dataset
echo "[3/4] Downloading CUTEst mathematical problem database..."
julia --project=. -e '
    using Pkg
    
    println("Applying MASTSIF patch to prevent Fortran crash...")
    ENV["MASTSIF"] = ""
    
    println("Precompiling environment...")
    Pkg.precompile()
    
    println("Fetching the SIF collection...")
    using CUTEst
    CUTEst.set_mastsif("sifcollection")
'

# 4. Run your actual benchmark script
# NOTE: Make sure your julia script is named "benchmark.jl" in the same folder!
echo "[4/4] Environment ready. Executing MS-LBFGS Benchmark..."
echo "===================================================="

# Run the benchmark script within the local project environment
julia --project=. GenerateProfile.jl

echo "===================================================="
echo "✅ Benchmark script finished executing!"