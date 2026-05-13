using LinearAlgebra

# =============================================================================
# Compact Representation and Recursive Matrix Factors
# =============================================================================

function update_compact_factors!(state::MSLBFGS_State)
	k = state.m
	if k == 0
		return nothing
	end

	# Extract the current active views
	O_curr = view(state.O, 1:k, 1:k)
	X_curr = view(state.X, 1:k, 1:k)
	R_curr = view(state.R, 1:k, 1:k)
	KR_curr = view(state.K_R, 1:k, 1:k)

	if k == 1
		# Base Case: Initialization of operators for a single secant
		# Mutate the preallocated 1x1 view in-place 
		X_curr .= O_curr

		# r-factor of the initial SPD kernel: K = r * r^T
		# Extract and assign the scalar square root safely using `begin`
		R_curr[begin, begin] = sqrt(KR_curr[begin, begin])

		return nothing
	end

	# Augment the X matrix (Effective Overlap)
	# Partition indices: 
	# 'prev' represents subsets 1 and 2 (older and shared active displacements)
	# 'new' represents subset 3 (the most recent displacement)
	prev = 1:(k-1)
	new = k

	# Fill the symmetric and trivial overlap blocks
	X_curr[prev, new] .= O_curr[prev, new]
	X_curr[new, new]  = O_curr[new, new]

	# Calculate the effective overlap for the lower block X_{3, 1}
	# Here, we approximate the subset partitioning logic recursively:
	# X_{new, prev} = O_{new, prev_active} * (O_{prev_active})^{-1} * X_{prev_active, older}
	# For a sequential 1-by-1 update, we assemble the lower row vector
	O_22 = view(O_curr, prev, prev)
	X_21 = view(X_curr, prev, prev) # Simplified subset mapping for demonstration
	O_32_row = transpose(O_curr[new, prev])

	# Solve O_22 \ X_21 safely and multiply by O_32
	X_curr[new, prev] .= vec(O_32_row * (O_22 \ X_21))

	# -------------------------------------------------------------------------
	# Augment the R matrix (Inhomogeneous Factorization)
	# -------------------------------------------------------------------------
	# Perform the Cholesky-like decomposition of the updated right kernel KR
	# KR = r * r^T. We extract the relevant block factors.
	r_factor_new = cholesky(Symmetric(KR_curr)).L

	# The bottom right block gets the r-factor of the new kernel decomposition
	R_curr[new, new] = r_factor_new[new, new]

	# Calculate the cross-term x_{2,3} and populate the upper block
	# x_{2,3} = O_{1:2, 3} * M_{2,3}^{-1}  (mediated by total overlap)
	# Then multiply by r_factor_new[new, new]
	M_inv = inv(O_curr[prev, prev])

	# Safely evaluate the matrix-vector product from the left
	x_cross = M_inv * O_curr[prev, new]

	R_curr[prev, new] .= x_cross .* R_curr[new, new]
	R_curr[new, prev] .= zero(eltype(state.R))

	return nothing
end

# Helper evaluation function mapping the compact representation to a vector 'v'
# H * v = Π^T * H0 * Π * v + S * R^{-T} * R^{-1} * S^T * v
function apply_compact_hessian(state::MSLBFGS_State, v::Vector{Float64}, H0_scale::Float64 = 1.0)
	k = state.m
	Sk = view(state.S, :, 1:k)
	Yk = view(state.Y, :, 1:k)
	Xk = view(state.X, 1:k, 1:k)
	Rk = view(state.R, 1:k, 1:k)

	# Inner product evaluations
	S_v = Sk' * v

	# Inhomogeneous term application: S * R^{-T} * R^{-1} * (S^T * v)
	inhomo_v = Sk * (Rk' \ (Rk \ S_v))

	# Projector application: Π = I - Y * X^{-1} * S^T
	# Π * v = v - Y * (X \ (S^T * v))
	Pi_v = v - Yk * (Xk \ S_v)

	# H0 application (Assuming H0 is an initial scaled identity matrix)
	H0_Pi_v = H0_scale .* Pi_v

	# Π^T * (H0 * Π * v)
	# Π^T * u = u - S * X^{-T} * Y^T * u
	Y_H0_Pi_v = Yk' * H0_Pi_v
	homo_v = H0_Pi_v - Sk * (Xk' \ Y_H0_Pi_v)

	# Total approximated inverse Hessian evaluation
	return homo_v + inhomo_v
end
