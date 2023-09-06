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
        template <typename> struct Triplet;

        namespace Kernel
        {
            class Derivatives : public Object, public Counted
            {
            public:
                static const char * const CallSign;
                
                virtual ~Derivatives() noexcept;

                static void UnderflowException();
                static void SingularFunctionException();
                static void OutOfDomainException();


            protected:
                explicit Derivatives() noexcept;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Derivatives);
            };
        }

        template <typename T>
        class Derivatives : public Kernel::Derivatives
        {
        public:
            typedef Function<T,T> FunctionType;

            explicit Derivatives();
            virtual ~Derivatives() noexcept;

            T eval_(FunctionType &F, const T xlo, const T x, const T xhi);

            void setMetrics( Triplet<T> &, const T x0, T &length, const Interval<T> &I) const;
            

            template <typename FUNCTION> inline
            T eval(FUNCTION &F, const T xlo, const T x, const T xhi)
            {
                Wrapper<T,T,FUNCTION> FW(F);
                return eval_(FW,xlo,x,xhi);
            }

            T compute_(FunctionType &F, const T x0, const T h, const Interval<T> &I);

            template <typename FUNCTION> inline
            T compute(FUNCTION &F, const T x0, const T h, const Interval<T> &I)
            {
                Wrapper<T,T,FUNCTION> FW(F);
                return compute_(FW,x0,h,I);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Derivatives);
            class Code;

            Code *code;
        };

    }
}

#endif

