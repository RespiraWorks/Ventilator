# -*- coding: utf-8 -*-
"""
Created on Sun Jun  7 16:43:32 2020

@author: bryan
"""


def Q34_from_AMS(kPa):
    import numpy as np

    A = 1.42549766  # was 1.21609795
    B = 6516.225347  # was 6653.33966
    C = 0.97  # correlation value for Re~10^4
    offset = 1.0  # for i2c AMS5915
    volts = kPa + 1
    Q = C * A * np.sqrt(B * abs((volts - offset) / (5.0 - offset)))
    return Q / 60


def Q34_from_adc(adc_val, adc_offset):
    import numpy as np

    # 3/4" is "new" venturi (first used 5/30/2020)
    A = 1.42549766  # was 1.21609795
    B = 6516.225347  # was 6653.33966
    C = 0.97  # correlation value for Re~10^4
    Q = C * A * np.sqrt(B * abs((adc_val - adc_offset) / (16383 - adc_offset)))
    return Q / 60


def kPa_from_adc(adc_val, adc_offset):
    return 3.920 * ((adc_val - adc_offset) / (16383 - adc_offset))
    # return ((adc_val-adc_offset)/16383)


#%% Import PCB data
import pandas as pd

folder = r"C:\Users\bryan\Documents\PV\06_07_Tests_at_Ethans\data\\"
file = r"ptg_venturi_i2c_0607_x1_pcb.txt"
p2pcbdata = folder + file

# Variables names from "pcbreathe-bringup.ino"
names = ["t", "ValvePos", "press_adc", "inh_adc", "exh_adc", "ams_p", "ams_t"]

# Create dataframe, reset time to start at 0ms
df0 = pd.read_csv(p2pcbdata, sep=None, names=names, index_col=False, engine="python")
df0.t = (df0.t - df0.t[0]) / 1000

# Add dP and Q columns for each sensor
press_adc_offset = 3251  # empircally on 6/7/20
inh_adc_offset = 3554  #
exh_adc_offset = 3103  #
df0["press_dp"] = kPa_from_adc(df0.press_adc, press_adc_offset)
df0["inh_dp"] = kPa_from_adc(df0.inh_adc, inh_adc_offset)
df0["exh_dp"] = kPa_from_adc(df0.exh_adc, exh_adc_offset)
df0["Q_press"] = Q34_from_adc(df0.press_adc, press_adc_offset)
df0["Q_inh"] = Q34_from_adc(df0.inh_adc, inh_adc_offset)
df0["Q_exh"] = Q34_from_adc(df0.exh_adc, exh_adc_offset)

# Convert AMS5915 to kPa
df0.ams_p = df0.ams_p / 1000
df0["Q_ams"] = Q34_from_AMS(df0.ams_p)

#%% Import PTG data
folder = r"C:\Users\bryan\Documents\PV\06_07_Tests_at_Ethans\data\\"
labchart_file = r"ptg_venturi_i2c_0607_x1_labchart.txt"
p2ptgdata = folder + labchart_file

ptg_names = ["t", "Q_ptg"]
ptg = pd.read_csv(
    p2ptgdata, sep=None, header=7, names=ptg_names, index_col=False, engine="python"
)
ptg.t = ptg.t - ptg.t[0] - 0.35

#%% Decimate PTG data to fit with PCB data
df1 = pd.merge(df0, ptg, on="t", how="inner")

#%%
#   -----------------------------------------------------------------------------------------------------------
#       Part 1:
#       Comparing the two pressure sensors: MXP5004D vs AMS5915

import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(121)
ax1.plot(df0.t, df0.press_dp)
ax1.plot(df0.t, df0.ams_p)
ax1.legend(["pcb", "AMS"])
ax1.set_title("MXP5004DP vs AMS5915")
ax1.set_ylabel("dP (kPa)")
ax1.set_xlabel("time (sec)")
ax1.grid()
ax1.legend(["MXP5004DP", "AMS5915"])

#%% Linear Regression
import numpy.polynomial.polynomial as poly

# poly.polyfit returns [A,B,C] for A + Bx + Cx^2 + Dx^3...

coefs_pp = poly.polyfit(df0.ams_p[df0.ams_p < 4.0], df0.press_dp[df0.ams_p < 4.0], 1)
ffit_pp = poly.polyval(df0.ams_p[df0.ams_p < 4.0], coefs_pp)

