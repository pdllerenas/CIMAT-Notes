using CUTEst
using NLPModels
using Optim
using BenchmarkProfiles
using Plots
using Random

using Revise
using MSLBFGS

function generate_performance_profile()
	# Strict Dimensionality Filtration (minimum 4 variables for multi-secant geometry)
	problem_names = select_sif_problems(min_var = 4, max_var = 50, max_con = 0)

	blacklist = ["BLEACHNG", "AKIVA", "INDEF", "PARKCH", "STRATEC", "FLETCBV2", "DANWOODLS", "RAT43LS"]
	filter!(name -> !(name in blacklist), problem_names)

	problem_names = problem_names[1:15]
	num_probs = length(problem_names)

	# Performance Metric: Gradient Evaluations (Ng)
	gcalls_mslbfgs = fill(Inf, num_probs)
	gcalls_lbfgs   = fill(Inf, num_probs)
	gcalls_bfgs    = fill(Inf, num_probs)

	println("--- Starting Comparative Benchmark ---")

	for (i, name) in enumerate(problem_names)
		println("Testing: $name ($(i)/$(num_probs))")
		nlp = CUTEstModel(name)

		try
			x0_default = copy(nlp.meta.x0)

			xi = 2.0 .* rand(length(x0_default)) .- 1.0
			x0 = x0_default .+ 0.5 .* xi .* max.(1.0, abs.(x0_default))

			# Wrapper functions
			f_wrapper(x) = obj(nlp, x)
			g_wrapper!(g, x) = grad!(nlp, x, g)

			# Standard Termination Tolerances
			optim_options = Optim.Options(iterations = 10000, g_tol = 1e-8)

			# =========================================================
			# SOLVER 1: MS-LBFGS (Our Multi-Secant Architecture)
			# =========================================================
			x_opt, f_opt, iter_ms, conv_ms = optimize_mslbfgs(f_wrapper, g_wrapper!, copy(x0), 5, max_iter = 10000)
			if conv_ms
				gcalls_mslbfgs[i] = iter_ms + 1
			end

			# =========================================================
			# SOLVER 2: Standard L-BFGS (Optim.jl Baseline)
			# =========================================================
			res_lbfgs = optimize(f_wrapper, g_wrapper!, copy(x0), LBFGS(m = 5), optim_options)
			if Optim.converged(res_lbfgs)
				gcalls_lbfgs[i] = Optim.g_calls(res_lbfgs)
			end

			# =========================================================
			# SOLVER 3: Classical BFGS (Optim.jl Baseline)
			# =========================================================
			res_bfgs = optimize(f_wrapper, g_wrapper!, copy(x0), BFGS(), optim_options)
			if Optim.converged(res_bfgs)
				gcalls_bfgs[i] = Optim.g_calls(res_bfgs)
			end

		catch e
			println("  ⚠️ Mathematical failure on $name. Skipping.")
		finally
			finalize(nlp)
		end
	end

	println("--- Benchmark Complete! Generating Plot... ---")

	# Format the data matrix for the Dolan-Moré profile
	T = hcat(gcalls_mslbfgs, gcalls_lbfgs, gcalls_bfgs)

	# Generate the Dolan-Moré Performance Profile
	theme(:default)
	p = performance_profile(
		PlotsBackend(),
		T,
		["MS-LBFGS (Ours)", "Standard L-BFGS (Optim.jl)", "Classical BFGS (Optim.jl)"],
		title = "Dolan-Moré Performance Profile",
		xlabel = "Performance Ratio (τ) [Gradient Evaluations]",
		ylabel = "Fraction of problems solved",
		linewidth = 2.5,
		legend = :bottomright,
		dpi = 300,
	)

	savefig(p, "perf_profile.png")
	println("✅ Success! 'perf_profile.png' has been saved to your directory.")

	println("\n--- Raw Data for Typst Table (Gradient Calls) ---")
	for i in 1:num_probs
		m_gcalls = gcalls_mslbfgs[i] == Inf ? "Failed" : Int(gcalls_mslbfgs[i])
		l_gcalls = gcalls_lbfgs[i] == Inf ? "Failed" : Int(gcalls_lbfgs[i])
		b_gcalls = gcalls_bfgs[i] == Inf ? "Failed" : Int(gcalls_bfgs[i])
		println("[$(problem_names[i])], [$m_gcalls], [$l_gcalls], [$b_gcalls],")
	end
end

generate_performance_profile()
