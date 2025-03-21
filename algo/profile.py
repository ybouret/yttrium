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


pH0 = 6
h0 = 10**(-pH0)
q0 = negative_charge(h0)


pH1 = 8
h1 = 10**(-pH1)
q1 = negative_charge(h1)

u = np.linspace(0, 1, 101)
Q = linear_mix(u, q0, q1)
h = np.linspace(0, 1, len(u))

for i in range(len(u)):
    h[i] = compute_h0(Q[i])

plt.plot(u, -np.log10(h))
# plt.plot(u,h)
plt.show()
