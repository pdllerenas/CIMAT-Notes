using CSV
using DataFrames
using LinearAlgebra
using ForwardDiff
using Random

include("gradient.jl")

"""
Cargar los archivos
"""
#Iris es una variable tipo tabla
iris = CSV.read("iris.csv", DataFrame)
#Solo seleccionamos las variables numericas
X = Matrix(iris[:, 1:4])
#Guardamos las especies que son, de la ultima fila
Species = iris[:, 5]


"""
Matriz D de distancias
"""
function pairwise_distances(X)
    #numero de observaciones
    m = size(X,1)
    #Matriz de mxm
    D = zeros(m,m)
    #d_ij=||x_i-x_j|
    for i in 1:m
        for j in i+1:m
            D[i,j] = norm(X[i,:] - X[j,:])
            D[j,i] = D[i,j]
        end
    end
    return D
end
#Llenamos con nuestros datos
D = pairwise_distances(X)


"""
MDS
"""
#z es el vector sobre el que vamos a minimizar
#D es la matriz de distancias
#ruido epsilon para evitar problemas con el gradiente
function MDS_stress(z_vec, D; ε=1e-8)
    #Numero de puntos
    m = size(D,1)
    #m vectores de dimension 2
    Z = reshape(z_vec, m, 2)
    #suma
    s = 0.0

    #Hacemos la suma con j<i
    for i in 1:m
        for j in i+1:m
            dij = D[i,j]
            #Agregamos un epsilon pues más adelante se necesitara dividir
            dist = sqrt(sum((Z[i,:] - Z[j,:]).^2) + ε)
            s += (dij - dist)^2
        end
    end
    return 0.5*s
end


"""
Inicializacion
"""
#numero de puntos
m = size(X,1)
#Semilla al azar
rng = MersenneTwister(2026)
#Iniciamos un punto inicial de forma aleatoria (m vectores de dimension 2)
z0 = randn(rng, 2m)


"""
Gradiente descendente con Armijo
"""
#Aqui usamos el metodo que desarrollamos en el ejercicio 1
#z -> MDS_stress(z, D) es la funcion anonima en Julia, fijamos D

#z_opt = GradientDescent(z -> MDS_stress(z, D), z0)


"""
Modficacion de gradiente descendente con Armijo, pero guardando cada iteracion
"""
#Es la misma funcion del ejercicio 1, solo que estamos guardando cada iteracion
function GradientDescentHistory(f, x₀; TOL=1e-6, MAX_ITER=1000, step_function=ArmijoStep)

  xₖ = copy(x₀)
  #guardamos los valores de f
  history_stress = Float64[]
  #guardamos los valores del gradiente
  history_grad = Float64[]

  for k in 1:MAX_ITER
    gₖ = ForwardDiff.gradient(f, xₖ)

    #Guardamos los valores
    push!(history_stress, f(xₖ))
    push!(history_grad, norm(gₖ))

    if norm(gₖ) < TOL
      println("Converged at iteration ", k)
      break
    end

    pₖ = -gₖ
    αₖ = step_function(f, xₖ, pₖ, gₖ)
    xₖ += αₖ .* pₖ
  end

  #Regresamos la solucion y los historiales
  return xₖ, history_stress, history_grad
end

"""
Evaluacion y guardado de las iteraciones
"""
z_opt, stress_hist, grad_hist =GradientDescentHistory(z -> MDS_stress(z, D), z0)


#graficamos
using Plots

#Hacer m vecotres de dimension 2 cada uno
Z = reshape(z_opt, m, 2)

#Especies
species_unique = unique(Species)
colors = Dict(species_unique .=> [:red, :blue, :green])

scatter()
for s in species_unique
    idx = findall(Species .== s)
    scatter!(Z[idx,1], Z[idx,2],
             color=colors[s],
             label=s)
end

savefig("iris_mds.png")