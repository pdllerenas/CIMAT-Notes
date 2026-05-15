using CUTEst
using NLPModels
using Optim
using BenchmarkProfiles
using Plots
using Random
using Dates # Needed for explicit time seeding

using Revise
using MSLBFGS

function execute_comprehensive_cutest_suite()
	problem_corpus = select_sif_problems(min_var = 4, max_con = 0)

	blacklist = ["AKIVA", "INDEF", "PARKCH", "STRATEC", "FLETCBV2", "DANWOODLS", "RAT43LS",
		"BA-L16LS", "BA-L21LS", "BA-L49LS", "BA-L52LS", "BA-L73LS", "FLETCBV3",
		"FLETCHBV", "MEYER3", "MGH10LS", "PENALTY2", "BLEACHNG"]

	filter!(name -> !(name in blacklist), problem_corpus)

	# Seed the RNG using the current system time in milliseconds
	Random.seed!(Dates.value(now()))

	# Shuffle the remaining safe problems randomly
	shuffle!(problem_corpus)

	# Safely grab 30 problems (or all of them if less than 30 are available)
	n_problems = min(30, length(problem_corpus))
	problem_corpus = problem_corpus[1:n_problems]

	println("--- Selected $n_problems random topologies for this run ---")
	# =========================================================================

	gcalls_mslbfgs = Float64[]
	gcalls_lbfgsb  = Float64[]

	println("--- Commencing Extended CUTEst Validation Protocol ---")

	for (i, name) in enumerate(problem_corpus)
		println("Analyzing Topology: $name ($(i)/$(length(problem_corpus)))")

		for pass in 1:2
			nlp = nothing
			evals_ms = Inf
			evals_lb = Inf

			try
				nlp = CUTEstModel(name)
				x0_base = copy(nlp.meta.x0)

				if length(x0_base) < 4
					continue
				end

				if pass == 1
					x0 = copy(x0_base)
				else
					xi = 2.0 .* rand(length(x0_base)) .- 1.0
					x0 = x0_base .+ 0.5 .* xi .* max.(1.0, abs.(x0_base))
				end

				f_wrapper(x) = obj(nlp, x)
				g_wrapper!(g, x) = grad!(nlp, x, g)

				optim_options = Optim.Options(iterations = 20000, g_tol = 1e-8)

				_, _, iter_ms, conv_ms = optimize_mslbfgs(f_wrapper, g_wrapper!, copy(x0), 5, max_iter = 20000)
				if conv_ms
					evals_ms = iter_ms + 1
				end

				res_lbfgsb = optimize(f_wrapper, g_wrapper!, copy(x0), LBFGS(m = 5), optim_options)
				if Optim.converged(res_lbfgsb)
					evals_lb = Optim.g_calls(res_lbfgsb)
				end
			catch e
				println("  ⚠️ Catastrophic collapse on $name (Pass $pass). Bypassing.")
				println("     Reason: ", typeof(e), " - ", e)
			finally
				if nlp !== nothing
					finalize(nlp)
				end
			end

			if evals_ms != Inf || evals_lb != Inf
				push!(gcalls_mslbfgs, evals_ms)
				push!(gcalls_lbfgsb, evals_lb)
			end
		end
	end

	println("--- Validation Concluded. Synthesizing Performance Profiles ---")

	T = hcat(gcalls_mslbfgs, gcalls_lbfgsb)

	theme(:default)
	p = performance_profile(
		PlotsBackend(),
		T,
		["MS-LBFGS (Ours)", "L-BFGS-B (Baseline)"],
		title = "Dolan-Moré Empirical Performance Profile (Ng)",
		xlabel = "Performance Ratio (τ)",
		ylabel = "Fraction of Topologies Resolved",
		linewidth = 2.5,
		legend = :bottomright,
		dpi = 300,
	)

	savefig(p, "comprehensive_perf_profile.png")
	println("✅ Artifact 'comprehensive_perf_profile.png' successfully compiled.")
end

execute_comprehensive_cutest_suite()
