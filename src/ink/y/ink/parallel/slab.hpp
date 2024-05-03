//! \file

#ifndef Y_Ink_Slab_Included
#define Y_Ink_Slab_Included 1

#include "y/ink/area.hpp"
#include "y/concurrent/frame/2d.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! HSegment matching Tiling<unit_t>::Segment
        //
        //
        //______________________________________________________________________
        struct HSegment
        {
            unit_t x;  //!< x
            unit_t y;  //!< y
            unit_t w;  //!< width
            unit_t xt; //!< x top

            //! convert x,y to coord
            inline const Coord start() const noexcept { return Coord(x,y); }
        };

        //______________________________________________________________________
        //
        //
        //
        //! Slab of area to work with threads
        //
        //
        //______________________________________________________________________
        class Slab : public Concurrent::Frame2D<unit_t>
        {
        public:
            static const size_t Bytes    = 256;              //!< number of Bytes for information
            static const size_t WordSize = sizeof(uint64_t); //!< map on uint64_t[Bytes]
            static const size_t Request  = Bytes * WordSize; //!< memory request

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Slab(const ThreadContext &ctx) noexcept; //!< setup
            virtual ~Slab() noexcept;                         //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void  honorRequest(); //!< once called, post initializatio
            Slab &ldz() noexcept; //!< clear content

            //! return cast of internal workspace
            template <typename T> inline
            T *as(const size_t n)
            {
                checkRequest(n,sizeof(T));
                return static_cast<T*>(wksp);
            }

            //! return cast of internal workspace
            template <typename T> inline
            const T *as(const size_t n) const
            {
                checkRequest(n,sizeof(T));
                return static_cast<const T*>(wksp);
            }

            //! scan min/max over the slab
            template <typename PIXMAP> inline
            void scanMinMax(const PIXMAP &pxm)
            {
                const size_t nseg = count(); if(nseg<=0) return;
                typedef typename PIXMAP::MutableType T;
                T * const data = static_cast<T*>(wksp);
                T &tmin = data[0];
                T &tmax = data[1];
                tmin = tmax = pxm( hseg[1].start() );
                for(size_t j=nseg;j>0;--j)
                {
                    const HSegment                 &seg = hseg[j];
                    const typename PIXMAP::RowType &row = pxm(seg.y);
                    for(unit_t i=seg.w,x=seg.x;i>0;--i,++x)
                    {
                        const T &temp = row(x);
                        tmin = Min(tmin,temp);
                        tmax = Max(tmax,temp);
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const HSegment * const & hseg; //!< transmogrified segments [1..count()]

            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slab);
            void * const wksp;
            const size_t wlen;
            void checkRequest(const size_t n, const size_t bs) const;
        };

    }

}

#endif

