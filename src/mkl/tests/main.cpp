#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(64)
{
    Y_UTEST(triplet);    
    Y_UTEST(interval);
    Y_UTEST(intervals);
    Y_UTEST(utils);
    Y_UTEST(tao0);
    Y_UTEST(tao1);
    Y_UTEST(tao2);
    Y_UTEST(tao3);
    Y_UTEST(taoTr);
    Y_UTEST(trigconv);

    
    Y_UTEST(ode_rk4);
    Y_UTEST(odeint);
    Y_UTEST(ode_scheme45);

 
    Y_UTEST(opt_bracket);
    Y_UTEST(opt_minimize);

    Y_UTEST(algebra_lu);
    Y_UTEST(algebra_ortho_space);
    Y_UTEST(algebra_tridiag);
    Y_UTEST(algebra_cyclic);
    Y_UTEST(algebra_svd);

    Y_UTEST(drvs1D);
    Y_UTEST(drvsND);

    Y_UTEST(interp_poly);
    Y_UTEST(interp_ratio);

    Y_UTEST(io_data_stream);


    Y_UTEST(filter_smooth);
    Y_UTEST(filter_arc);

    Y_UTEST(root_zfind);


    Y_UTEST(eigen_jacobi);
    Y_UTEST(eigen_diag);

    Y_UTEST(stats_descr);
   
    Y_UTEST(fit_samples);
    Y_UTEST(fit_zbode);

}
Y_UTEST_EXEC()

