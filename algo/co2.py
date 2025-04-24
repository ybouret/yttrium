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

# phosphoric acid
K_a1 = 10**(-2.14)
K_a2 = 10**(-7.20)
K_a3 = 10**(-12.37)


def water_charge(h):
    return h-K_w/h


def co2_charge(h, CO2):
    return - (CO2*K_1)/(K_H*h) * (1.0 + 2.0 * K_2/h)


def Y_charge(h, Y):
    return - Y * K_Y/(K_Y+h)


def phosphoric_charge(h, C_p):
    k1 = K_a1/h
    k2 = K_a2/h
    k3 = K_a3/h
    den = 1+k1*(1+k2*(1+k3))
    num = k1*(1+2*k2*(1+1.5*k3))
    return - C_p * num/den

# matter + charge conservation


def electroneutrality(h, CO2, Y, Na, C_p):
    return Na                \
        + water_charge(h)    \
        + Y_charge(h, Y)     \
        + co2_charge(h, CO2) \
        + phosphoric_charge(h,C_p)


def compute_initial_Na(initial_pH, initial_buffer, initial_phosphoric):
    h0 = 10.0**(-initial_pH)
    return - electroneutrality(h0, 0.0, initial_buffer, 0.0, initial_phosphoric)


def compute_protons(initial_pH, P_CO2, C_Y, C_p):
    print("C_p=",C_p)
    C_Na = compute_initial_Na(initial_pH, C_Y, C_p)
    res = root_scalar(electroneutrality,
                      args=(P_CO2, C_Y, C_Na, C_p),
                      xtol=1e-12,
                      method='bisect',
                      bracket=[1e-14, 1])
    # print(res)
    return res.root


compute_protons(8, 0, 0,0)

# build and array of buffer concentration
buffer_maxi  = 0.1
buffer_step  = 0.0001;
buffer_conc  = np.arange(0, buffer_maxi+buffer_step, buffer_step)
n = len(buffer_conc)

pH0 = [6.5, 7, 7.5, 8, 8.5, 9]
m = len(pH0)

# prepare array of pH
pH = np.zeros([m, n], dtype=float)


CO2 = 5.0/100
# CO2=0.0
for i in range(n):
    for j in range(m):
        pH[j][i] = -np.log10(compute_protons(pH0[j], CO2, buffer_conc[i],0))

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
