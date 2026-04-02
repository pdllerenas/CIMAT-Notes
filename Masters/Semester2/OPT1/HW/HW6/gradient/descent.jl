"""
  backtracking_armijo(f, g_val, x, p; α₀=1.0, ρ=0.5, c₁=1e-4)

Finds the step size α satisfying the Armijo condition.

# Arguments
- `f`: Objective function
- `g_val`: Pre-computed gradient vector at x
- `x`: Current point
- `p`: Descent direction
- `α₀`: Initial step size
- `ρ`: Contraction factor (often 0.5)
- `c₁`: Sufficient decrease parameter (often 1e-4)
"""
function backtracking_armijo(f, g_val, x, p; α₀ = 1.0, ρ = 0.5, c₁ = 1e-4)
	α = α₀
	f_x = f(x)

	dir_deriv = dot(g_val, p)

	while (f(x + α * p) > f_x + c₁ * α * dir_deriv)
		α *= ρ

		if α < 1e-10
			println("α became too small. Breaking line search.")
			break
		end
	end

	return α
end

"""
  steepest_descent_armijo(f, g, x₀; tol=1e-5, max_iter=1000)

Minimizes an objective function using Steepest Descent and Armijo line search.
"""
function steepest_descent_armijo(f, g, x₀; tol = 1e-5, max_iter = 1000)
	xₖ = copy(x₀)
	f_hist = Float64[]
	g_norm_hist = Float64[]

	for k in 1:max_iter
		gₖ = g(xₖ)
		fₖ = f(xₖ)
		norm_g = norm(gₖ)

		push!(f_hist, fₖ)
		push!(g_norm_hist, norm_g)

		if norm_g < tol
			println("Converged in $k iterations.")
			return xₖ, f_hist, g_norm_hist
		end

		pₖ = -gₖ
		α_guess = min(1.0, 1.0 / norm(gₖ))

		αₖ = backtracking_armijo(f, gₖ, xₖ, pₖ, α₀ = α_guess)

		xₖ += αₖ * pₖ
	end

	println("Maximum iterations ($max_iter) reached.")
	return xₖ, f_hist, g_norm_hist
end

function basic_gradient_descent(g, u0; alpha = 0.0001, max_iter = 15000)
	u = copy(u0)
	
	for k in 1:max_iter
			grad_E = g(u, u0)
			
			u .= u .- alpha .* grad_E
	end
	
	return u
end
