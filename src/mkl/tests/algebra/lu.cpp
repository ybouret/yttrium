


#include "y/mkl/algebra/lu.hpp"
#include "y/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "y/container/cxx-array.hpp"
#include "y/system/rtti.hpp"

using namespace Yttrium;
using namespace MKL;


namespace
{
    template <typename T>
    static inline void testLU(Random::Bits &ran, const size_t nmax=10)
    {
        std::cerr << "---------- Testing LU< " << TypeName<T>() << " > ----------" << std::endl;
        MKL::LU<T> lu;

        for(size_t n=1;n<=nmax;++n)
        {
            std::cerr << "\tn = " << n << std::endl;
            Matrix<T> a(n,n);
            Matrix<T> a0(n,n);

            for(size_t loop=0;loop<3;++loop)
            {
            FILL:
                FillMatrix(a,ran);
                a0.assign(a);
                if(!lu.build(a)) goto FILL;

                CxxArray<T,Memory::Dyadic> r(n); FillWritable(r,ran); // rhs
                CxxArray<T,Memory::Dyadic> b(r); lu.solve(a,b);       // b = a^-1 * r
                CxxArray<T,Memory::Dyadic> p(n); a0.mul(p,b);         // p = a*b = r

                typename ScalarFor<T>::Type delta = 0, zero = 0;
                for(size_t i=n;i>0;--i)
                {
                    delta += Fabs<T>::Of(p[i]-r[i]);
                }
                delta /= n;

                if( typeid(T) == typeid(apq) )
                {
                    Y_CHECK(delta<=zero);
                }
                else
                {
                    std::cerr << "delta=" << delta << std::endl;
                }
            }

            {
                // manual inversion
                Matrix<T> M(n,n);
                for(size_t i=1;i<=n;++i) M[i][i] = T(1);
                lu.solve(a,M);
                Matrix<T> P(n,n);
                a0.mmul(P,M);
                typename ScalarFor<T>::Type delta = 0, zero = 0;
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        if(i==j)
                        {
                            const T one(1);
                            delta += Fabs<T>::Of(P[i][j]-one);
                        }
                        else
                        {
                            delta += Fabs<T>::Of(P[i][j]);
                        }

                    }
                }
                delta /= (n*n);
                std::cerr << "manual  : ";
                if( typeid(T) == typeid(apq) )
                {
                    Y_CHECK(delta<=zero);
                }
                else
                {
                    std::cerr << "delta=" << delta << std::endl;
                }
            }

            {
                // auto inversion
                Matrix<T> I(n,n);
                lu.invert(a,I);
                Matrix<T> P(n,n);
                a0.mmul(P,I);
                typename ScalarFor<T>::Type delta = 0, zero = 0;
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        if(i==j)
                        {
                            const T one(1);
                            delta += Fabs<T>::Of(P[i][j]-one);
                        }
                        else
                        {
                            delta += Fabs<T>::Of(P[i][j]);
                        }

                    }
                }
                delta /= (n*n);
                std::cerr << "invert  : ";
                if( typeid(T) == typeid(apq) )
                {
                    Y_CHECK(delta<=zero);
                }
                else
                {
                    std::cerr << "delta=" << delta << std::endl;
                }
            }

            {
                // adjoint and determinant
                const T    detA = lu.determinant(a);
                Matrix<T> adjA(n,n);
                lu.adjoint(adjA,a0);

                Matrix<T> P(n,n);
                a0.mmul(P,adjA);
                P /= detA;
                typename ScalarFor<T>::Type delta = 0, zero = 0;
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        if(i==j)
                        {
                            const T one(1);
                            delta += Fabs<T>::Of(P[i][j]-one);
                        }
                        else
                        {
                            delta += Fabs<T>::Of(P[i][j]);
                        }

                    }
                }
                delta /= (n*n);
                std::cerr << "adjoint : ";
                if( typeid(T) == typeid(apq) )
                {
                    Y_CHECK(delta<=zero);
                }
                else
                {
                    std::cerr << "delta=" << delta << std::endl;
                }

            }

            std::cerr << std::endl;


        }
    }
}

Y_UTEST(algebra_lu)
{
    Random::Rand    ran;

    testLU<float>(ran);
    testLU<double>(ran);
    testLU<long double>(ran);

    testLU< XReal<float>       >(ran);
    testLU< XReal<double>      >(ran);
    testLU< XReal<long double> >(ran);

    testLU< Complex<float>       >(ran);
    testLU< Complex<double>      >(ran);
    testLU< Complex<long double> >(ran);

    testLU< Complex< XReal<float> >       >(ran);
    testLU< Complex< XReal<double> >      >(ran);
    testLU< Complex< XReal<long double> > >(ran);

    testLU<apq>(ran,4);
    

}
Y_UDONE()
