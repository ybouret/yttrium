import numpy as np
from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt

def evolution(t, z, k_f, k_d, k_p, k_r):
    """
    definition of evolution
        # t : time, unsused but necessary for ODE
        # z : [x,y] vector, initial conditions
        # k_f, k_d, k_p, k_r
    """
    # getting variables
    E, S, ES, P = z

    # computing rates
    v_f = k_f * E * S
    v_d = k_d * ES
    v_p = k_p * ES
    v_r = k_r * E * P

    # computing variations
    dt_E  = v_d - v_f + v_p - v_r
    dt_S  = v_d - v_f
    dt_ES = v_f - v_d + v_r - v_p
    dt_P  = v_p - v_r
    return [dt_E,dt_S,dt_ES,dt_P]

# parameters
E_0 = 1
S_0 = 2

C_0 = [E_0,S_0,0,0]

k_f = 3
k_d = 1
k_p = 4
k_r = 1

# using SciPy Initial Value Problem solver
tmax = 5
sol  = solve_ivp(evolution, [0,tmax], C_0, args=(k_f,k_d,k_p,k_r),
               dense_output=True)

# using result to draw something
t = np.linspace(0, tmax, 300)
z = sol.sol(t)
plt.plot(t, z.T)
plt.xlabel('t')
plt.legend(['E', 'S', 'ES', 'P'], shadow=True)
plt.title('Michaelis Menten')
plt.grid(color='tab:purple', linestyle='dotted', linewidth=1)
plt.show()