

#include "y/information/bwt/bwt.h"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;



Y_UTEST(info_bwt)
{

    if(argc>1)
    {

        const String   input = argv[1];
        const size_t   size  = input.size();
        if(size>0)
        {
            String         output(size,AsCapacity,true);
            Vector<size_t> indx(size,0);
            const size_t pidx = Y_BWT_Encode(&output[1],&input[1],size, indx());

            String decoded(size,AsCapacity,true);
            Y_BWT_Decode(&decoded[1], &output[1], size, indx(), pidx);
            std::cerr << input << " -> " << output << "@" << pidx << " -> " << decoded << std::endl;
            
        }
    }


}
Y_UDONE()

