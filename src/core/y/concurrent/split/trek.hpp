//! \file

#ifndef Y_Concurrent_Split_Trek_Included
#define Y_Concurrent_Split_Trek_Included 1

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
        //! Trek family to optimize sweep
        //
        //
        //______________________________________________________________________
        enum TrekFamily
        {
            TrekIncrease, //!< step=1, may use prefix ++
            TrekDecrease, //!< step=-1, may use prefix --
            TrekStandard  //!< |step| > 1
        };

        //______________________________________________________________________
        //
        //
        //
        //! Trek of precomputed indices
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Trek
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
            inline Trek(const Type start,
                        const Size count,
                        const Type delta,
                        const Type stop) noexcept:
            offset(start),
            length(count),
            update(delta),
            latest(stop),
            family(Qualify(update))
            {
                assert(0!=update);
            }

            //! default setup empty
            inline Trek() noexcept :
            offset(0),
            length(0),
            update(1),
            latest(0),
            family(TrekIncrease)
            {

            }

            //! copy
            inline Trek(const Trek &trek) noexcept:
            offset(trek.offset),
            length(trek.length),
            update(trek.update),
            latest(trek.latest),
            family(trek.family)
            {
            }

            //! cleanup
            inline virtual ~Trek() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const Trek &trek)
            {
                os << '#';
                os << '[';
                os << trek.offset;
                os << ':';
                os << trek.latest;
                os << ':';
                os << trek.update;
                os << ']';
                os << '=';
                os << trek.length;
                return os;
            }

            //! no-throw exchange
            inline void xch(Trek &trek) noexcept
            {
                CoerceSwap(offset,trek.offset);
                CoerceSwap(length,trek.length);
                CoerceSwap(update,trek.update);
                CoerceSwap(latest,trek.latest);
                CoerceSwap(family,trek.family);
            }

            //__________________________________________________________________
            //
            //! call proc(index) for each valid index
            //__________________________________________________________________
            template <typename PROC> inline
            void sweep(PROC &proc) const
            {
                switch(family)
                {
                    case TrekIncrease: { assert(1==update);
                        Size j = length; if(j<=0) return;
                        Type i = offset;
                    CYCLE_INC:
                        proc(i);
                        if(--j<=0) return;
                        ++i;
                        goto CYCLE_INC;
                    } break;

                    case TrekDecrease: { assert( Type(-1)==update);
                        Size j = length; if(j<=0) return;
                        Type i = offset;
                    CYCLE_DEC:
                        proc(i);
                        if(--j<=0) return;
                        --i;
                        goto CYCLE_DEC;
                    } break;

                    case TrekStandard: {
                        Size j = length;
                        if(j<=0) return;
                        Type       i = offset;
                        const Type d = update;
                    CYCLE_STD:
                        proc(i);
                        if(--j<=0) return;
                        i += d;
                        goto CYCLE_STD;
                    } break;

                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Type       offset; //!< initial value
            const Size       length; //!< number of indices
            const Type       update; //!< step
            const Type       latest; //!< final value
            const TrekFamily family; //!< for specific code

        private:
            Y_DISABLE_ASSIGN(Trek);

            inline TrekFamily Qualify(const T &step) noexcept {
                static const Int2Type<IsSigned<T>::Value> choice = {};
                return Qualify(choice,step);
            }

            inline TrekFamily Qualify(const Int2Type<false> &, const T &step) noexcept {
                if(1==step) return TrekIncrease;
                return TrekStandard;
            }

            inline TrekFamily Qualify(const Int2Type<true> &, const T &step) noexcept {
                if( 1 == step) return TrekIncrease;
                if(-1 == step) return TrekDecrease;
                return TrekStandard;
            }

        };

        //______________________________________________________________________
        //
        //
        //
        //! Split Global Loop into Local Loop(s)
        //
        //
        //______________________________________________________________________
        struct Split
        {

            //! for(T i=head;i<=tail;i += step) => Trek for size.rank
            template <typename T, typename U> static inline
            Trek<T> For(const U &size,
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
                    return Trek<T>(head,0,step,head);
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
                        return Trek<T>(head,0,step,head);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // computing Trek
                        //------------------------------------------------------
                        const T start = head  + offset * step;
                        const T stop_ = start + (length-1) * step;
                        return Trek<T>(start,length,step,stop_);
                    }
                }
            }


            //! for(T i=head;i<=tail;i += step) => Trek for context
            template <typename T> static inline
            Trek<T> For(const Context & cntx,
                        const T &       head,
                        const T &       tail,
                        const T &       step)
            {
                return For(cntx.size,cntx.rank,head,tail,step);
            }


            //! V count=length; for(T i=offset;count>0;++i,--count) => Trek for size.rank
            template <typename T,typename U,typename V> static inline
            Trek<T> Using(const U &size,
                          const U &rank,
                          const V &length,
                          const T &offset)
            {
                const T stride = 1;
                if(length<=0)
                {
                    return Trek<T>(offset, 0, stride, offset);
                }
                else
                {
                    const T ending = offset+(length-1);
                    return For(size,rank,offset,ending,stride);
                }
            }

            //! V count=length; for(T i=offset;count>0;++i,--count) => Trek for context
            template <typename T, typename V> static inline
            Trek<T> Using(const Context &cntx,
                          const V       &length,
                          const T       &offset)
            {
                return Using(cntx.size,cntx.rank,length,offset);
            }

            //! for(size_t i=1;i<=seq.size();++i) => Trek for size.rank
            template <typename U, typename SEQUENCE> static inline
            Trek<size_t> Part(const U &size, const U &rank, SEQUENCE &seq)
            {
                const size_t offset = 1;
                const size_t length = seq.size();
                return Using(size,rank,length,offset);
            }

            //! for(size_t i=1;i<=seq.size();++i) => Trek for context
            template <typename SEQUENCE> static inline
            Trek<size_t> Part(const Context &ctx, SEQUENCE &seq)
            {
                return Part(ctx.size,ctx.rank,seq);
            }
        };
    }

}

#endif
