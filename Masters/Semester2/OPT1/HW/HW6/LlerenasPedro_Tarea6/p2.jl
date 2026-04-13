using FileIO
using Images
using Plots
include("gradient/finite_difference.jl")
include("gradient/descent.jl")

function g(u, u0; epsilon = 0.0001, gamma = 40)
    grad_x, grad_y = gradient(u)
  
    mag = sqrt.(grad_x.^2 .+ grad_y.^2 .+ epsilon)
    
    norm_grad_x = grad_x ./ mag
    norm_grad_y = grad_y ./ mag
    
    return u .- u0 .- gamma .* divergence(norm_grad_x, norm_grad_y)
end

function main()
    img = load("noisy25_kodim23_greylevel.png") 
    img_gray = Gray.(img)
    u0 = Float64.(img_gray)
    
    u_final = basic_gradient_descent(g, u0, alpha=0.0001, max_iter=15000)
    
    clean_image = colorview(Gray, clamp.(u_final, 0.0, 1.0))
    save("denoised_result_2.png", clean_image)
    println("Denoising complete. Saved as denoised_result.png")
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end
