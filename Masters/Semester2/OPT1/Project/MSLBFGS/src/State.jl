# =============================================================================
# MS-LBFGS Data Structures
# =============================================================================

mutable struct MSLBFGS_State{T <: AbstractFloat}
	n::Int                  # Dimensionality of the decision vector x
	L::Int                  # Maximal memory length (maximum retained secants)
	m::Int                  # Current active window of secant conditions (m <= L)

	# Limited-Memory Displacement Matrices (n x L)
	# These represent the most recent coordinate and gradient displacements
	S::Matrix{T}
	Y::Matrix{T}

	# Dense Inner Dimensional Matrices (L x L)
	# Overlap matrix O = S^T Y
	O::Matrix{T}

	# Kernel matrices arising from the variational derivation limit (alpha -> +∞)
	K_R::Matrix{T}          # Right kernel: sqrt(O * O^T)
	K_L::Matrix{T}          # Left kernel: sqrt(O^T * O)

	# Compact Representation Factors (L x L)
	# Square and upper triangular matrices updated iteratively
	R::Matrix{T}
	X::Matrix{T}

	# Constructor for memory preallocation
	function MSLBFGS_State(n::Int, L::Int, ::Type{T} = Float64) where {T <: AbstractFloat}
		new{T}(
			n, L, 0,
			zeros(T, n, L),
			zeros(T, n, L),
			zeros(T, L, L),
			zeros(T, L, L),
			zeros(T, L, L),
			zeros(T, L, L),
			zeros(T, L, L),
		)
	end
end