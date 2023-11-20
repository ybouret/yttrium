# import what's necessary
import numpy as np
from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt

#definition of equation
# t : time, unsused
# z : [x,y] vector, initial conditions
# a,b,d,c: parameters
def evolution(t, z, a, b, c, d):
    x, y = z
    return [a*x - b*x*y,  c*x*y - d*y]

# using SciPy Ivitial Value Problem solver
tmax = 15
sol  = solve_ivp(evolution, [0,tmax], [10, 5], args=(1.5, 1, 1, 3),
                dense_output=True);

# using result to draw something
t = np.linspace(0, tmax, 300)
z = sol.sol(t)
plt.plot(t, z.T)
plt.xlabel('t')
plt.legend(['proie', 'prédateur'], shadow=True)
plt.title('Système Proie-Prédateur')
plt.grid(color='g', linestyle='dotted', linewidth=1)
plt.show()

xy = np.split(z.T,2,axis=1);
plt.plot(xy[0],xy[1]);
plt.xlabel('x')
plt.ylabel('y')
plt.grid(color='g', linestyle='dotted', linewidth=1)

