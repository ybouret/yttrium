//! \file

#ifndef Y_MKL_ROOT_ZFind_Included
#define Y_MKL_ROOT_ZFind_Included 1


#include "y/mkl/triplet.hpp"
#include "y/mkl/function-wrapper-1d.hpp"
#include "y/type/identifiable.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class ZFind : public Identifiable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Function<T,T> FunctionType; //!< alias
            struct  Handle {
                T *neg;
                T *pos;
            };


            virtual void lookup(Triplet<T> &x, Triplet<T> &f, FunctionType &F) = 0;
           
            template <typename FUNCTION>   inline
            void operator()(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                lookup(x,f,FW);
            }

            virtual ~ZFind() noexcept;

        protected:
            explicit ZFind() noexcept;
            const T half;
            //! return true if initial value is zero
            bool found(Handle &xp, Handle &fp, Triplet<T> &x, Triplet<T> &f, FunctionType &F);




        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZFind);


        };

    }

}



#endif