# fig = plt.figure()
ax2 = fig.add_subplot(122)
ax2.plot([0, 4], [0, 4], "m-")
ax2.plot(df0.ams_p, df0.press_dp, "b*")
ax2.plot(df0.ams_p[df0.ams_p < 4.0], ffit_pp, "r--")
ax2.set_title("MXP5004DP (y) vs AMS5915 (x)")
ax2.set_ylabel("MXP5004DP")
ax2.set_xlabel("AMS5915")
ax2.grid()
ax2.legend(
    [
        "1:1 Ideal",
        "Data",
        "y = " + str(round(coefs_pp[1], 3)) + "x + " + str(round(coefs_pp[0], 3)),
    ]
)

#%%
#   -----------------------------------------------------------------------------------------------------------
#       Part 2:
#       Plot dP vs Flow with fit line

import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.plot(df1.t, df1.Q_ptg, "g-")
ax1.plot(df1.t, df1.Q_ams, "r-")
ax1.plot(df1.t, df1.Q_press, "b-")
ax1.set_title("Flow vs Time")
ax1.set_ylabel("Flow (L/s)")
ax1.set_xlabel("time (sec)")
ax1.grid()
ax1.legend(["PTG", "MXP", "AMS"])


#%% Constrain fit domain to 0<dP<4.0
df2 = df1.copy(deep=True)
df2 = df2[df2.press_dp < 3.92]
df2 = df2[df2.press_dp > 0.01]
df2 = df2[df2.t > 17]
df2 = df2[df2.t < 40]

#%% Square Root Fit
import matplotlib.pyplot as plt
import numpy.polynomial.polynomial as poly
from scipy.optimize import curve_fit

# define the fit function
def aroot(x, a):
    import numpy as np

    return a * np.sqrt(x)


# find fitting coefficients for each of the 3 flow possiblities
popt_PTG, pcov_PTG = curve_fit(aroot, df2.press_dp, df2.Q_ptg)
popt_Qams, pcov_Qams = curve_fit(aroot, df2.ams_p, df2.Q_ams)
popt_Qmxp, pcov_Qmxp = curve_fit(aroot, df2.press_dp, df2.Q_press)

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.plot(df2.press_dp, df2.Q_ptg, "g*")
ax1.plot(df2.ams_p, df2.Q_ams, "r*")
ax1.plot(df2.press_dp, df2.Q_press, "b*")

PTGfit = ax1.plot(df2.press_dp, aroot(df2.press_dp, *popt_PTG), "g--")
AMSfit = ax1.plot(df2.press_dp, aroot(df2.ams_p, *popt_Qams), "r--")
MXPfit = ax1.plot(df2.press_dp, aroot(df2.press_dp, *popt_Qmxp), "b--")
ax1.set_title("Flow vs dP")
ax1.set_ylabel("Flow (L/s)")
ax1.set_xlabel("dP (kPa)")
ax1.grid()


PTG_fit_label = "PTG: " + str(round(popt_PTG[0], 3)) + "*sqrt(dP)"
AMS_fit_label = "PTG: " + str(round(popt_Qams[0], 3)) + "*sqrt(dP)"
MXP_fit_label = "MXP: " + str(round(popt_Qmxp[0], 3)) + "*sqrt(dP)"
# ax1.legend((MXPfit,PTGfit),(MXP_fit_label,PTG_fit_label))
ax1.legend(["PTG", "AMS", "MXP", PTG_fit_label, AMS_fit_label, MXP_fit_label])

#%% Quadratic Fit
import matplotlib.pyplot as plt
import numpy.polynomial.polynomial as poly
from scipy.optimize import curve_fit

# # define the fit function
# def asquare(x,a):
#     return x**2

# # find fitting coefficients for each of the 3 flow possiblities
# popt_PTG,pcov_PTG = curve_fit(asquare,df2.Q_ptg,df2.press_dp)
# popt_Qams,pcov_Qams = curve_fit(asquare,df2.Q_ams,df2.ams_p)
# popt_Qmxp,pcov_Qmxp = curve_fit(asquare,df2.Q_press,df2.press_dp)


import numpy.polynomial.polynomial as poly

