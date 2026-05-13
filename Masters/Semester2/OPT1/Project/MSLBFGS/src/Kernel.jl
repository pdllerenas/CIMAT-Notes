using LinearAlgebra

# =============================================================================
#  Core Variational MS-BFGS Kernel Evaluations
# =============================================================================

function evaluate_dense_kernels!(state::MSLBFGS_State, enforce_exactness::Bool = false)
	m = state.m
	if m == 0
		return nothing
	end

	# Extract the active mathematical window based on current memory length m
	Sm = view(state.S, :, 1:m)
	Ym = view(state.Y, :, 1:m)
	Om = view(state.O, 1:m, 1:m)

	# Compute the current overlap matrix O = S^T Y
	mul!(Om, Sm', Ym)

	# Evaluate the asymptotic dense kernels K_R and K_L
	# Utilizing LinearAlgebra.sqrt on Symmetric matrices guarantees SPD results
	KR_dense = Matrix(sqrt(Symmetric(Om * Om')))
	KL_dense = Matrix(sqrt(Symmetric(Om' * Om)))

	# Implement optional strict exactness for the most recent secant pair
	if enforce_exactness
		# Isolate the m-th column of the overlap matrix and its transpose
		o_m = Om[:, m]
		o_tilde_m = Om'[m, :] # equivalent to O^T e_m

		# Denominator scalars: e_m^T O e_m
		omm_scalar = o_m[m]

		# Standard basis vector for the most recent index
		e_m = zeros(eltype(state.O), m)
		e_m[m] = 1.0

		# Construct the modified right kernel \tilde{K}_R
		kr_em = KR_dense[:, m]
		KR_dense .= KR_dense .- (kr_em * kr_em') ./ kr_em[m] .+ (o_m * o_m') ./ omm_scalar

		# Construct the modified left kernel \tilde{K}_L
		pi_tilde = I - (e_m * o_tilde_m') ./ omm_scalar
		KL_dense .= pi_tilde' * KL_dense * pi_tilde .+ (o_tilde_m * o_tilde_m') ./ omm_scalar
	end

	# Populate the state matrices
	state.K_R[1:m, 1:m] .= KR_dense
	state.K_L[1:m, 1:m] .= KL_dense

	return nothing
end
