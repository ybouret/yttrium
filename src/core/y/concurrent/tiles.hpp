
//! \file

#ifndef Y_Concurrent_Tiles_Included
#define Y_Concurrent_Tiles_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/v2d.hpp"
#include "y/concurrent/split.hpp"
#include "y/type/utils.hpp"
#include "y/type/div.hpp"
#include "y/container/cxx/series.hpp"
#include "y/type/proxy.hpp"
#include "y/object.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Horizontal Segment, basic brick of a Tile
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
            typedef CxxSeries< const Segment<T> > Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline  Segment(const V2D<T> p, const T w) noexcept : start(p),       width(w),       x_end(start.x+width-1) { assert(width>0); } //!< setup
            inline  Segment(const Segment &s)          noexcept : start(s.start), width(s.width), x_end(s.x_end) {}                           //!< copy
            inline ~Segment()                          noexcept {}                                                                            //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Segment &s)
            {
                const V2D<T> q(s.x_end,s.start.y);
                os << "|#" << s.width << ":" << s.start << "->" << q <<"|";
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const V2D<T> start; //!< start coordinate
            const T      width; //!< width = items
            const T      x_end; //!< ending x position

        private:
            Y_DISABLE_ASSIGN(Segment);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Series of Segments
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Tile : public Object, public Proxy< typename Segment<T>::Series >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename Segment<T>::Series Segments; //!< alias
            typedef Proxy<Segments>             TileBase; //!< alias
            typedef CxxListOf<Tile>             List;     //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with capacity and rank
            inline explicit Tile(const size_t maxSegments, const size_t r) :
            segments(maxSegments),
            next(0),
            prev(0),
            items(0),
            rank(r),
            indx(rank+1)
            {}

            inline explicit Tile(const Tile &tile) :
            segments(tile.segments),
            next(0),
            prev(0),
            items(tile.items),
            rank(tile.rank),
            indx(tile.indx)
            {
            }


            //! cleanup
            inline virtual ~Tile() noexcept {}



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            inline Tile * clone() const
            {
                return new Tile(*this);
            }

            //! add a new segment
            inline void add(const V2D<T> p, const T w) {
                const Segment<T> _(p,w);
                segments.pushTail(_);
                Coerce(items) += _.width;
            }

        private:
            Y_DISABLE_ASSIGN(Tile);
            Segments     segments;
            typename TileBase::ConstInterface & surrogate() const noexcept { return segments; }

        public:
            Tile *       next;  //!< for list
            Tile *       prev;  //!< for list
            const T      items; //!< number of items
            const size_t rank;  //!< rank  in tiles
            const size_t indx;  //!< index in tiles

            //__________________________________________________________________
            //
            //
            //! Compound iterator over a Tile
            //
            //__________________________________________________________________
            class Iterator
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! initialize from a first segment and Tile's count
                inline Iterator(const Segment<T> * const first,
                                const size_t             count) :
                curr(first), last(curr+count), pos(0,0), num(0)
                { 
                    if(0!=first) { pos = first->start; num = first->width; }
                }

                //! cleanup
                inline ~Iterator() noexcept {}

                //! copy
                inline  Iterator(const Iterator &it) noexcept :
                curr(it.curr),
                last(it.last),
                pos(it.pos),
                num(it.num)
                {
                }

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! access current position
                inline V2D<T> operator*() const noexcept { return pos; }

                //! prefix increment operator.
                inline Iterator & operator++() noexcept { move(); return *this; }

                //!   postfix increment operator.
                inline Iterator operator++(int) noexcept { const Iterator temp = *this; move(); return temp; }

                //! testing difference
                inline friend bool operator != (const Iterator &lhs, const Iterator &rhs) noexcept
                {
                    if(lhs.curr!=rhs.curr)
                        return true;
                    else
                        return lhs.pos != rhs.pos;
                }

                

            private:
                Y_DISABLE_ASSIGN(Iterator);
                const Segment<T> * curr; //!< current segment
                const Segment<T> * last; //!< first invalid segment
                V2D<T>             pos;  //!< computed positions
                T                  num;  //!< remaining valid positions

                //! forwarding position
                inline void move() noexcept
                {
                    assert(curr<last);
                    assert(num>0);
                    if(--num<=0)
                    {
                        if(++curr>=last)
                        {
                            curr  = last  = 0;
                            pos.x = pos.y = 0;
                        }
                        else
                        {
                            pos = curr->start;
                            num = curr->width;
                        }
                    }
                    else
                    {
                        ++pos.x;
                    }
                }
            };

            //! beginning of Tile
            inline Iterator begin() const noexcept {
                assert(segments.size()>0);
                return Iterator( &segments[1], segments.size() );

            }

            //! dummy invalid iterator
            inline Iterator end()   const noexcept { return Iterator(0,0); }



        };

        //______________________________________________________________________
        //
        //
        //
        //! List of Tiles
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Tiles : public Proxy< const typename Tile<T>::List >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename Tile<T>::List Tiling; //!< alias
            typedef Proxy<const Tiling>    TProxy; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup for maximum nproc>0
            template <typename U>
            inline explicit Tiles(const U  nproc,
                                  V2D<T>   lower,
                                  V2D<T>   upper) :
            TProxy(),
            tiling()
            {
                assert(nproc>0);
                //--------------------------------------------------------------
                //
                //
                // setup area
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
                const T width = area.x;
                const T items = area.x * area.y;
                const U count = Min<U>(nproc,items);


                //--------------------------------------------------------------
                //
                //
                // create tiles
                //
                //
                //--------------------------------------------------------------
                for(U rank=0;rank<count;++rank)
                {
                    //----------------------------------------------------------
                    //
                    // compute items for this rank
                    //
                    //----------------------------------------------------------
                    T offset = 0;
                    T length = items;
                    Split::With(count, rank, length, offset); assert(length>0);

                    //----------------------------------------------------------
                    //
                    // deduce coordinates and number of segments
                    //
                    //----------------------------------------------------------
                    const T       finish = length + offset - 1;
                    const V2D<T>  v_ini  = idx2vtx(offset,width) + lower; // starting coordinates
                    const V2D<T>  v_end  = idx2vtx(finish,width) + lower; // final coordinates
                    const size_t  n_seg  = v_end.y-v_ini.y+1;             // corresponding number of segments

                    //----------------------------------------------------------
                    //
                    // build new Tile
                    //
                    //----------------------------------------------------------
                    Tile<T> &t = * tiling.pushTail( new  Tile<T>(n_seg,rank) );
                    if(n_seg<=1)
                    {
                        assert(1==n_seg);
                        assert(v_ini.y==v_end.y);
                        //------------------------------------------------------
                        // single segment
                        //------------------------------------------------------
                        t.add(v_ini,1+v_end.x-v_ini.x);
                        assert(t.items==length);
                    }
                    else
                    {
                        assert(n_seg>1);
                        //------------------------------------------------------
                        // first segment, partial width
                        //------------------------------------------------------
                        t.add(v_ini,1+upper.x-v_ini.x);

                        //------------------------------------------------------
                        // bulk segment(s), full width
                        //------------------------------------------------------
                        for(T y=v_ini.y+1;y<v_end.y;++y)
                            t.add( V2D<T>(lower.x,y),width);

                        //------------------------------------------------------
                        // last segment, partial width
                        //------------------------------------------------------
                        t.add( V2D<T>(lower.x,v_end.y), 1+v_end.x - lower.x);
                        assert(t.items==length);
                    }
                }
            }


            //! cleanup
            inline virtual ~Tiles() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Tiles &tiles)
            {
                os << '{' << std::endl;
                const size_t sz = tiles->size;
                for(const Tile<T> *t=tiles->head;t;t=t->next)
                {
                    os << "  " << sz << "." << t->rank << ":" << *t << " : #" << t->items << std::endl;
                }
                os << '}';
                return os;
            }

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            Tiling tiling;

            //! proxy behavior
            inline virtual
            typename TProxy::ConstInterface & surrogate() const noexcept { return tiling; }

            //! index to vertex using integer division
            static inline
            V2D<T> idx2vtx(const T p, const T w) noexcept
            {
                static const   Div<T> api;
                const typename Div<T>::Type dv = api.call(p,w);
                return V2D<T>(dv.rem,dv.quot);
            }

        };



    }

}


#endif
