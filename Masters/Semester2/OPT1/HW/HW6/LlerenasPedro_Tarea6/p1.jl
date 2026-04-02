using LinearAlgebra
using DelimitedFiles
include("gradient/gradient.jl")

function read_custom_matrix(filepath)
	open(filepath, "r") do io
		header = readline(io)
		dims = parse.(Float64, split(header))
		expected_rows, expected_cols = dims[1], dims[2]
		
		A = readdlm(io, Float64)
		
		if size(A) != (expected_rows, expected_cols)
				@warn "Matrix size $(size(A)) does not match header $dims"
		end
		
		return A
	end
end

function main()
	A = read_custom_matrix(ARGS[1])
	
	b_matrix = read_custom_matrix(ARGS[2])
	b = vec(b_matrix)
	
	x0 = similar(b)
	x0 .= 1.0
	
	M_inv = Diagonal(1.0 ./ diag(A))
	
	println("Result:")
	println(preconditioned_conjugate_gradient(A, b, x0, M_inv))
end

if abspath(PROGRAM_FILE) == @__FILE__
	main()
end
