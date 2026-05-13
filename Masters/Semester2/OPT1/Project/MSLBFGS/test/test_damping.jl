@testset "Secant Truncation and Symmetric Damping" begin
	n = 2
	L = 3

	# We will use simple Identity matrices for the approximations during tests
	B_approx = Matrix{Float64}(I, n, n)
	H_approx = Matrix{Float64}(I, n, n)

	@testset "Stable Overlap (No Truncation)" begin
		state = MSLBFGS_State(n, L)
		state.m = 2

		# Perfect, strongly positive definite secants
		state.S[1:2, 1:2] .= [1.0 0.0; 0.0 1.0]
		state.Y[1:2, 1:2] .= [2.0 0.0; 0.0 2.0]

		enforce_overlap_stability!(state, B_approx, H_approx)

		# The bounds check should pass easily; m should remain 2
		@test state.m == 2
	end

	@testset "Unstable Overlap (Forces Truncation)" begin
		state = MSLBFGS_State(n, L)
		state.m = 2

		# Standard positive definite secants
		state.S[1:2, 1:2] .= [1.0 0.0; 0.0 1.0]
		state.Y[1:2, 1:2] .= [2.0 0.0; 0.0 2.0]

		# Force the truncation by demanding absurdly high epsilon stability bounds
		impossible_eps = 1000.0

		# The loop should fail the condition and decrement m
		enforce_overlap_stability!(state, B_approx, H_approx, impossible_eps, impossible_eps)

		# Since it truncates until m=1, it should drop to 1
		@test state.m == 1
	end

	@testset "Symmetric Damping Execution (m = 1 limit)" begin
		state = MSLBFGS_State(n, L)
		state.m = 1

		# Setup orthogonal vectors s and y so that dot(s, y) = 0.
		# This violently violates the secant condition s^T y > 0, guaranteeing
		# that the absolute value check triggers the damping subproblem.
		state.S[:, 1] .= [1.0, 0.0]
		state.Y[:, 1] .= [0.0, 1.0]

		# Keep a copy of the original vectors to prove they were mutated
		s_orig = copy(state.S[:, 1])
		y_orig = copy(state.Y[:, 1])

		# Run with standard epsilons
		enforce_overlap_stability!(state, B_approx, H_approx)

		@test state.S[:, 1] != s_orig
		@test state.Y[:, 1] != y_orig

		@test state.S[:, 1] ≈ [0.997133, 0.002866] atol=1e-5
		@test state.Y[:, 1] ≈ [0.002866, 0.997133] atol=1e-5

	end
end
