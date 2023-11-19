import numpy as np
from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt

def LV(t, z, a, b, c, d):
    x, y = z
    return [a*x - b*x*y, -c*y + d*x*y]

sol = solve_ivp(LV, [0,15], [10, 5], args=(1.5, 1, 3, 1),
                dense_output=True);

t = np.linspace(0, 15, 300)
z = sol.sol(t)
plt.plot(t, z.T)
plt.xlabel('t')
plt.legend(['proie', 'prédateur'], shadow=True)
plt.title('Système Proie-Prédateur')
plt.show()