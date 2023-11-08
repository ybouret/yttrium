
//! \file

#ifndef Y_Concurrent_Tiles_Included
#define Y_Concurrent_Tiles_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/v2d.hpp"
#include "y/concurrent/split.hpp"
#include "y/type/utils.hpp"
#include "y/type/div.hpp"
#include "y/container/readable.hpp"

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
            typedef Memory::Wad<Segment<T>,Memory::Pooled> Strip; //!< alias
            typedef V2D<T>                                 VTX;   //!< alias

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

            //! display
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

        //______________________________________________________________________
        //
        //
        //
        //! Strip of segments
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Tile : public Segment<T>::Strip, public Readable< const Segment<T> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Memory::Wad<Tile<T>,Memory::Pooled> Paving;   //!< alias
            typedef typename Segment<T>::Strip          Segments; //!< alias
            typedef V2D<T>                              VTX;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //! allocate blank segments
            inline explicit Tile(const size_t maxSegments) :
            Segments(maxSegments),
            count(0),
            rwSeg( static_cast<Segment<T> *>(this->workspace) ),
            roSeg( rwSeg-1 ),
            items(0)
            {}

            //! cleanup
            inline virtual ~Tile() noexcept {
                while(count>0) Destruct(&rwSeg[--Coerce(count)]);
                Coerce(items) = 0;
                Coerce(rwSeg) = 0;
                Coerce(roSeg) = 0;
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! add a new segment to the tile
            inline void add(const VTX start, const T width) noexcept
            {
                assert(width>0);
                assert(count<this->capacity);
                Segment<T> *seg = &rwSeg[Coerce(count)++];
                new (seg) Segment<T>(start,width);
                //std::cerr << *seg << std::endl;
                Coerce(items) += seg->w;
            }

            inline virtual size_t size() const noexcept
            {
                return count;
            }

            inline virtual const Segment<T> & operator[](const size_t iseg) const noexcept
            {
                assert(iseg>0);
                assert(iseg<=count);
                return roSeg[iseg];
            }

            inline virtual const char * callSign() const noexcept
            {
                return "Tile";
            }



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tile);
            const size_t             count;   //!< number of segments
            Segment<T>       * const rwSeg;   //!< r/w segments
            const Segment<T> * const roSeg;   //!< rwSeg-1

        public:
            const size_t items; //!< number of target items

        };


        template <typename T>
        class TilesLayout : public Readable< const Tile<T> >
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
            count(0),
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
                Coerce(count) = Min<U>(nproc,items);
                std::cerr << "nproc=" << nproc << " => count=" << count << " for #items=" << items << std::endl;
            }


            virtual size_t size()           const noexcept { return count;   }
            virtual const char * callSign() const noexcept { return "Tiles"; }



            const size_t count;    //!< less or equal to user's CPU count
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
        class Tiles : 
        public TilesLayout<T>,
        public Tile<T>::Paving
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
            Tile<T>::Paving(this->count),
            rwTile( static_cast<Tile<T> *>(this->workspace) ),
            roTile( rwTile - 1 )
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
                    for(size_t rank=0;rank<this->count;++rank)
                    {
                        //------------------------------------------------------
                        //
                        // compute items for this rank
                        //
                        //------------------------------------------------------
                        T offset = 0;
                        T length = this->items;
                        Split::With(this->count, rank, length, offset); assert(length>0);
                        //std::cerr << "\trank=" << rank << " : @" << offset << " +" << length << std::endl;

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
                        Tile<T> &t = * ( new( &rwTile[built] ) Tile<T>(n_seg) );
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
                        std::cerr << "tile" << this->count << "." << rank << " = " << t << " #" << t.items << std::endl;
                    }
                    assert(this->count==built);
                }
                catch(...)
                {
                    erase(built);
                    throw;
                }
            }


            inline virtual ~Tiles() noexcept
            {
                erase( Coerce(this->count) );
            }


            inline virtual const Tile<T> & operator[](const size_t itile) const noexcept
            {
                assert(itile>=1);
                assert(itile<=this->count);
                return roTile[itile];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            Tile<T> *       const rwTile;
            const Tile<T> * const roTile;

            inline void erase(size_t &built) noexcept
            {
                while(built>0) Destruct( &rwTile[--built] );
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
