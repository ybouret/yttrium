#include "y/mkl/strain/filter.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/v4d.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/cxx/array.hpp"
#include "y/calculus/ipower.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            FilterInfo:: ~FilterInfo() noexcept {}
            FilterInfo::  FilterInfo() noexcept {}

            const char * const FilterInfo :: CallSign = "Strain::Filter";

            template <typename T>
            class Filter<T>:: Code : public Object
            {
            public:
                typedef V4D<T>                       Point;
                typedef Vector<Point,Memory::Dyadic> Points;
                typedef Antelope::Add<T>             XAdd;
                typedef CxxArray<T,Memory::Dyadic>   Coef;

                inline explicit Code() :
                points(), 
                solver(),
                xadd(),
                coef(FilterInfo::SIZE),
                zero(0),
                half(0.5)
                {}

                inline virtual ~Code() noexcept {}

                inline  void eval(const T &x0, const size_t degree)
                {
                    //__________________________________________________________
                    //
                    //
                    // initialize
                    //
                    //__________________________________________________________
                    const size_t np = points.size();
                    coef.ld(zero);
                    switch(np)
                    {
                        case 0: return;
                        case 1: coef[1] = points[1].y; return;
                        default:
                            break;
                    }

                    //__________________________________________________________
                    //
                    //
                    // coefficients to compute
                    //
                    //__________________________________________________________
                    const size_t ncof = Min(degree+1,np);
                    const T      den(np);
                    Coef         poly(ncof);

                    //__________________________________________________________
                    //
                    //
                    // recenter data x->z, y->w
                    //
                    //__________________________________________________________
                    xadd.make(np);
                    for(size_t i=np;i>0;--i) 
                    {
                        Point &p = points[i];
                        p.z      = p.x - x0;
                        xadd << points[i].y;
                    }
                    const T ym = xadd.sum()/den;
                    for(size_t i=np;i>0;--i)
                    {
                        Point &p = points[i];
                        p.w = p.y - ym;
                    }

                    //__________________________________________________________
                    //
                    //
                    // compute rhs, rhs[1] = 0;
                    //
                    //__________________________________________________________
                    for(size_t j=ncof;j>1;--j)
                    {
                        assert(xadd.isEmpty());
                        const size_t k=j-1;
                        for(size_t i=np;i>0;--i)
                        {
                            const Point p = points[i];
                            const T     r = p.w * ipower(p.z,k);
                            xadd << r;
                        }
                        poly[j] = xadd.sum();
                    }

                    //__________________________________________________________
                    //
                    //
                    // compute moments
                    //
                    //__________________________________________________________
                    Matrix<T> mu(ncof,ncof);
                    buildMu(mu);

                    if(!solver.build(mu))
                    {
                        buildMu(mu);
                        std::cerr << "mu="  << mu << std::endl;
                        std::cerr << "rhs=" << poly << std::endl;
                        throw Specific::Exception(CallSign, "singular #points=%u/ncof=%u @%.15g", unsigned(np), unsigned(ncof), double(x0));
                    }

                    solver.solve(mu,poly);
                    /**/        coef[1] = poly[1] + ym;
                    if(ncof>=2) coef[2] = poly[2];
                    if(ncof>=3) coef[3] = half * poly[3];

                }


                Points  points;
                LU<T>   solver;
                XAdd    xadd;
                Coef    coef;
                const T zero;
                const T half;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
                inline void buildMu(Matrix<T> &mu)
                {
                    const size_t ncof = mu.rows;
                    const size_t npts = points.size();
                    for(size_t i=1;i<=ncof;++i)
                    {
                        for(size_t j=i;j<=ncof;++j)
                        {
                            const size_t k = (i-1)+(j-1);
                            assert(xadd.isEmpty());
                            for(size_t i=npts;i>0;--i)
                            {
                                xadd << ipower(points[i].z,k);
                            }
                            mu[i][j] = mu[j][i] = xadd.sum();
                        }
                    }
                    assert( xadd.isEmpty() );
                }
            };

        }
    }
}


#define real_t float
#include "filter.hxx"
#undef real_t

#define real_t double
#include "filter.hxx"
#undef real_t

#define real_t long double
#include "filter.hxx"
#undef real_t

#define real_t XReal<float>
#include "filter.hxx"
#undef real_t

#define real_t XReal<double>
#include "filter.hxx"
#undef real_t

#define real_t XReal<long double>
#include "filter.hxx"
#undef real_t
