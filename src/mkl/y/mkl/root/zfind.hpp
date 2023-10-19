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
        //______________________________________________________________________
        //
        //
        //
        //! Zero-Finder API
        //
        //
        //______________________________________________________________________
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

            //! helper to locate negative/positive end
            struct  Handle {
                T *neg; //!< pointer to negative end
                T *pos; //!< pointer to positive end
            };

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! lookup interface
            virtual void lookup(Triplet<T> &x, Triplet<T> &f, FunctionType &F) = 0;
           
            //! wrapper to callable FUNCTION
            template <typename FUNCTION>   inline
            void operator()(FUNCTION &F, Triplet<T> &x, Triplet<T> &f )
            {
                Wrapper1D<T,T,FUNCTION> FW(F);
                lookup(x,f,FW);
            }
            
            //! return true if initial value is zero, oherwise set handles accordinly
            bool found(Handle &xp, Handle &fp, Triplet<T> &x, Triplet<T> &f, FunctionType &F);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~ZFind() noexcept; //!< cleanup

        protected:
            explicit ZFind() noexcept; //!< setup
            const T half;              //!< helper

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZFind);


        };

    }

}



#endif
