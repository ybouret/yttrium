


#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(32)
{
    Y_UTEST(triplet);    
    
    Y_UTEST(ode_rk4);
 
    Y_UTEST(opt_parabolic);
}
Y_UTEST_EXEC()

