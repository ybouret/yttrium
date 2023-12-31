//! \file

#ifndef Y_MKL_ROOT_ZRid_Included
#define Y_MKL_ROOT_ZRid_Included 1


#include "y/mkl/root/zfind.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Bisection+Ridder's Method
        //
        //
        //______________________________________________________________________
        template <typename T>
        class ZRid : public ZFind<T>
        {
        public:
            typedef typename ZFind<T>::FunctionType FunctionType; //!< alias

            explicit ZRid() noexcept; //!< setup
            virtual ~ZRid() noexcept; //!< cleanup

            virtual const char *callSign() const noexcept;                             //!< "MKL::ZRid"
            virtual void        lookup(Triplet<T> &x, Triplet<T> &f, FunctionType &F); //!< algorithm


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZRid);
        };

    }

}



#endif