coefs_ptg = poly.polyfit(df2.Q_ptg, df2.press_dp, 2)
ffit_ptg = poly.polyval(df2.Q_ptg, coefs_ptg)
coefs_ams = poly.polyfit(df2.Q_ams, df2.ams_p, 2)
ffit_ams = poly.polyval(df2.Q_ams, coefs_ams)
coefs_mxp = poly.polyfit(df2.Q_press, df2.press_dp, 2)
ffit_mxp = poly.polyval(df2.Q_press, coefs_mxp)


#
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.plot(df2.Q_ptg, df2.press_dp, "g*")
ax1.plot(df2.Q_ams, df2.ams_p, "r*")
ax1.plot(df2.Q_press, df2.press_dp, "b*")

PTGfit = ax1.plot(df2.Q_ptg, ffit_ptg, "g--")
AMSfit = ax1.plot(df2.Q_ams, ffit_ams, "r--")
MXPfit = ax1.plot(df2.Q_press, ffit_mxp, "b--")
ax1.set_title("Flow vs dP")
ax1.set_xlabel("Flow (L/s)")
ax1.set_ylabel("dP (kPa)")
ax1.grid()

# PTG_fit_label = 'PTG: ' + str(round(popt_PTG[0],3)) + '*sqrt(dP)'
# AMS_fit_label = 'PTG: ' + str(round(popt_Qams[0],3)) + '*sqrt(dP)'
# MXP_fit_label = 'MXP: ' + str(round(popt_Qmxp[0],3)) + '*sqrt(dP)'
# #ax1.legend((MXPfit,PTGfit),(MXP_fit_label,PTG_fit_label))
# ax1.legend(['PTG','AMS','MXP',PTG_fit_label,AMS_fit_label,MXP_fit_label])

PTG_fit_label = (
    "PTG: "
    + str(round(coefs_ptg[0], 4))
    + " + "
    + str(round(coefs_ptg[1], 4))
    + "x + "
    + str(round(coefs_ptg[2], 4))
    + "x^2"
)
AMS_fit_label = (
    "AMS: "
    + str(round(coefs_ams[0], 4))
    + " + "
    + str(round(coefs_ams[1], 4))
    + "x + "
    + str(round(coefs_ams[2], 4))
    + "x^2"
)
MXP_fit_label = (
    "MXP: "
    + str(round(coefs_mxp[0], 4))
    + " + "
    + str(round(coefs_mxp[1], 4))
    + "x + "
    + str(round(coefs_mxp[2], 4))
    + "x^2"
)
# ax1.legend((MXPfit,PTGfit),(MXP_fit_label,PTG_fit_label))
ax1.legend(["PTG", "AMS", "MXP", PTG_fit_label, AMS_fit_label, MXP_fit_label])

#%% Linear Regression

coefs_Qmxp = poly.polyfit(df2.Q_ptg, df2.Q_press, 1)
ffit_Qmxp = poly.polyval(df2.Q_ptg, coefs_Qmxp)
coefs_Qams = poly.polyfit(df2.Q_ptg, df2.Q_ams, 1)
ffit_Qams = poly.polyval(df2.Q_ptg, coefs_Qams)

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.plot([0, 2], [0, 2], "m-")
ax1.plot(df2.Q_ptg, df2.Q_ams, "r*")
ax1.plot(df2.Q_ptg, ffit_Qams, "r-", lw=3)
ax1.plot(df2.Q_ptg, df2.Q_press, "b*")
ax1.plot(df2.Q_ptg, ffit_Qmxp, "b-", lw=3)
ax1.set_title("Q_MXP5004DP and AMS (y) vs Q_PTG (x)")
ax1.set_ylabel("Flow (L/s) for MXP5004D and AMS5915")
ax1.set_xlabel("Q_PTG")
ax1.grid()
ams_reg_label = (
    "AMS sensor regression: y = "
    + str(round(coefs_Qams[1], 3))
    + "x + "
    + str(round(coefs_Qams[0], 3))
)
mxp_reg_label = (
    "MXP sensor regression: y = "
    + str(round(coefs_Qmxp[1], 3))
    + "x + "
    + str(round(coefs_Qmxp[0], 3))
)
ax1.legend(["1:1 Ideal", "Data", ams_reg_label, "Data", mxp_reg_label])
