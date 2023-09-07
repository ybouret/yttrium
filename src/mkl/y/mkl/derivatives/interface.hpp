//! \file

#ifndef Y_MKL_Derivatives_Interface_Included
#define Y_MKL_Derivatives_Interface_Included 1

#include "y/object.hpp"
#include "y/counted.hpp"
#include "y/mkl/function-wrapper.hpp"
#include "y/mkl/interval.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Kernel
        {
            //__________________________________________________________________
            //
            //
            //
            //! Common code for Derivatives
            //
            //
            //__________________________________________________________________
            class Derivatives : public Object, public Counted
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< Derivatives
                

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                static void UnderflowException();         //!< step size too small
                static void SingularFunctionException();  //!< unexpected function behavior
                static void OutOfDomainException();       //!< data point is out of definition domain

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Derivatives() noexcept; //!< cleanup
            protected:
                explicit Derivatives() noexcept; //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Derivatives);
            };
        }


        //______________________________________________________________________
        //
        //
        //
        //! Numerical Derivatives with Interval control
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Derivatives : public Kernel::Derivatives
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Function<T,T> FunctionType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Derivatives();            //!< setup
            virtual ~Derivatives() noexcept;   //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! internal computation by modified Ridder's Method
            T compute(FunctionType &F, const T x0, const T h, const Interval<T> &I);


            //! code wrapper
            template <typename FUNCTION> inline
            T computeFor(FUNCTION &F, const T x0, const T h, const Interval<T> &I)
            {
                Wrapper<T,T,FUNCTION> FW(F);
                return compute(FW,x0,h,I);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Derivatives);
            class Code;
            Code *code;
        };

    }
}

#endif

