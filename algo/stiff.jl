using LinearAlgebra
N=2
M=4
Phi = -rand(N,M)
Nu  = rand(N,M)
dd  = zeros(1,N)
for i = 1:N
	dd[i] = -1.0/(Nu[i,:]'*Phi[i,:])
end
DD = diagm(vec(dd))
Chi = Nu'*DD*Phi

