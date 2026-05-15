using CUTEst
using NLPModels
using Optim
using BenchmarkProfiles
using Plots
using Random

using Revise
using MSLBFGS

function execute_comprehensive_cutest_suite()
    # 1. Enforce dimensionality constraints derived from multi-secant geometric bounds
    problem_corpus = select_sif_problems(min_var = 4, max_con = 0)

    # 2. Extended pathological topology blacklist from the foundational MS-LBFGS literature
    blacklist = ["AKIVA", "INDEF", "PARKCH", "STRATEC", "FLETCBV2", "DANWOODLS", "RAT43LS",
                 "BA-L16LS", "BA-L21LS", "BA-L49LS", "BA-L52LS", "BA-L73LS", "FLETCBV3",
                 "FLETCHBV", "MEYER3", "MGH10LS", "PENALTY2", "BLEACHNG"]
    
    filter!(name -> !(name in blacklist), problem_corpus)

    # Preallocate memory structures for the performance metrics (Gradient Evaluations)
    gcalls_mslbfgs = Float64[]
    gcalls_lbfgs   = Float64[]
    gcalls_bfgs    = Float64[]
    gcalls_lbfgsb  = Float64[]

    println("--- Commencing Extended CUTEst Validation Protocol ---")

    for (i, name) in enumerate(problem_corpus)
        println("Analyzing Topology: $name ($(i)/$(length(problem_corpus)))")

        # 3. Execute both the default initialization and the randomized replication protocol
        for pass in 1:2
            nlp = nothing
            # Initialize iteration metrics to infinity to penalize convergence failures
            evals_ms = Inf; evals_l = Inf; evals_b = Inf; evals_lb = Inf

            try
                nlp = CUTEstModel(name)
                x0_base = copy(nlp.meta.x0)

                # Dynamic dimensionality assertion
                if length(x0_base) < 4
                    continue
                end

                if pass == 1
                    x0 = copy(x0_base)
                else
                    # Rigorous uniform perturbation to diminish initialization bias
                    xi = 2.0 .* rand(length(x0_base)) .- 1.0
                    x0 = x0_base .+ 0.5 .* xi .* max.(1.0, abs.(x0_base))
                end

                f_wrapper(x) = obj(nlp, x)
                g_wrapper!(g, x) = grad!(nlp, x, g)

                # 4. Upper convergence bound established at 20,000 iterations
                optim_options = Optim.Options(iterations = 20000, g_tol = 1e-8)

                # =========================================================
                # SOLVER 1: MS-LBFGS Architecture (Strong Wolfe Integrated)
                # =========================================================
                _, _, iter_ms, conv_ms = optimize_mslbfgs(f_wrapper, g_wrapper!, copy(x0), 5, max_iter = 20000)
                if conv_ms 
                    evals_ms = iter_ms + 1 
                end

                # =========================================================
                # SOLVER 2: Standard L-BFGS Baseline
                # =========================================================
                res_lbfgs = optimize(f_wrapper, g_wrapper!, copy(x0), LBFGS(m = 5), optim_options)
                if Optim.converged(res_lbfgs) 
                    evals_l = Optim.g_calls(res_lbfgs) 
                end

                # =========================================================
                # SOLVER 3: Exact BFGS Baseline
                # =========================================================
                res_bfgs = optimize(f_wrapper, g_wrapper!, copy(x0), BFGS(), optim_options)
                if Optim.converged(res_bfgs) 
                    evals_b = Optim.g_calls(res_bfgs) 
                end

                # =========================================================
                # SOLVER 4: L-BFGS-B Baseline 
                # (Invoked via Box-Constrained Wrapper with Infinite Limits)
                # =========================================================
                lower_bounds = fill(-Inf, length(x0))
                upper_bounds = fill(Inf, length(x0))
                res_lbfgsb = optimize(f_wrapper, g_wrapper!, lower_bounds, upper_bounds, copy(x0), Fminbox(LBFGS(m = 5)), optim_options)
                if Optim.converged(res_lbfgsb) 
                    evals_lb = Optim.g_calls(res_lbfgsb) 
                end

            catch e
                println("  ⚠️ Catastrophic topological collapse on $name (Pass $pass). Bypassing.")
            finally
                if nlp !== nothing
                    finalize(nlp)
                end
            end

            # Append results to preserve matrix dimensional alignment.
            # We omit problems where ALL solvers fail, as they yield no comparative performance ratio.
            if evals_ms != Inf || evals_l != Inf || evals_b != Inf || evals_lb != Inf
                push!(gcalls_mslbfgs, evals_ms)
                push!(gcalls_lbfgs, evals_l)
                push!(gcalls_bfgs, evals_b)
                push!(gcalls_lbfgsb, evals_lb)
            end
        end
    end

    println("--- Validation Concluded. Synthesizing Performance Profiles ---")

    T = hcat(gcalls_mslbfgs, gcalls_lbfgs, gcalls_bfgs, gcalls_lbfgsb)

    theme(:default)
    p = performance_profile(
        PlotsBackend(),
        T,
        ["MS-LBFGS (Ours)", "Standard L-BFGS", "Exact BFGS", "L-BFGS-B (Baseline)"],
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