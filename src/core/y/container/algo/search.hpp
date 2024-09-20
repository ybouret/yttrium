//! \file

#ifndef Y_Container_Algo_Search_Included
#define Y_Container_Algo_Search_Included 1

#include "y/sequence/interface.hpp"

namespace Yttrium
{
    namespace Algo
    {
        //______________________________________________________________________
        //
        //
        //! Brute Force Generic search
        /**
         \param output sequence of indices
         \param source 'Readable' source
         \param figure 'Readable' figure to find
         */
        //
        //______________________________________________________________________
        template <
        typename OUTPUT,
        typename SOURCE,
        typename FIGURE>
        inline void Search(OUTPUT &output,
                           SOURCE &source,
                           FIGURE &figure)
        {
            const size_t n = source.size();
            const size_t m = figure.size();
            for(size_t i=1,j=m;j<=n;++i,++j)
            {
                bool match = true;
                for(size_t k=j,l=m;l>0;--k,--l)
                {
                    if( source[k] != figure[l] ) {
                        match = false;
                        break;
                    }
                }
                if(!match) continue;
                output << i;
            }
        }
    }

}
#endif
