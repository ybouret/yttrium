
//! \file

#ifndef Y_Concurrent_Tiles_Included
#define Y_Concurrent_Tiles_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/v2d.hpp"
#include "y/concurrent/split.hpp"
#include "y/type/utils.hpp"
#include "y/type/div.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Horizontal Segment
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Segment
        {
        public:
            typedef Memory::Wad<Segment<T>,Memory::Pooled> Strip;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup at position + width
            inline Segment(const T X, const T W) noexcept :
            lower(X), width(W), upper(lower+width-1) { assert(width>0); }

            //! copy
            inline Segment(const Segment &other) noexcept :
            lower(other.lower),
            width(other.width),
            upper(other.upper)
            {}


            //! cleanup
            inline ~Segment() noexcept {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const T lower; //!< lower=starting position
            const T width; //!< number of values
            const T upper; //!< upper=final position

        private:
            Y_DISABLE_ASSIGN(Segment);
        };

        template <typename T>
        class Tile : public Segment<T>::Strip
        {
        public:
            typedef typename Segment<T>::Strip Segments;

            inline explicit Tile(const size_t maxSegments) : Segments(maxSegments) {}
            inline virtual ~Tile() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Collection to Tiles
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Tiles
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef V2D<T>  VTX; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create tiles for AT MOST 'size' processors, are lower to upper
            template <typename U>
            inline explicit Tiles(const U size,
                                  VTX     lower,
                                  VTX     upper)
            {
                //--------------------------------------------------------------
                //
                // setup layout
                //
                //--------------------------------------------------------------
                if(upper.x<lower.x) Swap(upper.x,lower.x);
                if(upper.y<lower.y) Swap(upper.y,lower.y);
                const V2D<T> width(1+upper.x-lower.x,1+upper.y-lower.y);

                //--------------------------------------------------------------
                //
                // compute items and matching count of tiles (less than 'size'!)
                //
                //--------------------------------------------------------------
                const T      items = width.x * width.y;
                const size_t count = Min<U>(size,items);
                std::cerr << "size=" << size << " => count=" << count << " for #items=" << items << std::endl;
                for(size_t rank=0;rank<count;++rank)
                {
                    T offset = 0;
                    T length = items;
                    Split::With(count, rank, length, offset); assert(length>0);
                    std::cerr << "\trank=" << rank << " : @" << offset << " +" << length << std::endl;
                    const T   finish = length + offset - 1;
                    const VTX v_ini  = idx2vtx(offset,width.x) + lower; // starting reduced coordinates
                    const VTX v_end  = idx2vtx(finish,width.x) + lower; // final coordinates
                    const T   n_seg  = v_end.y-v_ini.y+1;
                    std::cerr << "#seg=" << n_seg << std::endl;
                    Tile<T> tile(n_seg);


                }


            }

            inline virtual ~Tiles() noexcept {}

        private:
            Y_DISABLE_ASSIGN(Tiles);
            static inline VTX idx2vtx(const T p, const T w) noexcept
            {
                static const Div<T> divAPI;
                const typename Div<T>::Type dv = divAPI.call(p,w);
                return VTX(dv.rem,dv.quot);
            }


        };


    }

}


#endif
