using Optim
using LinearAlgebra

# =====================================================================
# Extended Rosenbrock Topology Definition
# =====================================================================
function extended_rosenbrock(x::Vector{Float64})
	n = length(x)
	res = 0.0
	for i in 1:2:(n-1)
		res += 100.0 * (x[i+1] - x[i]^2)^2 + (1.0 - x[i])^2
	end
	return res
end

function extended_rosenbrock_g!(g::Vector{Float64}, x::Vector{Float64})
	n = length(x)
	fill!(g, 0.0)
	for i in 1:2:(n-1)
		t1     = x[i+1] - x[i]^2
		t2     = 1.0 - x[i]
		g[i]   = -400.0 * x[i] * t1 - 2.0 * t2
		g[i+1] = 200.0 * t1
	end
	return g
end

# =====================================================================
# Isolated Validation Protocol
# =====================================================================
function execute_rosenbrock_validation()
	println("--- Commencing Isolated Validation Protocol on Extended Rosenbrock Topology ---")

	# Initialize 4-dimensional topology
	n = 4
	x0 = zeros(n)
	for i in 1:n
		x0[i] = isodd(i) ? -1.2 : 1.0
	end

	optim_options = Optim.Options(iterations = 20000, g_tol = 1e-8)

	# 1. Execute MS-LBFGS (Our Multi-Secant Geometry)
	x_ms, f_ms, iter_ms, conv_ms = optimize_mslbfgs(extended_rosenbrock, extended_rosenbrock_g!, copy(x0), 5, max_iter = 20000)

	println(">>> MS-LBFGS (Ours)")
	println("    Converged:  $conv_ms")
	println("    Iterations: $iter_ms")
	println("    Final f(x): $f_ms")
	println("    Final x:    $x_ms\n")

	# 2. Execute Standard L-BFGS Baseline (Optim.jl)
	res_lbfgs = optimize(extended_rosenbrock, extended_rosenbrock_g!, copy(x0), LBFGS(m = 5), optim_options)

	println(">>> Standard L-BFGS (Optim.jl Baseline)")
	println("    Converged:  $(Optim.converged(res_lbfgs))")
	println("    Iterations: $(Optim.iterations(res_lbfgs))")
	println("    Final f(x): $(Optim.minimum(res_lbfgs))")
	println("    Final x:    $(Optim.minimizer(res_lbfgs))\n")

	# 3. Execute Classical Exact BFGS Baseline (Optim.jl)
	res_bfgs = optimize(extended_rosenbrock, extended_rosenbrock_g!, copy(x0), BFGS(), optim_options)

	println(">>> Classical Exact BFGS (Optim.jl Baseline)")
	println("    Converged:  $(Optim.converged(res_bfgs))")
	println("    Iterations: $(Optim.iterations(res_bfgs))")
	println("    Final f(x): $(Optim.minimum(res_bfgs))")
	println("    Final x:    $(Optim.minimizer(res_bfgs))")
	println("-------------------------------------------------------------------------------")
end

execute_rosenbrock_validation()