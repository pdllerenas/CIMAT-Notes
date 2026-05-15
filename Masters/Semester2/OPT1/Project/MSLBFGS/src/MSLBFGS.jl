module MSLBFGS

using LinearAlgebra

export MSLBFGS_State
export evaluate_dense_kernels!
export enforce_overlap_stability!
export update_compact_factors!
export apply_compact_hessian
export optimize_mslbfgs
export check_strong_wolfe


include("State.jl")
include("Kernel.jl")
include("Damping.jl")
include("CompactRepresentation.jl")
include("Optimization.jl")

end