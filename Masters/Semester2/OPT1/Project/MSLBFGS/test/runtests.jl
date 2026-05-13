using MSLBFGS
using Test
using LinearAlgebra

@testset "MSLBFGS Solver Suite" begin
	@testset "State Intialization" begin
		include("test_init.jl")
	end

	@testset "Overlap Matrix" begin
		include("test_overlap.jl")
	end

	@testset "Damping" begin
		include("test_damping.jl")
	end

	@testset "Compact Representation and Hessian Application" begin
		include("test_compact.jl")
	end

	@testset "End-to-End Optimization Loop" begin
		include("test_optimization.jl")
	end


end
