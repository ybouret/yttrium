
#ifndef Y_MKL_Tao1_Included
#define Y_MKL_Tao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {
            //__________________________________________________________________
            //
            //! target[1..target.size()] = source[1..target.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Load(TARGET &target, SOURCE &source)
            {
                assert(target.size()<=source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] = source[i];
            }

        }

    }

}

#endif
