
#include "y/mkl/eigen/diagonalization.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/utils.hpp"
#include "y/mkl/numeric.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/mkl/antelope/sum3.hpp"
#include "y/mkl/algebra/svd.hpp"

#include "y/sort/heap.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/exception.hpp"
#include "y/container/cxx/array.hpp"

#include <cfloat>

namespace Yttrium
{
    namespace MKL
    {

        namespace Eigen
        {


            template <typename T>
            class Diagonalization<T> :: Code : public Object
            {
            public:
                typedef Memory::Pooled     Model;
                typedef Complex<T>         Cplx;
                typedef Vector<T,Model>    VectorR;
                typedef Vector<Cplx,Model> VectorC;

                typedef Antelope::Add<T>   XAdd;

                const T         zero;
                const T         one;
                XAdd            radd;
                XAdd            cadd;
                VectorR         wr;
                VectorR         wi;
                VectorC         wc;
                const Values<T> values;
                SVD<T>          svd;

                inline explicit Code() :
                Object(),
                zero(0),
                one(1),
                radd(),
                cadd(),
                wr(),
                wi(),
                wc(),
                values(wr,wc),
                svd()
                {
                }


#include "diag/balance.hxx"
#include "diag/reduce.hxx"
#include "diag/qr.hxx"
               


             

                inline const Values<T> * eig(Matrix<T> &a)
                {
                    assert(a.isSquare());
                    assert(a.rows>0);
                    //----------------------------------------------------------
                    //
                    // preparing
                    //
                    //----------------------------------------------------------
                    const size_t n = a.rows;
                    wr.adjust(n,zero);
                    wi.adjust(n,zero);
                    wc.free();

                    //----------------------------------------------------------
                    //
                    // balance/reduce to Hessenber form
                    //
                    //----------------------------------------------------------
                    balance(a);
                    reduce(a);
                    size_t nr = 0;
                    try {

                        if( QR(a, wr, wi, nr ) )
                        {
                            assert(nr<=n);

                            //------------------------------------------------------
                            // extract complex values
                            //------------------------------------------------------
                            wc.ensure(n-nr);
                            for(size_t j=1+nr;j<=n;++j)
                            {
                                const Cplx z(wr[j],wi[j]);
                                wc << z;
                            }

                            //------------------------------------------------------
                            // trim real part of complex values
                            //------------------------------------------------------
                            while( wr.size() > nr ) wr.popTail();
                            assert(values.wr.size() + values.wc.size() == n);
                            return &values;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    catch(...)
                    {
                        wr.free();
                        wi.free();
                        wc.free();
                        throw;
                    }

                }

                inline void guess(Writable<T>            &eVal,
                                  Matrix<T>              &eVec,
                                  const Readable<size_t> &eIdx,
                                  const Matrix<T>        &a)
                {
                    static const char fn[] = "Diagonalize::guess";

                    assert(a.isSquare());
                    assert(eVec.rows   == eVal.size());
                    assert(eIdx.size() == eVal.size());
                    assert(eVec.cols   == a.cols);
                    const size_t n  = a.rows;
                    const size_t nv = eVal.size();
                    const size_t nr = wr.size();
                    Matrix<T>    A(a);
                    Matrix<T>    u(n,n);
                    Matrix<T>    v(n,n);
                    CxxArray<T>  w(n);

                    for(size_t iv=1;iv<=nv;++iv)
                    {
                        const size_t k   = eIdx[iv]; if(k<1||k>nr) throw Specific::Exception(fn, "index=%u not in [1:%u]", unsigned(k), unsigned(nr));
                        const T      lam = eVal[iv] = wr[k];
                        //std::cerr << "\tlam" << iv << "=" << lam << std::endl;
                        for(size_t i=n;i>0;--i)
                        {
                            A[i][i] = a[i][i] - lam;
                        }
                        if(!svd.build(u,w,v,A))
                            throw Specific::Exception(fn,"no SVD for eigenvalue %.15g", double(lam));
                        //std::cerr << "\t#w=" << w << std::endl;
                        //std::cerr << "\tv=" << v << std::endl;

                        size_t imin = 1;
                        T      amin = Fabs<T>::Of(w[1]);
                        for(size_t i=n;i>1;--i)
                        {
                            const T atmp = Fabs<T>::Of(w[i]);
                            if(atmp<amin)
                            {
                                amin = atmp;
                                imin = i;
                            }
                        }
                        //std::cerr << "\t#|w|_min=" << amin << " @" << imin << std::endl;
                        for(size_t i=n;i>0;--i)
                        {
                            eVec[iv][i] = v[i][imin];
                        }
                        std::cerr << "\tv" << iv << "=" << eVec[iv] << std::endl;

                    }


                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

#define real_t float
#include "diagonalization.hxx"

#undef  real_t
#define real_t double
#include "diagonalization.hxx"

#undef  real_t
#define real_t long double
#include "diagonalization.hxx"

#undef  real_t
#define real_t XReal<float>
#include "diagonalization.hxx"

#undef  real_t
#define real_t XReal<double>
#include "diagonalization.hxx"

#undef  real_t
#define real_t XReal<long double>
#include "diagonalization.hxx"
        }

    }

}

