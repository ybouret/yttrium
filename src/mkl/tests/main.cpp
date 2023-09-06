


#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(32)
{
    Y_UTEST(triplet);    
    Y_UTEST(interval);
    
    Y_UTEST(ode_rk4);
 
    Y_UTEST(opt_parabolic);
    Y_UTEST(opt_bracket);

    Y_UTEST(algebra_lu);
    Y_UTEST(algebra_ortho_space);

    Y_UTEST(drvs_1d);

}
Y_UTEST_EXEC()

