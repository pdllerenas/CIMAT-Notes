using LinearAlgebra
using ForwardDiff
using Random
using Printf

const seed = 2026

"""
Armijo condition for step size
"""
function ArmijoStep(f, x, d, g; σ=1e-4, ρ=0.5)
  l = 0
  α = 1.0
  while (f(x .+ α .* d) > f(x) + σ * α * dot(g, d))
    α *= ρ
    l += 1
  end
  return α, l
end

"""
Generic gradient descent. We can plug in any step finding function. Default is
Armijo condition
"""
function GradientDescent(f, x₀, file_out; TOL=1e-6, MAX_ITER=10000, step_function=ArmijoStep)::Vector{Float64}
  step_io = open("$file_out step_log.csv", "w")
  println(step_io, "iter,step,backtrack")
  k::Int = 0
  xₖ = copy(x₀)
  gₖ = 0.0

  start_time = time_ns()
  open("$file_out.csv", "w") do io
    println(io, "iter,time,f(xk),||Df(xk)||")
    for k in 1:MAX_ITER 
      t₀ = time_ns()

      # ======
      fₖ = f(xₖ) # only used for logging
      gₖ = ForwardDiff.gradient(f, xₖ)
      nrm = norm(gₖ)
      if (norm(gₖ) < TOL) 
        t₁ = time_ns()
        s = (t₁ - t₀) / 1e9 # to seconds
        @printf(io, "%d,%.6f,%.6e,%.8f\n", k,s,fₖ,nrm)
        break
      end
      pₖ = -gₖ
      αₖ, lₖ = step_function(f, xₖ, pₖ, gₖ)
      println(step_io, "$k,$αₖ,$lₖ")
      xₖ += αₖ .* pₖ
      # ======

      t₁ = time_ns()
      s = (t₁ - t₀) / 1e9 # to seconds
      @printf(io, "%d,%.6f,%.6e,%.8f\n", k,s,fₖ,nrm)
    end
  end
  end_time = time_ns()
  close(step_io)
  println("Total time: ", (end_time - start_time)/1e9)
  return xₖ
end

"""
Generic Rosenbrock function at x
"""
function Rosenbrock(x)
  sum(100 .* (x[2:end] .- x[1:end - 1].^2).^2 .+ (1 .- x[1:end-1]).^2)
end

# Inicialización clásica
A = [isodd(i) ? -1.2 : 1.0 for i in 1:128]

# Inicialización aleatoria
rng = MersenneTwister(seed)
B = rand(rng, 128)

xₐ = GradientDescent(Rosenbrock, A, "classical_rosenbrock")
println("Solution for classical initializacion: ", xₐ, "\n")

xᵦ = GradientDescent(Rosenbrock, B, "random_rosenbrock")
println("Solution for random initializacion: ", xᵦ, "\n")

println("Norm of difference vector: ", norm(xₐ .- xᵦ), "\n")