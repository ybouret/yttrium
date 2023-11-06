
//! \file

#ifndef Y_Concurrent_Tiles_Included
#define Y_Concurrent_Tiles_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/v2d.hpp"
#include "y/concurrent/split.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        //! segment
        template <typename T>
        class Segment
        {
        public:
            typedef Memory::Wad<Segment<T>,Memory::Pooled> WadType;

            inline Segment(const T X, const T W) noexcept :
            lower(X), width(W), upper(lower+width-1) { assert(width>0); }

            inline Segment(const Segment &other) noexcept :
            lower(other.lower),
            width(other.width),
            upper(other.upper)
            {}


            inline ~Segment() noexcept {}

            const T lower;
            const T width;
            const T upper;

        private:
            Y_DISABLE_ASSIGN(Segment);
        };


        template <typename T>
        class Tiles
        {
        public:
            template <typename U>
            inline explicit Tiles(const U size,
                                 V2D<T>  lower,
                                 V2D<T>  upper)
            {
                // setup layout
                if(upper.x<lower.x) Swap(upper.x,lower.x);
                if(upper.y<lower.y) Swap(upper.y,lower.y);
                const V2D<T> width(1+upper.x-lower.x,1+upper.y-lower.y);

                // compute items and matching count of tiles
                const T      items = width.x * width.y;
                const size_t count = Min<U>(size,items);
                std::cerr << "size=" << size << " => count=" << count << " for #items=" << items << std::endl;
                for(size_t rank=0;rank<count;++rank)
                {
                    T offset = 0;
                    T length = items;
                    Split::With(count, rank, length, offset); assert(length>0);
                    std::cerr << "\trank=" << rank << " : @" << offset << " +" << length << std::endl;
                    const T finish = length + offset - 1;
                    for(T p=offset;p<=finish;++p)
                    {
                        
                    }

                }


            }

            inline virtual ~Tiles() noexcept {}

        private:
            Y_DISABLE_ASSIGN(Tiles);
        };


    }

}


#endif
