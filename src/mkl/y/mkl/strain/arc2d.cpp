
#include "y/mkl/strain/arc2d.hpp"
#include "y/mkl/strain/filter.hpp"
#include "y/type/nullify.hpp"
#include "y/object.hpp"
#include "y/mkl/xreal.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/string.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            const char * const Arc2DInfo:: CallSign = "Strain::Arc2D";

            Arc2DInfo:: Arc2DInfo() noexcept
            {
            }

            Arc2DInfo:: ~Arc2DInfo() noexcept
            {
            }

            template <typename T>
            class Arc2D<T>:: Code : public Object
            {
            public:
                typedef CxxArray<Vertex,Memory::Dyadic> Coef2D;


                inline explicit Code() :
                Object(),
                xf(),
                yf(),
                cf(SIZE),
                v0()
                {}

                inline virtual ~Code() noexcept {}

                inline void free() noexcept
                {
                    yf.free();
                    xf.free();
                    cf.ld(v0);
                }

                inline void eval(const T t0, const size_t xdg, const size_t ydg)
                {
                    //----------------------------------------------------------
                    // initialize coefficients
                    //----------------------------------------------------------
                    cf.ld(v0);

                    //----------------------------------------------------------
                    // query each filter
                    //----------------------------------------------------------
                    assert(xf.load() == yf.load());
                    xf.eval(t0,xdg);
                    yf.eval(t0,ydg);

                    //----------------------------------------------------------
                    // fill information
                    //----------------------------------------------------------
                    for(size_t i=SIZE;i>0;--i)
                        cf[i] =  Vertex( xf[i], yf[i] );
                }

                Filter<T>    xf; //!< filter for x
                Filter<T>    yf; //!< filter for y
                Coef2D       cf; //!< cf[1..SIZE]
                const Vertex v0; //!< (0,0)
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

        }

    }

}

#define real_t float
#include "arc2d.hxx"
#undef real_t

#define real_t double
#include "arc2d.hxx"
#undef real_t

#define real_t long double
#include "arc2d.hxx"
#undef real_t

#define real_t XReal<float>
#include "arc2d.hxx"
#undef real_t

#define real_t XReal<double>
#include "arc2d.hxx"
#undef real_t

#define real_t XReal<long double>
#include "arc2d.hxx"
#undef real_t
