# =============================================================================
# Global Optimization Control Flow and Line Search Integration
# =============================================================================

function optimize_mslbfgs(f::Function, g!::Function, x0::Vector{Float64}, L_max::Int = 8;
	max_iter::Int = 10000,
	eps_g::Float64 = 1e-8, eps_g_min::Float64 = 1e-4, eps_g_max::Float64 = 1.0)
	n = length(x0)
	state = MSLBFGS_State(n, L_max)

	x = copy(x0)
	g = zeros(n)
	g!(g, x)

	g0_norm = norm(g, Inf)

	# Established convergence threshold based on the gradient infinity norm
	tau_convergence = min(max(eps_g * max(1.0, g0_norm), eps_g_min), eps_g_max)

	iter = 0
	reset_search = true # Flag to trigger Goldstein rules on trivial Hessian

	while norm(g, Inf) > tau_convergence && iter < max_iter
		# Compute Search Direction
		if state.m == 0 || reset_search
			d = -g # Steepest descent for initial step or post-failure reset
		else
			s_recent = view(state.S, :, state.m)
			y_recent = view(state.Y, :, state.m)
			
			sty = dot(s_recent, y_recent)
			yty = dot(y_recent, y_recent)
			
			# Calculate the scale factor gamma_k, safeguarded against division by zero
			H0_k = yty > 1e-14 ? sty / yty : 1.0
			
			# Pass the scale factor into the compact representation
			d = -apply_compact_hessian(state, g, H0_k)
		end

		# Line Search Execution
		alpha = 1.0 # Unit initial step size
		x_new = copy(x)
		f_val = f(x)
		g_dot_d = dot(g, d)

		# Backtracking Safeguard & Armijo / Goldstein logic
		step_success = false
		while alpha > 1e-10
			x_new .= x .+ alpha .* d
			f_new = f(x_new)

			# Backtracking safeguard for undefined/infinite responses
			if isnan(f_new) || isinf(f_new)
				alpha *= 0.5
				continue
			end

			if reset_search
				# Evaluate Goldstein conditions for initial step scaling
				# (Conceptual placeholder: normally checks lower and upper bounding lines)
				if f_new <= f_val + 0.1 * alpha * g_dot_d
					step_success = true
					break
				end
			else
				# Evaluate Armijo condition with guarded quadratic guessing
				if f_new <= f_val + 1e-4 * alpha * g_dot_d
					step_success = true
					break
				end
			end

			# Guarded quadratic guessing parameter reduction
			alpha *= 0.5
		end

		if !step_success
			# Enforced reset upon line search failure
			state.m = 0
			reset_search = true
			continue
		end
		reset_search = false

		# Update Gradients and Displacements
		g_new = zeros(n)
		g!(g_new, x_new)

		s_k = x_new - x
		y_k = g_new - g

		# Integrate into State Memory (Circular shift/append logic)
		m_next = min(state.m + 1, state.L)
		# Shift old columns left if memory is full
		if state.m == state.L
			state.S[:, 1:(end-1)] .= state.S[:, 2:end]
			state.Y[:, 1:(end-1)] .= state.Y[:, 2:end]
		end
		state.S[:, m_next] .= s_k
		state.Y[:, m_next] .= y_k
		state.m = m_next

		# Invoke Overlap Stability
		# B_approx and H_approx could be initialized dynamically; using Identity scaling for base
		enforce_overlap_stability!(state, Matrix{Float64}(I, n, n), Matrix{Float64}(I, n, n))

		# Formulate Compact Representation
		update_compact_factors!(state)

		# Advance Iteration
		x .= x_new
		g .= g_new
		iter += 1
	end

	return x, f(x), iter, norm(g, Inf) <= tau_convergence
end
