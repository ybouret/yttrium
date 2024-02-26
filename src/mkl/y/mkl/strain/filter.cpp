#include "y/mkl/strain/filter.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/v2d.hpp"
#include "y/sequence/vector.hpp"
#include "y/container/cxx/array.hpp"
#include "y/calculus/ipower.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            FilterInfo:: ~FilterInfo() noexcept {}
            FilterInfo::  FilterInfo() noexcept {}

            const char * const FilterInfo :: CallSign = "Filter";

            template <typename T>
            class Filter<T>:: Code : public Object
            {
            public:
                typedef V2D<T>                       Point;
                typedef Vector<Point,Memory::Dyadic> Points;
                typedef Antelope::Add<T>             XAdd;
                typedef CxxArray<T,Memory::Dyadic>   Coef;

                inline explicit Code() :
                points(), 
                solver(),
                xadd(),
                coef(FilterInfo::SIZE)
                {}

                inline virtual ~Code() noexcept {}

                inline  void eval(const T &x0, const size_t degree)
                {
                    static const T zero(0);
                    const size_t np = points.size();
                    coef.ld(zero);
                    switch(np)
                    {
                        case 0: return;
                        case 1: coef[1] = points[1].y; return;
                        default:
                            break;
                    }

                    // coefficients to compute
                    const size_t ncof = Min(degree+1,np);
                    const T      den(np);
                    Coef         poly(ncof);

                    // recenter data
                    xadd.make(np);
                    for(size_t i=np;i>0;--i) xadd << points[i].x;
                    const T xm = xadd.sum()/den;

                    assert(xadd.isEmpty());
                    for(size_t i=np;i>0;--i) xadd << points[i].y;
                    const T ym = xadd.sum()/den;

                    // compute rhs
                    for(size_t j=ncof;j>0;--j)
                    {
                        assert(xadd.isEmpty());
                        const size_t k=j-1;
                        for(size_t i=np;i>0;--i)
                        {
                            const Point p = points[i];
                            const T     r = (p.y-ym) * ipower(p.x-xm,k);
                            xadd << r;
                        }
                        poly[j] = xadd.sum();
                    }

                    Matrix<T> mu(ncof,ncof);
                    mu[1][1] = ncof;
                    const size_t maxPower = (ncof-1)*2;
                    for(size_t k=maxPower;k>0;--k)
                    {
                        assert(xadd.isEmpty());
                        for(size_t i=np;i>0;--i)
                        {
                            xadd << ipower(points[i].x-xm,k);
                        }
                    }

                }


                Points  points;
                LU<T>   solver;
                XAdd    xadd;
                Coef    coef;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
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
