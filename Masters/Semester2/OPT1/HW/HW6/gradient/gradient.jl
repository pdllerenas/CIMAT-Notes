using LinearAlgebra

function conjugate_gradient(A, b, x0; tol = 1e-8, max_iter=1000)
	x = copy(x0)
	r = A*x .- b
	p = -r

	Ap = similar(x)
	r_norm_sq = dot(r,r)
	for k in 1:max_iter
		if sqrt(r_norm_sq) < tol
			break
		end

		mul!(Ap, A, p)
		alpha = r_norm_sq / dot(p,Ap)

		x .+= alpha .* p
		r .+= alpha .* Ap

		r_next_norm_sq = dot(r,r)
		beta = r_next_norm_sq / r_norm_sq

		p .= -r .+beta .* p

		r_norm_sq = r_next_norm_sq
	end
	return x
end

function preconditioned_conjugate_gradient(A, b, x0, M_inv; tol = 1e-8, max_iter=1000)
	x = copy(x0)
	r = A*x .- b
	
	y = M_inv * r
	
	p = -copy(y)

	Ap = similar(x)
	
	r_y_dot = dot(r, y)
	
	for k in 1:max_iter
		if norm(r) < tol
			println("Converged in $k iterations.")
			break
		end

		mul!(Ap, A, p)
		alpha = r_y_dot / dot(p, Ap)

		x .+= alpha .* p
		r .+= alpha .* Ap

		y .= M_inv * r

		r_next_y_next_dot = dot(r, y)
		beta = r_next_y_next_dot / r_y_dot

		p .= .-y .+ beta .* p

		r_y_dot = r_next_y_next_dot
	end
	return x
end
