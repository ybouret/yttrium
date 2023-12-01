
#include "y/concurrent/split.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace Yttrium
{

    namespace Concurrent
    {


        class Trek
        {
        public:
            enum Kind
            {
                Increase,
                Decrease,
                Standard
            };


            const Kind kind;

            virtual ~Trek() noexcept {}
            explicit Trek(const Kind k) noexcept : kind(k) {}
            explicit Trek(const Trek &trek) noexcept : kind(trek.kind) {}


        private:
            Y_DISABLE_ASSIGN(Trek);
        };


        template <typename T>
        class TrekOf : public Trek
        {
        public:
            typedef T                                     Type;
            typedef typename UnsignedInt<sizeof(T)>::Type Size;

            inline TrekOf(const Type start,
                          const Size count,
                          const Type delta,
                          const Type stop_) noexcept :
            Trek( GetKind(delta) ),
            offset(start),
            length(count),
            update(delta),
            latest(stop_)
            {
                assert(0!=update);

            }

            inline TrekOf(const TrekOf &trek) noexcept :
            Trek(trek),
            offset(trek.offset),
            length(trek.length),
            update(trek.update),
            latest(trek.latest)
            {
            }

            inline ~TrekOf() noexcept {}

            inline friend std::ostream & operator<<(std::ostream &os, const TrekOf &trek)
            {
                os << '#';
                os << '[';
                os << trek.offset;
                os << ':';
                os << trek.update;
                os << ':';
                os << trek.latest;
                os << ']';
                os << '=';
                os << trek.length;
                return os;
            }

            const Type offset; //!< initial value
            const Size length; //!< number of stops
            const Type update; //!< step
            const Type latest; //!< final value

        private:
            Y_DISABLE_ASSIGN(TrekOf);

            inline Kind GetKind(const T &step) noexcept
            {
                static const Int2Type<IsSigned<T>::Value> choice = {};
                return GetKind(choice,step);
            }

            inline Kind GetKind(const Int2Type<false> &, const T &step) noexcept
            {
                if(1==step) return Increase;
                return Standard;
            }

            inline Kind GetKind(const Int2Type<true> &, const T &step) noexcept
            {
                if( 1 == step) return Increase;
                if(-1 == step) return Decrease;
                return Standard;
            }

        };

        struct Divide
        {

            template <typename T, typename U> static inline
            TrekOf<T> For(const U &size,
                          const U &rank,
                          const T &head,
                          const T &tail,
                          const T &step)
            {
                assert(size>0);
                assert(rank<size);
                assert(0!=step);
                //--------------------------------------------------------------
                //
                // compute all the stop points
                //
                //--------------------------------------------------------------
                const T stops = (step+tail-head)/step;
                if(stops<=0)
                {
                    //----------------------------------------------------------
                    //
                    // empty Trek, level-1
                    //
                    //----------------------------------------------------------
                    return TrekOf<T>(head,0,step,head);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // computing offset/length of stop points
                    //
                    //----------------------------------------------------------
                    T offset = 0;
                    T length = stops;
                    U left   = size;
                    T todo   = length/left;
                    for(U r=0;r<rank;++r)
                    {
                        --left;
                        length -= todo;
                        offset += todo;
                        todo    = length/left;
                    }
                    length = todo;

                    if(length<=0)
                    {
                        //------------------------------------------------------
                        // empty Trek, level-2
                        //------------------------------------------------------
                        return TrekOf<T>(head,0,step,head);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // computing Trek
                        //------------------------------------------------------
                        const T start = head  + offset * step;
                        const T stop_ = start + (length-1) * step;
                        return TrekOf<T>(start,length,step,stop_);
                    }
                }
            }

            template <typename T> static inline
            TrekOf<T> For(const Context & cntx,
                          const T &       head,
                          const T &       tail,
                          const T &       step)
            {
                return Using(cntx.size,cntx.rank,head,tail,step);
            }


            template <typename T,typename U,typename V> static inline
            TrekOf<T> Using(const U &size,
                            const U &rank,
                            const V &length,
                            const T &offset)
            {
                const T stride = 1;
                if(length<=0)
                {
                    return TrekOf<T>(offset, 0, stride, offset);
                }
                else
                {
                    const T ending = offset+(length-1);
                    return For(size,rank,offset,ending,stride);
                }
            }

            template <typename T, typename V> static inline
            TrekOf<T> Using(const Context &cntx,
                            const V       &length,
                            const T       &offset)
            {
                return Using(cntx.size,cntx.rank,length,offset);
            }

            template <typename U, typename SEQUENCE> static inline
            TrekOf<size_t> Part(const U &size, const U &rank, SEQUENCE &seq)
            {
                const size_t offset = 1;
                const size_t length = seq.size();
                return Using(size,rank,length,offset);
            }


            template <typename SEQUENCE> static inline
            TrekOf<size_t> Part(const Context &ctx, SEQUENCE &seq)
            {
                return Part(ctx.size,ctx.rank,seq);
            }
        };
    }

}

Y_UTEST(concurrent_split1d)
{

#if 0
    int Length = 100;
    int Offset = 0;

    if(argc>1) Length = ASCII::Convert::To<int>(argv[1],"Length");
    if(argc>2) Offset = ASCII::Convert::To<int>(argv[2],"Offset");


    for(size_t size=1;size<=10;++size)
    {
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            int length = Length;
            int offset = Offset;

            Concurrent::Split::With(size, rank, length, offset);
            std::cerr << "  " << size << "." << rank << " : @" << offset << " +" << length << std::endl;
        }
    }
#endif

    Y_SIZEOF(Concurrent::TrekOf<int8_t>);
    Y_SIZEOF(Concurrent::TrekOf<int16_t>);
    Y_SIZEOF(Concurrent::TrekOf<int32_t>);
    Y_SIZEOF(Concurrent::TrekOf<int64_t>);


    int head = 1;
    int tail = 3;
    int step = 1;
    if(argc>1) head = ASCII::Convert::To<int>(argv[1],"head");
    if(argc>2) tail = ASCII::Convert::To<int>(argv[2],"tail");
    if(argc>3) step = ASCII::Convert::To<int>(argv[3],"step");



    for(size_t size=1;size<=2;++size)
    {
        std::cerr << std::endl;
        std::cerr << "size=" << size << std::endl;
        for(size_t rank=0;rank<size;++rank)
        {
            Concurrent::TrekOf<int> trek = Concurrent::Divide::For(size,rank, head, tail, step);
            std::cerr << "->" << trek << std::endl;
        }
    }

}
Y_UDONE()

