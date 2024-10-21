#include "y/utest/run.hpp"
#include <cstdio>
#include <cstring>

using namespace Yttrium;



Y_UTEST(nbits)
{
    static const uint8_t one = 1;
    uint8_t byte=0;
    for(unsigned i=0;i<16;++i)
    {
        for(unsigned j=0;j<16;++j,++byte)
        {
            unsigned n = 0;
            for(size_t p=0;p<8;++p)
            {
                if( 0 != ( byte & (one<<p)) ) ++n;
            }
            //std::cerr << int(byte) << " => " << n << std::endl;
            fprintf(stdout,"%1u",n);
            if(byte<255) fprintf(stdout, ", ");
        }
        fprintf(stdout,"\n");
    }
}
Y_UDONE()
