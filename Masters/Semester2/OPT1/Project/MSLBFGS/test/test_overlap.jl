@testset "Dense Kernel Evaluations" begin
	n = 5
	L = 3
	state = MSLBFGS_State(n, L)

	@testset "Empty State Return" begin
		# Test when m = 0 (should return early, no mutation)
		evaluate_dense_kernels!(state)
		@test all(state.K_R .== 0.0)
	end

	@testset "Standard Evaluation (enforce_exactness = false)" begin
		# Set m = 2 and populate S and Y with simple diagonal data
		state.m = 2
		state.S[1:2, 1:2] .= [1.0 0.0; 0.0 1.0]
		state.Y[1:2, 1:2] .= [2.0 0.0; 0.0 3.0]

		evaluate_dense_kernels!(state, false)

		# O = S^T * Y
		@test state.O[1:2, 1:2] ≈ [2.0 0.0; 0.0 3.0]

		# K_R = sqrt(O * O^T) => should be [2.0 0.0; 0.0 3.0]
		@test state.K_R[1:2, 1:2] ≈ [2.0 0.0; 0.0 3.0]

		# K_L = sqrt(O^T * O) => should be [2.0 0.0; 0.0 3.0]
		@test state.K_L[1:2, 1:2] ≈ [2.0 0.0; 0.0 3.0]

		# Ensure the rest of the preallocated memory buffer wasn't touched
		@test state.K_R[3, 3] == 0.0
	end

	@testset "Exactness Enforcement (enforce_exactness = true)" begin
		# Reset state kernels
		state.K_R .= 0.0
		state.K_L .= 0.0

		# Use slightly more complex data to ensure the projection math runs
		# without singular matrix errors or dimension mismatches
		state.m = 2
		state.S[1:2, 1:2] .= [1.0 0.5; 0.5 1.0]
		state.Y[1:2, 1:2] .= [1.5 0.2; 0.2 2.0]

		evaluate_dense_kernels!(state, true)

		# Check for numerical blowups
		@test !any(isnan.(state.K_R[1:2, 1:2]))
		@test !any(isnan.(state.K_L[1:2, 1:2]))

		# The resulting kernels should theoretically remain symmetric
		# We use a relative tolerance (rtol) to account for floating-point drift
		@test state.K_R[1:2, 1:2] ≈ state.K_R[1:2, 1:2]' rtol=1e-8
		@test state.K_L[1:2, 1:2] ≈ state.K_L[1:2, 1:2]' rtol=1e-8
	end
end
