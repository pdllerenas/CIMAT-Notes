@testset "Compact Representation and Hessian Application" begin
    n = 10
    L = 3
    
    @testset "Empty State (m = 0)" begin
        state = MSLBFGS_State(n, L)
        update_compact_factors!(state)
        # Should return early doing nothing
        @test all(state.X .== 0.0)
    end

    @testset "Base Case Update (m = 1)" begin
        state = MSLBFGS_State(n, L)
        state.m = 1
        
        # Inject mock data into O and K_R
        state.O[1, 1] = 4.0
        state.K_R[1, 1] = 9.0
        
        update_compact_factors!(state)
        
        # Check if the corrected [1, 1] logic works
        @test state.X[1, 1] == 4.0
        @test state.R[1, 1] == 3.0 # sqrt(9.0)
    end

    @testset "Recursive Augmentation (m = 2)" begin
        state = MSLBFGS_State(n, L)
        state.m = 2
        
        # Setup valid SPD mock data so Cholesky doesn't fail
        # O = [2.0 1.0; 1.0 2.0]
        state.O[1:2, 1:2] .= [2.0 1.0; 1.0 2.0]
        state.X[1, 1] = 2.0 # Assume previous state was correctly set
        
        # Make K_R a valid SPD matrix
        state.K_R[1:2, 1:2] .= [4.0 1.0; 1.0 4.0]
        
        update_compact_factors!(state)
        
        # Check X augmentation
        # X[1, 2] gets O[1, 2]
        @test state.X[1, 2] == 1.0
        # X[2, 2] gets O[2, 2]
        @test state.X[2, 2] == 2.0
        
        # Check R augmentation (Cholesky behavior)
        # cholesky([4 1; 1 4]).L yields lower triangular factor
        L_fact = cholesky(Symmetric([4.0 1.0; 1.0 4.0])).L
        @test state.R[2, 2] == L_fact[2, 2]
        
        # Ensure strict upper triangularity was enforced
        @test state.R[2, 1] == 0.0
    end

    @testset "Hessian Application (apply_compact_hessian)" begin
        state = MSLBFGS_State(n, L)
        state.m = 2
        
        # Mock matrices to allow the complex inverse math to run without SingularExceptions
        state.S[1:2, 1:2] .= [1.0 0.0; 0.0 1.0]
        state.Y[1:2, 1:2] .= [1.0 0.0; 0.0 1.0]
        state.X[1:2, 1:2] .= [2.0 0.5; 0.5 2.0]
        state.R[1:2, 1:2] .= [2.0 0.5; 0.0 2.0] # Upper triangular
        
        v = ones(Float64, n)
        H0_scale = 1.0
        
        result_v = apply_compact_hessian(state, v, H0_scale)
        
        # Dimensionality check (must return a vector of size n)
        @test length(result_v) == n
        @test typeof(result_v) == Vector{Float64}
        
        # Numerical stability check
        @test !any(isnan.(result_v))
        @test !any(isinf.(result_v))
    end
end