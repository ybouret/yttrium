import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# water
K_w = 10**(-14)

# carbonic acid
K_1 = 4.45 * 10**(-7)
K_2 = 4.69 * 10**(-11)
K_H = 29.41  # atm.L.mol^(-1), Henry's constant

# buffer pKa, here for HEPES
K_Y = 10**(-7.5)

# global data for computation
C_Y   = 0         # buffer concentration
C_Na  = C_Y/2     # added NaOH to produce buffer
P_CO2 = 0.000355  # atmospheric pressure in atm


# matter + charge conservation
def electroneutrality(h):
    return h + C_Na          \
        - (K_w/h)            \
        - C_Y * K_Y/(K_Y+h)  \
        - (P_CO2*K_1)/(K_H*h) * (1.0 + 2.0 * K_2/h)


# use SciPy to solve equation from pH=7
def compute_pH(pressure, buffer):
    global P_CO2, C_Y, C_Na
    P_CO2 = pressure
    C_Y   = buffer
    C_Na  = buffer * 0.5
    return -np.log10(fsolve(electroneutrality, 10**(-7))[0])


# build and array of buffer concentration
buffer_conc = np.arange(0,0.01,0.00001)

# prepare array of pH
pH          = np.arange(len(buffer_conc), dtype=float)

# fill pH
for i in range(len(pH)):
    pH[i] = compute_pH(0.000355,buffer_conc[i])

fig, ax = plt.subplots()
plt.plot(buffer_conc,pH)
plt.grid()
plt.show()


