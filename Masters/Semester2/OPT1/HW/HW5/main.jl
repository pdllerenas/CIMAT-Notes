using GZip
using Pickle
using LinearAlgebra
using Plots

include("trust_region/trust_region.jl")
include("trust_region/step_finder.jl")

function calculate_error(z, X_tilde, y)
	π_vec = sigmoid(X_tilde * z)

	predictions = π_vec .> 0.5

	n = length(y)
	error_rate = sum(abs.(predictions .- y)) / n

	return error_rate
end

function sigmoid(z::AbstractVector)
	return 1.0 ./ (1.0 .+ exp.(.-z))
end

# objective function
function f(z, X_tilde, y, λ = 1e-4)
	πᵢ = sigmoid(X_tilde * z)
	ϵ = 1e-15
	# clamp pi element-wise to keep withing machine epsilon for numerical stability
	πᵢ = clamp.(πᵢ, ϵ, 1.0 - ϵ)
	log_likelihood = -sum(y .* log.(πᵢ) .+ (1.0 .- y) .* log.(1.0 .- πᵢ))
	return log_likelihood + (λ / 2) * sum(z .^ 2)
end

# gradient of objective function
function g(z, X_tilde, y, λ = 1e-4)
	πᵢ = sigmoid(X_tilde * z)
	return X_tilde' * (πᵢ .- y) .+ λ .* z
end

# hessian of objective function
function H(z, X_tilde, λ = 1e-4)
	πᵢ = sigmoid(X_tilde * z)
	D = πᵢ .* (1.0 .- πᵢ)
	# make positive definite so finding inverse does not fail
	return X_tilde' * (D .* X_tilde) + λ * I
end

function main()
	file_path = "mnist.pkl.gz"

	data = GZip.open(file_path, "r") do io
		Pickle.npyload(io)
	end

	train_set, valid_set, test_data = data
	train_x, train_y = train_set
	test_x, test_y = test_data

	# only consider 0 and 1 y's (as per the document suggestion)
	binary_mask = (train_y .== 0) .| (train_y .== 1)
	train_x_bin = train_x[binary_mask, :]
	train_y_bin = train_y[binary_mask]

	test_binary_mask = (test_y .== 0) .| (test_y .== 1)
	test_x_bin = test_x[test_binary_mask, :]
	test_y_bin = test_y[test_binary_mask]

	n_samples = size(train_x_bin, 1)
	X_tilde = hcat(train_x_bin, ones(n_samples))

	n_test_samples = size(test_x_bin, 1)
	X_tilde_test = hcat(test_x_bin, ones(n_test_samples))

	f_obj(z) = f(z, X_tilde, train_y_bin)
	g_obj(z) = g(z, X_tilde, train_y_bin)
	H_obj(z) = H(z, X_tilde)

	z₀ = zeros(785)

	# compile code before timing it (JIT compilation may add more time than its real execution time)
	trust_region(f_obj, g_obj, H_obj, z₀, 10.0, 1.0, 0.15, dogleg_step_finder, max_iter = 1)
	steepest_descent_armijo(f_obj, g_obj, z₀, max_iter = 1)

	time_dogleg = @elapsed begin
		z_dogleg, f_hist_dogleg, g_hist_dogleg, Δ_hist_dogleg = trust_region(f_obj, g_obj, H_obj, z₀, 10.0, 1.0, 0.15, dogleg_step_finder)
	end
	println("Time: ", round(time_dogleg, digits = 4), " seconds")
	time_armijo = @elapsed begin
		z_armijo, f_hist_armijo, g_hist_armijo = steepest_descent_armijo(f_obj, g_obj, z₀, max_iter = 5000)
	end
	println("Time: ", round(time_armijo, digits = 4), " seconds")

	p_gk = plot(
		g_hist_dogleg,
		label = "Trust Region (Dogleg)",
		linewidth = 2,
		xlabel = "Iterations (k)",
		ylabel = "||∇f(z)||",
		title = "Gradient Norm vs Iterations",
		yaxis = :log10, 
	)
	plot!(p_gk, g_hist_armijo, label = "Steepest Descent", linewidth = 2)
	savefig(p_gk, "plot_gk.png")
	println("Saved plot_gk.png")

	p_fk = plot(
		f_hist_dogleg,
		label = "Trust Region (Dogleg)",
		linewidth = 2,
		xlabel = "Iterations (k)",
		ylabel = "Negative Log-Likelihood f(z)",
		title = "Objective Function vs Iterations",
	)
	plot!(p_fk, f_hist_armijo, label = "Steepest Descent", linewidth = 2)
	savefig(p_fk, "plot_fk.png")
	println("Saved plot_fk.png")


	p_deltak = plot(
		Δ_hist_dogleg,
		label = "Δ_k (Dogleg)",
		linewidth = 2,
		xlabel = "Iterations (k)",
		ylabel = "Trust Region Radius Δ",
		title = "Trust Region Size vs Iterations",
		color = :green,
	)
	savefig(p_deltak, "plot_deltak.png")
	println("Saved plot_deltak.png")

	println("f(armijo) :", f_hist_armijo[end])
	println("f(dogleg): ", f_hist_dogleg[end])

	println("||g(armijo)|| :", norm(g_hist_armijo[end]))
	println("||g(dogleg)||: ", norm(g_hist_dogleg[end]))

	error_dogleg = calculate_error(z_dogleg, X_tilde_test, test_y_bin)
	error_armijo = calculate_error(z_armijo, X_tilde_test, test_y_bin)

	println("Dogleg Test Error: ", round(error_dogleg * 100, digits = 2), "%")
	println("Armijo Test Error: ", round(error_armijo * 100, digits = 2), "%")
end

if abspath(PROGRAM_FILE) == @__FILE__
	main()
end
