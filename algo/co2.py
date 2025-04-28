import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import root_scalar
import matplotlib as mpl

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
    #print("C_p=",C_p)
    C_Na = compute_initial_Na(initial_pH, C_Y, C_p)
    res = root_scalar(electroneutrality,
                      args=(P_CO2, C_Y, C_Na, C_p),
                      xtol=1e-12,
                      method='bisect',
                      bracket=[1e-14, 1])
    # print(res)
    return res.root


def buffer_deps():
    #compute_protons(8, 0, 0,0)
    
    # build and array of buffer concentration
    buffer_maxi  = 0.1
    buffer_step  = 0.0001;
    buffer_conc  = np.arange(0, buffer_maxi+buffer_step, buffer_step)
    n = len(buffer_conc)
    
    pH0 = [6, 7, 8, 9]
    m = len(pH0)
    
    # prepare array of pH
    pH_Y = np.zeros([m, n], dtype=float)
    pH_P = np.zeros([m, n], dtype=float)
    
    CO2 = 5.0/100
    # CO2=0.0
    for i in range(n):
        for j in range(m):
            pH_Y[j][i] = -np.log10(compute_protons(pH0[j], CO2, buffer_conc[i],0))
            pH_P[j][i] = -np.log10(compute_protons(pH0[j], CO2, 0,buffer_conc[i]))
    
    
    cmap   = mpl.colormaps['tab10']
    colors = cmap(np.linspace(0, 1, m))
    
    fig, ax = plt.subplots()
    description = []
    for j in range(m):
        plt.plot(buffer_conc, pH_Y[j],linestyle='-',color=colors[j])
        description.append(f"pH$_0$={pH0[j]:.1f}/$HEPES$")
        
    for j in range(m):
        plt.plot(buffer_conc, pH_P[j],linestyle='--',color=colors[j])
        description.append(f"pH$_0$={pH0[j]:.1f}/$H_3PO_4$")
    
    plt.legend(description,fontsize='xx-small')
    plt.xlabel('[buffer] in mol/L')
    plt.ylabel('pH')
    plt.ylim(4.0,8)
    plt.grid()
    plt.show()
    
def co2_deps():
    # variations of co2
    co2_maxi = 5.0/100
    co2_step = 5.0/10000
    co2_pres = np.arange(0,co2_maxi+co2_step,co2_step)
    n = len(co2_pres)
    
    # initial pHs to tests
    pH0 = [6, 7, 8, 9]
    m = len(pH0)
    
    # prepare arrays of pH
    pH_Y = np.zeros([m, n], dtype=float)
    pH_P = np.zeros([m, n], dtype=float)
    C_Y  = 15/10000
    C_P  = 15/10000
    for i in range(n):
        for j in range(m):
            pH_Y[j][i] = -np.log10(compute_protons(pH0[j], co2_pres[i], C_Y,0))
            pH_P[j][i] = -np.log10(compute_protons(pH0[j], co2_pres[i], 0,C_P))
            
    # display all
    cmap   = mpl.colormaps['tab10']
    colors = cmap(np.linspace(0, 1, m))
    
    fig, ax = plt.subplots()
    description = []
   
    for j in range(m):
        plt.plot(co2_pres, pH_P[j],linestyle='-',color=colors[j])
        description.append(f"pH$_0$={pH0[j]:.1f}/$H_3PO_4$")
    
    for j in range(m):
        plt.plot(co2_pres, pH_Y[j],linestyle='--',color=colors[j])
        description.append(f"pH$_0$={pH0[j]:.1f}/$HEPES$")
        
    plt.legend(description,fontsize='small')
    plt.xlabel('%$CO_2$')
    plt.ylabel('pH')
    #plt.ylim(4.0,8)
    plt.grid()
    plt.show()
    
co2_deps()
