
#include "y/mkl/filter/smooth-arc.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/filter/smooth.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace MKL
    {

        template <typename T>
        class SmoothArc<T>:: Code : public Object
        {
        public:
            typedef V2D<T>                   Vtx;
            typedef Vector<T,Memory::Dyadic> Tableau;

            inline explicit Code() :
            sm(),
            zero(0),
            r(zero,zero),
            v(zero,zero),
            d(zero,zero),
            curvature(zero),
            coord()
            {}
            
            inline virtual ~Code() noexcept {}


            void run(const T             &t0,
                     const Readable<T>   &t,
                     const Readable<T>   &x,
                     const Readable<T>   &y,
                     const size_t         xdegree,
                     const size_t         ydegree,
                     const SmoothGeometry geometry)
            {
                assert( t.size() == x.size() );
                assert( t.size() == y.size() );
                sm(t0,t,x,xdegree);
                r.x = sm[1];
                v.x = sm[2];
                d.x = sm[3];

                sm(t0,t,y,ydegree);
                r.y = sm[1];
                v.y = sm[2];
                d.y = sm[3];

                computeCurvature(geometry);
            }

            void run(const T             &t0,
                     const Readable<T>   &t,
                     const Readable<Vtx> &p,
                     const size_t         xdegree,
                     const size_t         ydegree,
                     const SmoothGeometry geometry)
            {
                assert(t.size() == p.size() );
                const size_t n = t.size();
                coord.adjust(n,zero);

                for(size_t i=n;i>0;--i) coord[i] = p[i].x;

                sm(t0,t,coord,xdegree);
                r.x = sm[1];
                v.x = sm[2];
                d.x = sm[3];

                for(size_t i=n;i>0;--i) coord[i] = p[i].y;
                sm(t0,t,coord,ydegree);
                r.y = sm[1];
                v.y = sm[2];
                d.y = sm[3];

                computeCurvature(geometry);
            }

            void computeCurvature(const SmoothGeometry geometry)
            {
                const T velocity = v.norm();
                switch(geometry)
                {
                    case SmoothPlanar:
                        curvature = (v.x*d.y-v.y*d.x)/(velocity*velocity*velocity);
                        break;

                    case SmoothAxisymmetric:
                        curvature = (v.x*d.y-v.y*d.x)/(velocity*velocity*velocity);
                        break;
                }
            }

            Smooth<T>   sm;         //!< 1D smoothing
            const T     zero;       //!< constant
            Vtx         r;          //!< position
            Vtx         v;          //!< velocity
            Vtx         d;          //!< second derivative
            T           curvature;  //!< computed curvature
            Tableau     coord;      //!< temporary 1D coordinates
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

#define real_t float
#include "smooth-arc.hxx"
#undef real_t

#define real_t double
#include "smooth-arc.hxx"
#undef real_t

#define real_t long double
#include "smooth-arc.hxx"
#undef real_t

#define real_t XReal<float>
#include "smooth-arc.hxx"
#undef real_t

#define real_t XReal<double>
#include "smooth-arc.hxx"
#undef real_t

#define real_t XReal<long double>
#include "smooth-arc.hxx"
#undef real_t


#define real_t apq
#include "smooth-arc.hxx"
#undef real_t

    }

}


