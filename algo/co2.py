import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import root_scalar

# water
K_w = 10**(-14)

# carbonic acid
K_1 = 4.45 * 10**(-7)
K_2 = 4.69 * 10**(-11)
K_H = 29.41  # atm.L.mol^(-1), Henry's constant

# buffer pKa, here for HEPES
K_Y = 10**(-7.5)


# matter + charge conservation
def electroneutrality(h, CO2, Y, Na):
    return h + Na          \
        - (K_w/h)          \
        - Y * K_Y/(K_Y+h)  \
        - (CO2*K_1)/(K_H*h) * (1.0 + 2.0 * K_2/h)


def compute_initial_Na(initial_pH, initial_buffer):
    h0 = 10.0**(-initial_pH)
    return - electroneutrality(h0, 0.0, initial_buffer, 0.0)


def compute_protons(initial_pH, P_CO2, C_Y):
    C_Na = compute_initial_Na(initial_pH, C_Y)
    res = root_scalar(electroneutrality,
                      args=(P_CO2, C_Y, C_Na),
                      xtol=1e-12,
                      method='bisect',
                      bracket=[1e-14, 1])
    # print(res)
    return res.root


compute_protons(8, 0, 0)

# build and array of buffer concentration
buffer_conc = np.arange(0, 0.1, 0.0001)
n = len(buffer_conc)

pH0 = [7, 7.5, 8, 8.5, 9]
m = len(pH0)

# prepare array of pH
pH = np.zeros([m, n], dtype=float)


CO2 = 5.0/100
# CO2=0.0
for i in range(n):
    for j in range(m):
        pH[j][i] = -np.log10(compute_protons(pH0[j], CO2, buffer_conc[i]))

fig, ax = plt.subplots()
description = []
for j in range(m):
    plt.plot(buffer_conc, pH[j])
    description.append(f"pH0={pH0[j]:.1f}")

plt.legend(description)
plt.xlabel('[buffer] in mol/L')
plt.ylabel('pH')
plt.grid()
plt.show()
