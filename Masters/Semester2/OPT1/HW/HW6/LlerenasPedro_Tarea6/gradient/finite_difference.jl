using LinearAlgebra

function forward_derivative_x(u)
	dx = zeros(eltype(u), size(u))
	dx[1:end-1, :] .= u[2:end, :] .- u[1:end-1, :]
	return dx
end

function forward_derivative_y(u)
	dy = zeros(eltype(u), size(u))
	dy[:, 1:end-1] .= u[:, 2:end] .- u[:, 1:end-1]
	return dy
end

function backward_derivative_x(u)
	dx = zeros(eltype(u), size(u))
	dx[2:end, :] .= u[2:end, :] .- u[1:end-1, :]
	return dx
end

function backward_derivative_y(u)
	dy = zeros(eltype(u), size(u))
	dy[:, 2:end] .= u[:, 2:end] .- u[:, 1:end-1]
	return dy
end

function gradient(u)
	return forward_derivative_x(u), forward_derivative_y(u)
end

function divergence(px, py)
	return backward_derivative_x(px) .+ backward_derivative_y(py)
end

function gradient_2d(u)
	grad_x = zeros(eltype(u), size(u))
	grad_y = zeros(eltype(u), size(u))
	
	# forward difference in x
	grad_x[1:end-1, :] .= u[2:end, :] .- u[1:end-1, :]
	
	# forward difference in y
	grad_y[:, 1:end-1] .= u[:, 2:end] .- u[:, 1:end-1]
	
	return grad_x, grad_y
end

function divergence_2d(px, py)
	div_x = zeros(eltype(px), size(px))
	div_y = zeros(eltype(py), size(py))
	
	# backward difference in x
	div_x[2:end, :] .= px[2:end, :] .- px[1:end-1, :]
	
	# backward difference in y
	div_y[:, 2:end] .= py[:, 2:end] .- py[:, 1:end-1]
	
	return div_x .+ div_y
end
