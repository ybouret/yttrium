
//! \file

#ifndef Y_MKL_Filter_Smooth_Arc_Included
#define Y_MKL_Filter_Smooth_Arc_Included 1


#include "y/mkl/v2d.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //! kind of arc
        enum SmoothGeometry
        {
            SmoothPlanar,       //!< regular 2D planar arc
            SmoothAxisymmetric  //!< arc describing axisymmetric contour
        };

        //______________________________________________________________________
        //
        //
        //! Smoothing an arc
        //
        //______________________________________________________________________
        template <typename T>
        class SmoothArc
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef V2D<T> Vtx; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit  SmoothArc();          //!< setup
            virtual  ~SmoothArc() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! extract local arc metrics
            void operator()(const T            &t0,
                            const Readable<T>   &t,
                            const Readable<T>   &x,
                            const Readable<T>   &y,
                            const size_t         xdegree,
                            const size_t         ydegree,
                            const SmoothGeometry geometry);

            //! extract local arc metrics
            void operator()(const T             &t0,
                            const Readable<T>   &t,
                            const Readable<Vtx> &p,
                            const size_t         xdegree,
                            const size_t         ydegree,
                            const SmoothGeometry geometry);

            //__________________________________________________________________
            //
            //! reserve internal memory to process from 0 to degree
            //__________________________________________________________________
            void reserveMaxDegree(const size_t degree);


            //__________________________________________________________________
            //
            //! reserve internal memory to process points
            //__________________________________________________________________
            void reserveMaxLength(const size_t points);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SmoothArc);
            class Code;
            Code *code;
        public:
            const Vtx &r;           //!< last position
            const Vtx &v;           //!< velocity
            const T   &curvature;   //!< last curvature
        };

    }

}

#endif

