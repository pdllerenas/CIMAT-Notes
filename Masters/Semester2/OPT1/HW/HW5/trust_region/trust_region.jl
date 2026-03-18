using LinearAlgebra
using Printf
"""
  (f::Function, g::Function, H::Function, x₀::Vector, Δ::Float64, Δ₀::Float64, η::Float64, step_finder::Function)

# Arguments
- `f::Function`: Objective function
- `g::Function`: Gradient of objective function
- `H::Function`: Hessian of objective function
- `x₀::Vector`: Initial iteration point
- `Δ::Float64`: Max trust region
- `Δ₀::Float64`: Initial trust region
- `η::Float64`: Trust region 
- `step_finder::Function`: Step finding function
- `tol::Float64`: tolerance
- `max_iter::Int`: maximum iterations allowed

# Returns
- `xₖ`: found minimizer
- `f(xₖ)`: function at minimizer
- `k`: iterations
"""
function trust_region(f, g, H, x₀, Δ, Δ₀, η, step_finder; tol = 1e-5, max_iter = 1000)
	if Δ <= 0
		error("trust region radius must be positive")
	end
	if (Δ₀ <= 0 || Δ₀ >= Δ)
		error("initial trust region must be positive and less than Δ")
	end
	if (η < 0 || η > 0.25)
		error("invalid η, must be in [0,0.25]")
	end

	xₖ = copy(x₀)
	Δₖ = Δ₀

	f_hist = Float64[]
	g_norm_hist = Float64[]
	Δ_hist = Float64[]

	for k in 1:max_iter
		gₖ = g(xₖ)
		fₖ = f(xₖ)
		norm_g = norm(gₖ)

		push!(f_hist, fₖ)
		push!(g_norm_hist, norm_g)
		push!(Δ_hist, Δₖ)

		if norm_g < tol
			return xₖ, f_hist, g_norm_hist, Δ_hist
		end

		Bₖ = H(xₖ)
		pₖ = step_finder(gₖ, Bₖ, Δₖ)

		act_red = fₖ - f(xₖ + pₖ)
		pred_red = -(dot(gₖ, pₖ) + 0.5 * dot(pₖ, Bₖ * pₖ))
		ρₖ = act_red / pred_red

		if ρₖ < 0.25
			Δₖ *= 0.25
		elseif ρₖ >= 0.75 && isapprox(norm(pₖ), Δₖ, atol = 1e-4)
			Δₖ = min(2 * Δₖ, Δ)
		end

		if ρₖ > η
			xₖ += pₖ
		end
	end

	return xₖ, f_hist, g_norm_hist, Δ_hist
end
