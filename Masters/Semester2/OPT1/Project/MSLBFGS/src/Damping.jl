using LinearAlgebra

# =============================================================================
# Dynamic Secant Truncation and Symmetric Damping
# =============================================================================

function enforce_overlap_stability!(state::MSLBFGS_State, B_approx::Matrix, H_approx::Matrix, eps_s::Float64 = 1e-2, eps_y::Float64 = 1e-3)

	# Secant Truncation Loop
	while state.m > 1
		# Re-evaluate dense kernels for the current memory length
		evaluate_dense_kernels!(state, false)

		Sm = view(state.S, :, 1:state.m)
		Ym = view(state.Y, :, 1:state.m)
		KR_m = view(state.K_R, 1:state.m, 1:state.m)
		KL_m = view(state.K_L, 1:state.m, 1:state.m)

		# Evaluate the stability bounding metrics
		det_KR = det(KR_m)
		det_S_B_S = det(Sm' * B_approx * Sm)

		inv_trace_KL = 1.0 / tr(inv(KL_m))
		tr_Y_H_Y = tr(Ym' * H_approx * Ym)

		# Verify sufficient non-singularity
		if (det_KR >= eps_s * det_S_B_S) && (inv_trace_KL >= eps_y * tr_Y_H_Y)
			break
		else
			# Discard the oldest secant constraint by shifting the history arrays left
			for i in 1:(state.m-1)
				state.S[:, i] .= state.S[:, i+1]
				state.Y[:, i] .= state.Y[:, i+1]
			end
			state.m -= 1
		end
	end

	# Symmetric Damping for the limiting m = 1 case
	if state.m == 1
		s = view(state.S, :, 1)
		y = view(state.Y, :, 1)

		Bs = B_approx * s
		Hy = H_approx * y
		sBs = dot(s, Bs)
		yHy = dot(y, Hy)
		sty = dot(s, y)

		sty_sign = sign(sty)
		sty_sign = sty_sign == 0 ? 1.0 : sty_sign # Safe default for exactly zero

		# Check limiting 1D eigenvalue bounds
		if abs(sty) < max(eps_s * sBs, eps_y * yHy)

			# Setup for 2D Damping Optimization Subproblem: min theta_s^2 + theta_y^2
			# Analytical mapping for variables x_a = theta_s, x_b = theta_y
			a_0 = sqrt(eps_s) .* Bs
			b_0 = sty_sign * sqrt(eps_y) .* Hy
			Delta = b_0 .- a_0
			epsilon = sqrt(eps_s * eps_y)

			# Invoke 2D Newton Solver for quadratic objective over box bounds [0, 1/2]^2
			theta_s, theta_y = solve_2d_damping_subproblem(a_0, b_0, Delta, epsilon)

			# Apply the minimal symmetric deformation
			s .= (1.0 - theta_s) .* s .+ sty_sign * theta_s .* Hy
			y .= (1.0 - theta_y) .* y .+ sty_sign * theta_y .* Bs
		end
	end
	evaluate_dense_kernels!(state, false)

	return nothing
end

function solve_2d_damping_subproblem(a_0, b_0, Delta, epsilon)
	# Project the 2D optimization onto the diagonal theta_s = theta_y = theta.
	# At the upper bound theta = 0.5, the vectors become identical (a = b)
	# and the constraint a^T b >= epsilon * max(|a|^2, |b|^2) is unconditionally
	# satisfied because epsilon < 1.0.

	theta_low = 0.0
	theta_high = 0.5
	theta_opt = 0.5 

	# 1D Bisection loop to find the minimal symmetric deformation
	# 20 iterations yield precision well beyond floating-point requirements
	for _ in 1:20
		theta_mid = 0.5 * (theta_low + theta_high)

		# Symmetrically deform the vectors
		a_mid = a_0 .+ theta_mid .* Delta
		b_mid = b_0 .- theta_mid .* Delta

		# Evaluate the geometric cross-correlations
		a_dot_b = dot(a_mid, b_mid)
		max_norm_sq = max(dot(a_mid, a_mid), dot(b_mid, b_mid))

		# Verify the eigenvalue-bounding inequality
		if a_dot_b >= epsilon * max_norm_sq
			# The constraint is satisfied; attempt to find a strictly smaller deformation
			theta_opt = theta_mid
			theta_high = theta_mid
		else
			# The constraint is violated; a more aggressive deformation is strictly required
			theta_low = theta_mid
		end
	end

	return theta_opt, theta_opt
end
