

#ifndef Y_Concurrent_Split_Divide_Included
#define Y_Concurrent_Split_Divide_Included 1

#include "y/concurrent/split/trek.hpp"
#include "y/concurrent/context.hpp"
#include "y/type/ints.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Trek of precomputed indices
        //
        //
        //______________________________________________________________________
        template <typename T>
        class TrekOf : public Trek
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef T                                     Type; //!< alias
            typedef typename UnsignedInt<sizeof(T)>::Type Size; //!< unsigned alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             \param start first index
             \param count number of indices: zero means empty
             \param delta step between each index
             \param stop  final reached index
             */
            inline TrekOf(const Type start,
                          const Size count,
                          const Type delta,
                          const Type stop) noexcept:
            Trek( GetKind(delta) ),
            offset(start),
            length(count),
            update(delta),
            latest(stop)
            {
                assert(0!=update);

            }


            //! copy
            inline TrekOf(const TrekOf &trek) noexcept:
            Trek(trek),
            offset(trek.offset),
            length(trek.length),
            update(trek.update),
            latest(trek.latest)
            {
            }

            //! cleanup
            inline ~TrekOf() noexcept {}

            //! display
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

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Type offset; //!< initial value
            const Size length; //!< number of indices
            const Type update; //!< step
            const Type latest; //!< final value

        private:
            Y_DISABLE_ASSIGN(TrekOf);

            inline Kind GetKind(const T &step) noexcept {
                static const Int2Type<IsSigned<T>::Value> choice = {};
                return GetKind(choice,step);
            }

            inline Kind GetKind(const Int2Type<false> &, const T &step) noexcept {
                if(1==step) return Increase;
                return Standard;
            }

            inline Kind GetKind(const Int2Type<true> &, const T &step) noexcept {
                if( 1 == step) return Increase;
                if(-1 == step) return Decrease;
                return Standard;
            }

        };

        //______________________________________________________________________
        //
        //
        //
        //! Dividing loops into treks
        //
        //
        //______________________________________________________________________
        struct Divide
        {

            //! for(T i=head;i<=tail;i += step) => Trek for size.rank
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


            //! for(T i=head;i<=tail;i += step) => Trek for context
            template <typename T> static inline
            TrekOf<T> For(const Context & cntx,
                          const T &       head,
                          const T &       tail,
                          const T &       step)
            {
                return Using(cntx.size,cntx.rank,head,tail,step);
            }


            //! V count=length; for(T i=offset;count>0;++i,--count) => Trek for size.rank
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

            //! V count=length; for(T i=offset;count>0;++i,--count) => Trek for context
            template <typename T, typename V> static inline
            TrekOf<T> Using(const Context &cntx,
                            const V       &length,
                            const T       &offset)
            {
                return Using(cntx.size,cntx.rank,length,offset);
            }

            //! for(size_t i=1;i<=seq.size();++i) => Trek for size.rank
            template <typename U, typename SEQUENCE> static inline
            TrekOf<size_t> Part(const U &size, const U &rank, SEQUENCE &seq)
            {
                const size_t offset = 1;
                const size_t length = seq.size();
                return Using(size,rank,length,offset);
            }

            //! for(size_t i=1;i<=seq.size();++i) => Trek for context
            template <typename SEQUENCE> static inline
            TrekOf<size_t> Part(const Context &ctx, SEQUENCE &seq)
            {
                return Part(ctx.size,ctx.rank,seq);
            }
        };
    }

}

#endif
