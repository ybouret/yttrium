
//! \file

#ifndef Y_Concurrent_Tiling_Included
#define Y_Concurrent_Tiling_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/v2d.hpp"
#include "y/concurrent/split.hpp"
#include "y/type/utils.hpp"
#include "y/type/proxy.hpp"
#include "y/object.hpp"
#include "y/data/list/cxx.hpp"
#include "y/type/ints.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! 2D Tiling
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct Tiling
        {
            typedef T                                     Type;   //!< alias
            typedef typename UnsignedInt<sizeof(T)>::Type Size;   //!< alias
            typedef V2D<Type>                             Vertex; //!< alias
            typedef V2D<Size>                             Area;   //!< alia

            //__________________________________________________________________
            //
            //
            //
            //! linear segment to build a tile
            //
            //
            //__________________________________________________________________
            class Segment
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from position and length
                inline  Segment(const Vertex p, const Size w) noexcept :
                start(p),
                width(w),
                x_end(start.x+width-1)
                { assert(width>0); }

                //! no-throw copy
                inline  Segment(const Segment &s) noexcept :
                start(s.start),
                width(s.width),
                x_end(s.x_end)
                {}

                //! cleanup
                inline ~Segment() noexcept {}


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const Segment &s)
                {
                    const Vertex q(s.x_end,s.start.y);
                    os << "|#" << s.width << ":" << s.start << "->" << q <<"|";
                    return os;
                }


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Vertex start; //!< start coordinate
                const Size   width; //!< width = items
                const Type   x_end; //!< ending x position

            private:
                Y_DISABLE_ASSIGN(Segment);
            };

            //! linear memory for segments, to form a tile
            typedef Memory::Wad<Segment,Memory::Pooled> Segments;

            //__________________________________________________________________
            //
            //
            //
            //! Tile of contiguous segments
            //
            //
            //__________________________________________________________________
            class Tile : public Object, public Segments
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Tile> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with memory for segments
                explicit Tile(const size_t n) :
                Object(),
                Segments(n),
                size(0),
                items(0),
                base( this->lead() ),
                next(0),
                prev(0)
                {}

                //! cleanup
                virtual ~Tile() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                inline Tile * clone() const { return new Tile(*this); }

                //! add a new segment
                inline void add(const Vertex p, const Size w)
                {
                    assert(size<this->capacity);
                    Coerce(items) +=  (new ( &base[Coerce(size)++] ) Segment(p,w))->width;
                }

                //! return self for list display
                inline const Tile & operator*() const noexcept { return *this; }

                //! display as an array of segments
                inline friend std::ostream & operator<<(std::ostream &os, const Tile &tile)
                {
                    os << '{' << std::endl;
                    for(size_t i=0;i<tile.size;++i)
                    {
                        os << ' ' << tile.base[i] << std::endl;
                    }
                    return os << '}';
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const size_t size;   //!< number of segments
                const size_t items;  //!< total items of original region

            private:
                Y_DISABLE_ASSIGN(Tile);
                Segment * const base;

                inline Tile(const Tile &tile) :
                Object(),
                Segments(tile.size),
                size(tile.size),
                items(tile.items),
                base( this->lead() ),
                next(0),
                prev(0)
                {
                    Memory::OutOfReach::Copy(base, tile.base, size*sizeof(Segment) );
                }

            public:
                Tile *next; //!< for list
                Tile *prev; //!< for list

                //______________________________________________________________
                //
                //
                //! Compound Iterator over a Tile
                //
                //______________________________________________________________
                class Iterator
                {
                public:
                    //______________________________________________________________
                    //
                    // C++
                    //______________________________________________________________

                    //! initialize from a first segment and Tile's count
                    inline Iterator(const Segment * const first,
                                    const size_t          count) :
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
                    inline Vertex operator*() const noexcept { return pos; }

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
                    const Segment * curr; //!< current segment
                    const Segment * last; //!< first invalid segment
                    Vertex          pos;  //!< computed positions
                    Size            num;  //!< remaining valid positions

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
                    assert(size>0);
                    assert(0!=base);
                    return Iterator(base,size);
                }

                //! dummy invalid iterator
                inline Iterator end()   const noexcept { return Iterator(0,0); }


            };

            typedef typename Tile::Iterator Iterator; //!< syntax alias


            //__________________________________________________________________
            //
            //
            //
            //! List of contiguous, balanced tiles over an initial region
            //
            //
            //__________________________________________________________________
            class Tiles : public Proxy<const typename Tile::List>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Proxy<const typename Tile::List> TProxy; //!< alias

                //! setup for maximum nproc>0
                inline explicit Tiles(const size_t nproc,
                                      Vertex       lower,
                                      Vertex       upper) :
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
                    const Area area(1+upper.x-lower.x,1+upper.y-lower.y);

                    //--------------------------------------------------------------
                    //
                    //
                    // compute items and matching count of tiles (less than 'size'!)
                    //
                    //
                    //--------------------------------------------------------------
                    const Size   width = area.x;
                    const Size   items = area.x * area.y;
                    const size_t count = Min<size_t>(nproc,items);


                    //--------------------------------------------------------------
                    //
                    //
                    // create tiles
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
                        Size offset = 0;
                        Size length = items;
                        Split::With(count, rank, length, offset); assert(length>0);

                        //----------------------------------------------------------
                        //
                        // deduce coordinates and number of segments
                        //
                        //----------------------------------------------------------
                        const T       finish = length + offset - 1;
                        const Vertex  v_ini  = idx2vtx(offset,width) + lower; // starting coordinates
                        const Vertex  v_end  = idx2vtx(finish,width) + lower; // final coordinates
                        const size_t  n_seg  = v_end.y-v_ini.y+1;             // corresponding number of segments

                        //----------------------------------------------------------
                        //
                        // build new Tile
                        //
                        //----------------------------------------------------------
                        Tile &t = * tiling.pushTail( new  Tile(n_seg) );
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
                                t.add( Vertex(lower.x,y),width);

                            //------------------------------------------------------
                            // last segment, partial width
                            //------------------------------------------------------
                            t.add( Vertex(lower.x,v_end.y), 1+v_end.x - lower.x);
                            assert(t.items==length);
                        }
                    }
                }

                //! cleanup
                virtual ~Tiles() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tiles);
                typename Tile::List tiling;
                
                inline virtual
                typename TProxy::ConstInterface & surrogate() const noexcept
                { return tiling; }

                //! index to vertex using integer division
                static inline
                Vertex idx2vtx(const Size p, const Size w) noexcept
                {
                    assert(w>0);
                    return Vertex(p%w,p/w);
                    //static const   Div<T> api;
                    //const typename Div<T>::Type dv = api.call(p,w);
                    //return V2D<T>(dv.rem,dv.quot);
                }
            };



        };

    }

}

#endif

