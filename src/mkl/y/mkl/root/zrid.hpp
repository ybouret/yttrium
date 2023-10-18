//! \file

#ifndef Y_MKL_ROOT_ZRid_Included
#define Y_MKL_ROOT_ZRid_Included 1


#include "y/mkl/root/zfind.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class ZRid : public ZFind<T>
        {
        public:
            typedef typename ZFind<T>::FunctionType FunctionType;

            explicit ZRid() noexcept;
            virtual ~ZRid() noexcept;

            virtual const char *callSign() const noexcept;
            virtual void        lookup(Triplet<T> &x, Triplet<T> &f, FunctionType &F);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZRid);
        };

    }

}



#endif
