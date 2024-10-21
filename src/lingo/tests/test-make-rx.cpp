#include "y/utest/run.hpp"
#include <cstdio>
#include <cstring>

using namespace Yttrium;



Y_UTEST(makerx)
{
    static const char extra[] = "_ #@";

    int c = 0;
    for(unsigned i=0;i<16;++i)
    {
        for(unsigned j=0;j<16;++j,++c)
        {
            if( c>0 && (isalnum(c) || 0!= strchr(extra,c)) )
            {
                fprintf(stdout,"\"%c\"    ",c);
            }
            else
            {
                fprintf(stdout,"\"\\\\x%02x\"",c);
            }
            if(c<255) fprintf(stdout, ", ");
        }
        fprintf(stdout,"\n");
    }

    


}
Y_UDONE()
