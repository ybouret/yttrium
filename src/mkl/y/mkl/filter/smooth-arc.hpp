
//! \file

#ifndef Y_MKL_Filter_Smooth_Arc_Included
#define Y_MKL_Filter_Smooth_Arc_Included 1


#include "y/mkl/v2d.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{
    namespace MKL
    {

        enum SmoothGeometry
        {
            SmoothPlanar,
            SmoothAxisymmetric
        };

        template <typename T>
        class SmoothArc
        {
        public:
            typedef V2D<T> Vtx;

            explicit  SmoothArc();
            virtual  ~SmoothArc() noexcept;

            void operator()(const T            &t0,
                            const Readable<T>   &t,
                            const Readable<T>   &x,
                            const Readable<T>   &y,
                            const size_t         xdegree,
                            const size_t         ydegree,
                            const SmoothGeometry geometry);

            void operator()(const T             &t0,
                            const Readable<T>   &t,
                            const Readable<Vtx> &p,
                            const size_t         xdegree,
                            const size_t         ydegree,
                            const SmoothGeometry geometry);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SmoothArc);
            class Code;
            Code *code;
        public:
            const Vtx &r;
            const Vtx &v;
            const T   &curvature;
        };

    }

}

#endif

