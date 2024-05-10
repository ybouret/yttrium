
#include "y/chemical/reactive/plexus/limits.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(limits)
{
    Species proton("H+",1,1);
    Species hydroxy("OH-",-1,2);
    Species AH("AH",0,3);

    LimitsBank lbank;
    SBank      sbank;

    Limits     limits(lbank,sbank);

    limits(proton,0.1);
    limits(hydroxy,0.2);
    
    std::cerr << "limits=" << limits << std::endl;
   
    limits.free();
    limits(hydroxy,0.2);
    limits(proton,0.1);
    std::cerr << "limits=" << limits << std::endl;


    limits.free();
    limits(hydroxy,0.2);
    limits(proton,0.2);
    std::cerr << "limits=" << limits << std::endl;


}
Y_UDONE()
