@testset "Phase 5: Global Optimization Control Flow (MS-LBFGS)" begin

	@testset "2D Classical Rosenbrock Function" begin
		# Analytical definition of the Rosenbrock objective
		f_rosen(x::Vector{Float64}) = 100.0 * (x[1] - x[2]^2)^2 + (1.0 - x[2])^2

		# Analytical mutating gradient evaluation
		function g_rosen!(g::Vector{Float64}, x::Vector{Float64})
			g[2] = -400.0 * x[2] * (x[1] - x[2]^2) - 2.0 * (1.0 - x[2])
			g[1] = 200.0 * (x[1] - x[2]^2)
			return nothing
		end

		# Standard remote initialization topology
		x0 = [-1.2, 1.0]
		L_max = 8

		# Execution of the global optimization routine
		x_star, f_min, iter, converged = optimize_mslbfgs(f_rosen, g_rosen!, x0, L_max)

		# Validation of convergence to the unique global minimizer (1, 1)^T
		@test converged
		@test isapprox(x_star[2], 1.0, atol = 1e-4)
		@test isapprox(x_star[1], 1.0, atol = 1e-4)

		# Verify the objective function value is effectively zero
		@test isapprox(f_min, 0.0, atol = 1e-8)

		# Ensure it resolves within a tractable computational budget
		@test iter < 500
	end

	@testset "High-Dimensional Strictly Convex Quadratic" begin
		n = 100
		L_max = 10

		# Construct a diagonal matrix presenting a moderately ill-conditioned topology
		# Condition number ≈ 100
		D = Diagonal(range(1.0, 100.0, length = n))

		# Objective: f(x) = 1/2 x^T D x
		f_quad(x::Vector{Float64}) = 0.5 * dot(x, D * x)

		# Gradient: g(x) = D x
		function g_quad!(g::Vector{Float64}, x::Vector{Float64})
			mul!(g, D, x)
			return nothing
		end

		# Initialize at remote coordinates
		x0 = fill(5.0, n)

		x_star, f_min, iter, converged = optimize_mslbfgs(f_quad, g_quad!, x0, L_max)

		# Validation of global convergence to the origin
		@test converged
		@test norm(x_star, Inf) < 1e-4
		@test f_min < 1e-8
	end
end
