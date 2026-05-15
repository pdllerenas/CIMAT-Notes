using LinearAlgebra
using CUTEst
using NLPModels

 using MSLBFGS

function run_benchmarks()
    # 1. Grab 5 small, unconstrained problems
    problem_names = select_sif_problems(min_var=2, max_var=50, max_con=0)[1:5]

    println("--- Starting CUTEst Benchmark ---")

    for name in problem_names
        println("\nLoading Problem: $name")
        nlp = CUTEstModel(name)
        
        try
            x0 = nlp.meta.x0
            f_wrapper(x) = obj(nlp, x)
            g_wrapper!(g, x) = grad!(nlp, x, g)
            
            # Run your solver
            x_opt, f_opt, iters, converged = optimize_mslbfgs(f_wrapper, g_wrapper!, x0, 5)
            
            if converged
                println("  ✅ Converged in $iters iterations.")
                println("  🎯 Final Objective Value: ", round(f_opt, digits=6))
            else
                println("  ❌ Failed to converge (Hit Max Iterations).")
            end
            
        catch e
            println("  ⚠️ Error running solver on $name: ", e)
        finally
            finalize(nlp)
        end
    end

    println("\n--- Benchmark Complete ---")
end

# Run it!
run_benchmarks()