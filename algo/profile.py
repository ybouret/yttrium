""" import what is necessart """
import numpy as np
import matplotlib.pyplot as plt


KW = 1e-14

LAM_AH = 0.55
LAM_A = 1.0-LAM_AH
CA0 = 0.00

KA = 10**(-5.2)


def compute_AH(h):
    return CA0 * h / (h+KA)


def compute_A(h):
    return CA0 * KA/(h+KA)


def negative_charge(h):
    """ negative charge """
    return KW/h-h+compute_A(h)


def linear_mix(u, f0, f1):
    """ Linear Function """
    return f0*(1-u)+f1*u


def compute_h0(q):
    """ [H^+] when strong acid/base only """
    delta = q*q + 4 * KW
    return (np.sqrt(delta)-q)*0.5


ca_list = [0,0.01]
for ca in ca_list:
    #global CA0
    CA0 = ca
    

print(CA0)

pH0 = 6
h0 = 10**(-pH0)
q0 = negative_charge(h0)


pH1 = 8
h1 = 10**(-pH1)
q1 = negative_charge(h1)

u = np.linspace(0, 1, 101)
Q = linear_mix(u, q0, q1)
h_0 = np.linspace(0, 1, len(u))

for i in range(len(u)):
    h_0[i] = compute_h0(Q[i])

fig, plotpH = plt.subplots(figsize=(5, 4))
plotpH.plot(u,-np.log10(h_0));

plotpH.set_ylabel("pH")
plotpH.set_xlabel("$x/L$")

#plt.plot(u, -np.log10(h_0))
#plt.plot(u,h)
plt.tight_layout()
plt.show()

