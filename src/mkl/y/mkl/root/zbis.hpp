
//! \file

#ifndef Y_MKL_ROOT_ZBis_Included
#define Y_MKL_ROOT_ZBis_Included 1


#include "y/mkl/root/zfind.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Bisection
        //
        //
        //______________________________________________________________________
        template <typename T>
        class ZBis : public ZFind<T>
        {
        public:
            typedef typename ZFind<T>::FunctionType FunctionType; //!< alias

            explicit ZBis() noexcept; //!< setup
            virtual ~ZBis() noexcept; //!< cleanup

            virtual const char *callSign() const noexcept;                             //!< "MKL::ZBis"
            virtual void        lookup(Triplet<T> &x, Triplet<T> &f, FunctionType &F); //!< algorithm


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZBis);
        };

    }

}



#endif
