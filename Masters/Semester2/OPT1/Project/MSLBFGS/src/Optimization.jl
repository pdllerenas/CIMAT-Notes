# =====================================================================
# Strong Wolfe Conditions Evaluator
# =====================================================================
function check_strong_wolfe(f_val_new::Float64, f_val_old::Float64,
	g_new::Vector{Float64}, g_old::Vector{Float64},
	p::Vector{Float64}, alpha::Float64;
	c1::Float64 = 1e-4, c2::Float64 = 0.9)

	dir_deriv_old = dot(g_old, p)

	# Verify the sufficient decrease (Armijo) condition
	if f_val_new > f_val_old + c1 * alpha * dir_deriv_old
		return false
	end

	# Verify the strong curvature condition
	dir_deriv_new = dot(g_new, p)
	if abs(dir_deriv_new) > c2 * abs(dir_deriv_old)
		return false
	end

	return true
end

# =====================================================================
# Main Global Optimization Wrapper (MS-LBFGS)
# =====================================================================
function optimize_mslbfgs(f::Function, g!::Function, x0::Vector{Float64}, L_max::Int = 8;
	max_iter::Int = 20000,
	eps_g::Float64 = 1e-8, eps_g_min::Float64 = 1e-4, eps_g_max::Float64 = 1.0)

	n = length(x0)
	state = MSLBFGS_State(n, L_max)

	x = copy(x0)
	g = zeros(n)
	g!(g, x)

	g0_norm = norm(g, Inf)
	tau_convergence = min(max(eps_g * max(1.0, g0_norm), eps_g_min), eps_g_max)

	iter = 0
	reset_search = true

	# Global convergence loop relying strictly on gradient tolerance and iterations
	while norm(g, Inf) > tau_convergence && iter < max_iter

		# ==========================
		# Compute Search Direction
		# ==========================
		if state.m == 0 || reset_search
			d = -g
		else
			s_recent = view(state.S, :, state.m)
			y_recent = view(state.Y, :, state.m)

			sty = dot(s_recent, y_recent)
			yty = dot(y_recent, y_recent)

			H0_k = yty > 1e-14 ? sty / yty : 1.0

			try
				d = -apply_compact_hessian(state, g, H0_k)
			catch
				# If inverse Hessian application fails, fallback to Steepest Descent
				d = -g
				state.m = 0
				reset_search = true
			end
		end

		# =======================
		# Line Search Execution 
		# =======================
		alpha = 1.0
		x_new = copy(x)
		g_new = zeros(n)

		f_val = f(x)
		g_dot_d = dot(g, d)

		step_success = false
		alpha_min = 0.0
		alpha_max = Inf

		# 1D Bracketing and Bisection Zoom Phase
		for ls_iter in 1:40
			x_new .= x .+ alpha .* d
			f_new = f(x_new)

			# Guard against numerical overflow in severe non-convexities
			if isnan(f_new) || isinf(f_new)
				alpha_max = alpha
				alpha *= 0.5
				continue
			end

			# Evaluate the trial gradient to compute the new directional derivative
			g!(g_new, x_new)

			# Unconditionally test the exact Strong Wolfe limits
			if check_strong_wolfe(f_new, f_val, g_new, g, d, alpha, c1 = 1e-4, c2 = 0.9)
				step_success = true
				break
			else
				# Constraint violation logic: Determine proper bracketing interval
				dir_deriv_new = dot(g_new, d)
				if f_new > f_val + 1e-4 * alpha * g_dot_d || dir_deriv_new > 0
					# Violates sufficient decrease OR overshoots local minimum
					alpha_max = alpha
					alpha = 0.5 * (alpha_min + alpha_max)
				else
					# Violates curvature condition; optimal alpha strictly lies to the right
					alpha_min = alpha
					if isinf(alpha_max)
						alpha *= 2.0 # Aggressive extrapolation
					else
						alpha = 0.5 * (alpha_min + alpha_max)
					end
				end
			end
		end

		if !step_success
			state.m = 0
			reset_search = true
			continue
		end
		reset_search = false

		# =====================================================================
		# Update Displacements
		# =====================================================================
		s_k = x_new - x
		y_k = g_new - g

		sty = dot(s_k, y_k)

		# Secant Safeguard
		if sty > 1e-10 * norm(s_k) * norm(y_k) && norm(s_k) > 1e-12

			m_next = min(state.m + 1, state.L)
			if state.m == state.L
				state.S[:, 1:(end-1)] .= state.S[:, 2:end]
				state.Y[:, 1:(end-1)] .= state.Y[:, 2:end]
			end
			state.S[:, m_next] .= s_k
			state.Y[:, m_next] .= y_k
			state.m = m_next

			try
				enforce_overlap_stability!(state, Matrix{Float64}(I, n, n), Matrix{Float64}(I, n, n))
				update_compact_factors!(state)
			catch e
				break
			end
		else
			reset_search = true
			state.m = 0
		end

		x .= x_new
		g .= g_new
		iter += 1
	end

	return x, f(x), iter, norm(g, Inf) <= tau_convergence
end
