
//! \file

#ifndef Y_Concurrent_Tiling_Included
#define Y_Concurrent_Tiling_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/v2d.hpp"
#include "y/concurrent/split/for-loop.hpp"
#include "y/type/utils.hpp"
#include "y/type/proxy.hpp"
#include "y/object.hpp"
#include "y/data/list/cxx.hpp"
#include "y/type/ints.hpp"
#include "y/ptr/auto.hpp"
#include "y/check/static.hpp"

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
                scxx( base-1       ),
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

                //! clone this tile
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
                    const size_t n  = tile.size;
                    const size_t nm = n-1;
                    os << '{' << std::endl;
                    for(size_t i=0;i<n;++i)
                    {
                        os << ' ' << tile.base[i];
                        if(nm==i) os << " @total=" << tile.items;
                        os << std::endl;
                    }
                    return os << '}';
                }

                //! access segment in [1..size]
                inline const Segment & operator[](const size_t i) const noexcept
                {
                    assert(i>=1);
                    assert(i<=size);
                    return scxx[i];
                }

                //! convert Segment to binary compatible strip
                /**
                 STRIP[1..size]
                 */
                template <typename STRIP> inline
                const STRIP * as() const noexcept
                {
                    Y_STATIC_CHECK(sizeof(STRIP)==sizeof(Segment),ErrorStripSize);
                    return Memory::OutOfReach::Cast<const STRIP,const Segment>(scxx);
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
                Segment *       const base;
                const Segment * const scxx;

                inline Tile(const Tile &tile) :
                Object(),
                Segments(tile.size),
                size(tile.size),
                items(tile.items),
                base( this->lead() ),
                scxx( base-1       ),
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
            //! record 2D request
            //
            //__________________________________________________________________
            class Locus
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline explicit Locus(const Vertex &org, const Vertex &end) noexcept :
                lower(org),
                upper(end)
                {
                    if(lower.x>upper.x) CoerceSwap(lower.x,upper.x);
                    if(lower.y>upper.y) CoerceSwap(lower.y,upper.y);

                }

                //! copy
                inline  Locus(const Locus &_) noexcept : lower(_.lower), upper(_.upper) {}

                //! cleanup
                inline ~Locus() noexcept {}

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const Locus &my)
                {
                    os << '[' << my.lower << ':' << my.upper <<']';
                    return os;
                }

                //! equality
                inline friend bool operator==(const Locus &lhs, const Locus &rhs) noexcept
                {
                    return lhs.lower == rhs.lower && lhs.upper==rhs.upper;
                }

                //! difference
                inline friend bool operator!=(const Locus &lhs, const Locus &rhs) noexcept
                {
                    return lhs.lower != rhs.lower || lhs.upper!=rhs.upper;
                }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                const Vertex lower; //!< lower coordinate
                const Vertex upper; //!< upper coordinate

            private:
                Y_DISABLE_ASSIGN(Locus);
            };

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

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup for maximum nproc>0
                inline explicit Tiles(const size_t nproc,
                                      Vertex       lower,
                                      Vertex       upper) :
                TProxy(),
                tiling()
                {
                    assert(nproc>0);
                    //----------------------------------------------------------
                    //
                    //
                    // setup area
                    //
                    //
                    //----------------------------------------------------------
                    const Area area = MakeArea(lower,upper);

                    //----------------------------------------------------------
                    //
                    //
                    // compute items and matching count of tiles (less than 'size'!)
                    //
                    //
                    //----------------------------------------------------------
                    const Size   width = area.x;
                    const Size   items = area.x * area.y;
                    const size_t count = Min<size_t>(nproc,items);

                    //----------------------------------------------------------
                    //
                    //
                    // create tiles
                    //
                    //
                    //----------------------------------------------------------
                    for(size_t rank=0;rank<count;++rank)
                    {
                        //------------------------------------------------------
                        //
                        // compute items for this rank
                        //
                        //------------------------------------------------------
                        const Size start = 0;
                        //const Size length = items;
                        const ForLoop<Size> trek = Split::Using(count, rank, items, start);
                        assert(trek.length>0);
                        //Split::With(count, rank, length, offset); assert(length>0);

                        tiling.pushTail( MakeTile(lower, upper, width, trek.offset, trek.length) );
                    }
                }

                //! cleanup
                virtual ~Tiles() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! create tile matching context, can be NULL
                static inline Tile *For(const Context &ctx,
                                        Vertex         lower,
                                        Vertex         upper)
                {
                    //----------------------------------------------------------
                    //
                    // setup area
                    //
                    //----------------------------------------------------------
                    const Area area = MakeArea(lower,upper);

                    //----------------------------------------------------------
                    //
                    // compute items and matching count of tiles
                    //
                    //----------------------------------------------------------
                    const Size   width = area.x;
                    const Size   items = area.x * area.y;
                    if(items<ctx.size)
                        return 0;

                    //----------------------------------------------------------
                    //
                    // compute items for this rank
                    //
                    //----------------------------------------------------------
                    const Size start = 0;
                    const ForLoop<Size> trek = Split::Using(ctx, items, start);
                    assert(trek.length>0);

                    //----------------------------------------------------------
                    //
                    // return matching Tile
                    //
                    //----------------------------------------------------------
                    return MakeTile(lower, upper, width, trek.offset, trek.length);
                }

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
                }

                static inline Area MakeArea(Vertex &lower, Vertex &upper) noexcept
                {
                    if(upper.x<lower.x) Swap(upper.x,lower.x);
                    if(upper.y<lower.y) Swap(upper.y,lower.y);
                    return Area(1+upper.x-lower.x,1+upper.y-lower.y);
                }

                static inline Tile *MakeTile(const Vertex &lower,
                                             const Vertex &upper,
                                             const Size    width,
                                             const Size    offset,
                                             const Size    length)

                {
                    //------------------------------------------------------
                    //
                    // deduce coordinates and number of segments
                    //
                    //------------------------------------------------------
                    const T       finish = length + offset - 1;
                    const Vertex  v_ini  = idx2vtx(offset,width) + lower; // starting coordinates
                    const Vertex  v_end  = idx2vtx(finish,width) + lower; // final coordinates
                    const size_t  n_seg  = v_end.y-v_ini.y+1;             // corresponding number of segments

                    //------------------------------------------------------
                    //
                    // build new Tile
                    //
                    //------------------------------------------------------
                    AutoPtr<Tile> guard = new Tile(n_seg);
                    {
                        Tile &t = *guard;
                        if(n_seg<=1)
                        {
                            assert(1==n_seg);
                            assert(v_ini.y==v_end.y);
                            //--------------------------------------------------
                            // single segment
                            //--------------------------------------------------
                            t.add(v_ini,1+v_end.x-v_ini.x);
                            assert(t.items==length);
                        }
                        else
                        {
                            assert(n_seg>1);
                            //--------------------------------------------------
                            // first segment, partial width
                            //--------------------------------------------------
                            t.add(v_ini,1+upper.x-v_ini.x);

                            //--------------------------------------------------
                            // bulk segment(s), full width
                            //--------------------------------------------------
                            for(T y=v_ini.y+1;y<v_end.y;++y)
                                t.add( Vertex(lower.x,y),width);

                            //--------------------------------------------------
                            // last segment, partial width
                            //--------------------------------------------------
                            t.add( Vertex(lower.x,v_end.y), 1+v_end.x - lower.x);
                            assert(t.items==length);
                        }
                    }
                    return guard.yield();
                }

            };



        };

    }

}

#endif

