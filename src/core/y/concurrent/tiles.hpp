
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
            typedef Memory::Wad<Tile<T>,Memory::Pooled> Paving;   //!< allias
            typedef typename Segment<T>::Strip          Segments; //!< alias
            typedef V2D<T>                              VTX;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Tile(const size_t maxSegments) :
            Segments(maxSegments),
            size(0),
            segment( static_cast<Segment<T> *>(this->workspace) ),
            items(0)
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


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
            Segment<T> *segment;

        public:
            const size_t items;

        };


        template <typename T>
        class TilesLayout
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef V2D<T>  VTX; //!< alias

            inline virtual ~TilesLayout() noexcept {}

            template <typename U>
            inline explicit TilesLayout(const U  nproc,
                                        VTX     &lower,
                                        VTX     &upper) :
            size(0),
            items(0),
            width(0)
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
                const V2D<T> area(1+upper.x-lower.x,1+upper.y-lower.y);

                //--------------------------------------------------------------
                //
                //
                // compute items and matching count of tiles (less than 'size'!)
                //
                //
                //--------------------------------------------------------------
                Coerce(width) = area.x;
                Coerce(items) = area.x * area.y;
                Coerce(size)  = Min<U>(nproc,items);
                std::cerr << "nproc=" << nproc << " => size=" << size << " for #items=" << items << std::endl;
            }

            const size_t size;    //!< less or equal to user's CPU count
            const T      items;   //!< total number of point to dispatch
            const T      width;   //!< bulk width

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TilesLayout);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Collection of Tiles
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Tiles : public TilesLayout<T>, public Tile<T>::Paving
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
            inline explicit Tiles(const U nproc,
                                  VTX     lower,
                                  VTX     upper) :
            TilesLayout<T>(nproc,lower,upper),
            Tile<T>::Paving(this->size),
            tile( static_cast<Tile<T> *>(this->workspace) )
            {

                size_t   built = 0;
                try{

                    //--------------------------------------------------------------
                    //
                    //
                    // build count tiles
                    //
                    //
                    //--------------------------------------------------------------
                    for(size_t rank=0;rank<this->size;++rank)
                    {
                        //------------------------------------------------------
                        //
                        // compute items for this rank
                        //
                        //------------------------------------------------------
                        T offset = 0;
                        T length = this->items;
                        Split::With(this->size, rank, length, offset); assert(length>0);
                        std::cerr << "\trank=" << rank << " : @" << offset << " +" << length << std::endl;

                        //------------------------------------------------------
                        //
                        // deduce coordinates and number of segments
                        //
                        //------------------------------------------------------
                        const T       finish = length + offset - 1;
                        const VTX     v_ini  = idx2vtx(offset,this->width) + lower; // starting reduced coordinates
                        const VTX     v_end  = idx2vtx(finish,this->width) + lower; // final coordinates
                        const size_t  n_seg  = v_end.y-v_ini.y+1;               // corresponding segments

                        //------------------------------------------------------
                        //
                        // build tile
                        //
                        //------------------------------------------------------
                        //Tile<T> t(n_seg);
                        Tile<T> &t = * ( new( &tile[built] ) Tile<T>(n_seg) );
                        ++built;
                        assert(t.capacity>=n_seg);
                        if(n_seg<=1)
                        {
                            assert(1==n_seg);
                            assert(v_ini.y==v_end.y);
                            //--------------------------------------------------
                            // single segment
                            //--------------------------------------------------
                            t.add(v_ini,1+v_end.x-v_ini.x);
                            assert(t.items==size_t(length));
                        }
                        else
                        {
                            assert(n_seg>1);
                            //--------------------------------------------------
                            // first segment
                            //--------------------------------------------------
                            t.add(v_ini,1+upper.x-v_ini.x);

                            //--------------------------------------------------
                            // bulk segment(s)
                            //--------------------------------------------------
                            for(T y=v_ini.y+1;y<v_end.y;++y)
                                t.add( VTX(lower.x,y), this->width);

                            //--------------------------------------------------
                            // last segment
                            //--------------------------------------------------
                            t.add( VTX(lower.x,v_end.y), 1+v_end.x - lower.x);
                            assert(t.items==size_t(length));
                        }
                    }
                    assert(this->size==built);
                }
                catch(...)
                {
                    erase(built);
                    throw;
                }


            }

            inline virtual ~Tiles() noexcept 
            {
                erase( Coerce(this->size) );
            }

        private:
            Y_DISABLE_ASSIGN(Tiles);
            Tile<T> *tile;

            inline void erase(size_t &built) noexcept
            {
                while(built>0) Destruct( &tile[--built] );
            }

            static inline VTX idx2vtx(const T p, const T w) noexcept
            {
                static const   Div<T> api;
                const typename Div<T>::Type dv = api.call(p,w);
                return VTX(dv.rem,dv.quot);
            }


        };


    }

}


#endif
