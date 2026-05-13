n = 100
L = 5
state = MSLBFGS_State(n, L)

@test state.n == 100
@test state.L == 5
@test state.m == 0

@test size(state.S) == (n, L)
@test size(state.O) == (L, L)

@test all(state.S .== 0.0)
@test all(state.K_R .== 0.0)
