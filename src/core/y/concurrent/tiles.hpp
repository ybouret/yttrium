
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Memory::Wad<Segment<T>,Memory::Pooled> Strip;
            typedef V2D<T>                                 VTX; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup at position + width
            inline Segment(const VTX start, const T width) noexcept :
            p(start), w(width), q(p.x+width-1,p.y) { assert(width>0); }

            //! copy
            inline Segment(const Segment &other) noexcept :
            p(other.p),
            w(other.w),
            q(other.q)
            {}


            //! cleanup
            inline ~Segment() noexcept {}

            //! diplay
            inline friend std::ostream & operator<<(std::ostream &os, const Segment &s)
            {
                os << "|" << s.p << "->" << s.q << "|=" << s.w;
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const VTX p; //!< starting position
            const T   w; //!< width
            const VTX q; //!< quit position
        private:
            Y_DISABLE_ASSIGN(Segment);

        };

        template <typename T>
        class Tile : public Segment<T>::Strip
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename Segment<T>::Strip Segments; //!< alias
            typedef V2D<T>                     VTX;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Tile(const size_t maxSegments) :
            Segments(maxSegments),
            size(0),
            items(0),
            segment( static_cast<Segment<T> *>(this->workspace) )
            {}

            inline virtual ~Tile() noexcept 
            {
                while(size>0) Destruct(&segment[--Coerce(size)]);
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline void add(const VTX start, const T width) noexcept
            {
                assert(width>0);
                assert(size<this->capacity);
                Segment<T> *seg = &segment[Coerce(size)++];
                new (seg) Segment<T>(start,width);
                std::cerr << *seg << std::endl;
                Coerce(items) += seg->w;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t size;
            const size_t items;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
            Segment<T> *segment;
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
                //
                // setup layout
                //
                //
                //--------------------------------------------------------------
                if(upper.x<lower.x) Swap(upper.x,lower.x);
                if(upper.y<lower.y) Swap(upper.y,lower.y);
                const V2D<T> width(1+upper.x-lower.x,1+upper.y-lower.y);

                //--------------------------------------------------------------
                //
                //
                // compute items and matching count of tiles (less than 'size'!)
                //
                //
                //--------------------------------------------------------------
                const T      items = width.x * width.y;
                const size_t count = Min<U>(size,items);
                std::cerr << "size=" << size << " => count=" << count << " for #items=" << items << std::endl;
              

                //--------------------------------------------------------------
                //
                //
                // build count tiles
                //
                //
                //--------------------------------------------------------------
                for(size_t rank=0;rank<count;++rank)
                {
                    //----------------------------------------------------------
                    //
                    // compute items for this rank
                    //
                    //----------------------------------------------------------
                    T offset = 0;
                    T length = items;
                    Split::With(count, rank, length, offset); assert(length>0);
                    std::cerr << "\trank=" << rank << " : @" << offset << " +" << length << std::endl;

                    //----------------------------------------------------------
                    //
                    // deduce coordinates and number of segments
                    //
                    //----------------------------------------------------------
                    const T       finish = length + offset - 1;
                    const VTX     v_ini  = idx2vtx(offset,width.x) + lower; // starting reduced coordinates
                    const VTX     v_end  = idx2vtx(finish,width.x) + lower; // final coordinates
                    const size_t  n_seg  = v_end.y-v_ini.y+1;               // corresponding segments

                    //----------------------------------------------------------
                    //
                    // build tile
                    //
                    //----------------------------------------------------------
                    Tile<T> tile(n_seg); assert(tile.capacity>=n_seg);
                    if(n_seg<=1)
                    {
                        assert(1==n_seg);
                        assert(v_ini.y==v_end.y);
                        //------------------------------------------------------
                        // single segment
                        //------------------------------------------------------
                        tile.add(v_ini,1+v_end.x-v_ini.x);
                        assert(tile.items==size_t(length));
                    }
                    else
                    {
                        assert(n_seg>1);
                        //------------------------------------------------------
                        // first segment
                        //------------------------------------------------------
                        tile.add(v_ini,1+upper.x-v_ini.x);

                        //------------------------------------------------------
                        // bulk segment(s)
                        //------------------------------------------------------
                        for(T y=v_ini.y+1;y<v_end.y;++y)
                            tile.add( VTX(lower.x,y), width.x);

                        //------------------------------------------------------
                        // last segment
                        //------------------------------------------------------
                        tile.add( VTX(lower.x,v_end.y), 1+v_end.x - lower.x);

                        assert(tile.items==size_t(length));
                    }

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
