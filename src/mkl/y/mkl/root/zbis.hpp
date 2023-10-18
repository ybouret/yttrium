
//! \file

#ifndef Y_MKL_ROOT_ZBis_Included
#define Y_MKL_ROOT_ZBis_Included 1


#include "y/mkl/root/zfind.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class ZBis : public ZFind<T>
        {
        public:
            typedef typename ZFind<T>::FunctionType FunctionType;
            
            explicit ZBis() noexcept;
            virtual ~ZBis() noexcept;
            
            virtual void lookup(Triplet<T> &x, Triplet<T> &f, FunctionType &F);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZBis);
        };

    }

}



#endif
