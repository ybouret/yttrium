""" minimal code to check valid real value """
import numpy as np


def check_valid_real(x, variable_name):
    """ perform assertions """
    assert (not np.isinf(x)) and (not np.isnan(x)), "invalid " + variable_name


check_valid_real(0.0, "x")
check_valid_real(1.0, "y")
check_valid_real(np.NaN, "z")
