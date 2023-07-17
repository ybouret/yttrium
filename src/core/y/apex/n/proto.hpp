//! \file

#ifndef Y_Apex_Natural_Proto_Included
#define Y_Apex_Natural_Proto_Included 1

#include "y/apex/n/split64.hpp"
#include "y/object.hpp"
#include "y/apex/types.hpp"
#include "y/apex/m/block.hpp"
#include "y/check/static.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/type/capacity.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/system/wtime.hpp"
#include "y/random/bits.hpp"
#include <iomanip>

namespace Yttrium
{
    namespace Apex
    {

        namespace Nexus
        {
            class Proto : public Object
            {
            protected:
                explicit Proto() noexcept;

            public:
                virtual ~Proto() noexcept;

                static size_t BitsToBytes(const size_t numBits) noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Proto);
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Algorithm prototypes
        //
        //
        //______________________________________________________________________
        template <typename CORE_TYPE, typename WORD_TYPE>
        class Proto : public Nexus::Proto
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned                        CoreSize  = sizeof(CORE_TYPE);       //!< alias
            static const unsigned                        WordSize  = sizeof(WORD_TYPE);       //!< alias
            static const unsigned                        WordLog2  = iLog2<WordSize>::Value ; //!< alias
            static const unsigned                        WordBits  = WordSize << 3;           //!< alias
            static const unsigned                        CoreBits  = CoreSize << 3;           //!< alias
            typedef typename UnsignedInt<CoreSize>::Type CoreType;                            //!< alias
            typedef typename UnsignedInt<WordSize>::Type WordType;                            //!< alias
            typedef Block<WordType>                      DataType;                            //!< alias
            typedef Split64Into<WordType>                Splitter;                            //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create a Proto with a given minimal BYTES capacity
            inline explicit Proto(const size_t      numBytes,
                                  const AsCapacity_ &)  :
            Nexus::Proto(),
            nbits(0),
            bytes(0),
            words(0),
            block(numBytes)
            {
                Y_STATIC_CHECK(WordSize<CoreSize,InvalidMetrics);
            }

            inline explicit Proto(const uint64_t qword) :
            Nexus::Proto(),
            nbits( BitCount::For(qword)          ),
            bytes( BitsToBytes(nbits)            ),
            words( Splitter::BytesToWords(bytes) ),
            block( sizeof(qword)                 )
            {
                assert(block.words>=Splitter::MaxWords);
                Splitter::DoSplit(block.entry,words,qword);
            }

            inline Proto(const Proto &proto) :
            Nexus::Proto(),
            nbits( proto.nbits ),
            bytes( proto.bytes ),
            words( proto.words ),
            block( proto.block )
            {
            }

            inline Proto(const size_t userBits, Random::Bits &ran) :
            Nexus::Proto(),
            nbits( userBits ),
            bytes( BitsToBytes(nbits) ),
            words( Splitter::BytesToWords(bytes) ),
            block( bytes )
            {
                size_t    remaining = nbits;
                WordType *target    = block.entry;
                while(remaining>=WordBits)
                {
                    *(target++) = ran.to<WordType>();
                    remaining  -= WordBits;
                }
                if(remaining>0)
                {
                    assert(remaining<WordBits);
                    *target = ran.to<WordType>( static_cast<unsigned>(remaining) );
                }
                
            }

            inline Proto(const WordType *W, const size_t N) :
            Nexus::Proto(),
            nbits(0),
            bytes(N*sizeof(words)),
            words(N),
            block(bytes)
            {
                assert(W);
                memcpy(block.entry,W,bytes);
                update();
            }


            inline virtual ~Proto() noexcept {}

            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! update metrics from a precomputed number of words
            //__________________________________________________________________
            inline void update() noexcept
            {
                while(words>0)
                {
                    const size_t    msi = words-1;
                    const WordType &msw = block.entry[msi];
                    if(msw<=0) { Coerce(words) = msi; continue; }
                    Coerce(nbits) = BitCount::For(msw) + msi * WordBits;
                    Coerce(bytes) = RequiredBytesFor(nbits);
                    return;
                }
                assert(0==words);
                Coerce(bytes) = 0;
                Coerce(nbits) = 0;
            }


            inline uint64_t ls64() const noexcept
            {
                uint64_t qw = 0;
                for(size_t i=words;i>0;)
                    (qw <<= WordBits) |= block.entry[--i];
                return qw;
            }

            //__________________________________________________________________
            //
            //! display status
            //__________________________________________________________________
            inline void display() const
            {
                std::cerr << "<APEX " << CoreBits << "-" << WordBits;
                std::cerr << " bytes="  << std::setw(3) << bytes;
                std::cerr << ", words=" << std::setw(3) << words;
                std::cerr << ", nbits=" << std::setw(5) << nbits ;
                std::cerr << ">";
                std::cerr << " [";
                for(size_t i=0;i<words;++i) std::cerr << ' ' << Hexadecimal(block.entry[i]);
                std::cerr << " ]" << std::endl;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t nbits; //!< exact number of bits
            const size_t bytes; //!< exact number of bytes
            const size_t words; //!< aligned to bytes
            DataType     block; //!< resources

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________



            //__________________________________________________________________
            //
            //! Addition
            /**
             \param lhs left words
             \param lnw left num words
             \param rhs right words
             \param rns right num words
             */
            //__________________________________________________________________
            static inline
            Proto * Add(const WordType * const lhs, const size_t lnw,
                        const WordType * const rhs, const size_t rnw,
                        uint64_t &ell)
            {
                assert(0!=lhs);
                assert(0!=rhs);
                if(lnw<=0)
                {
                    if(rnw<=0)
                    {
                        return new Proto(0,AsCapacity);
                    }
                    else
                    {
                        return new Proto(rhs,rnw);
                    }
                }
                else
                {
                    assert(lnw>0);
                    if(rnw<=0)
                    {
                        return new Proto(lhs,lnw);
                    }
                    else
                    {
                        // order a >= b
                        const WordType *a  = lhs; assert(0!=a);
                        const size_t    na = lnw; assert(na>0);
                        const WordType *b  = rhs; assert(0!=b);
                        const size_t    nb = rnw; assert(nb>0);
                        if(na<nb) { Swap(a,b); CoerceSwap(na,nb); }
                        assert(na>=nb);

                        // acquire resources
                        const size_t    ns = na+1;
                        Proto          *S  = new Proto(ns << WordLog2,AsCapacity); assert(S->block.words>=ns);
                        WordType       *s  = S->block.entry;

                        const uint64_t  mark = WallTime::Ticks();
                        {
                            // initialize algorithm
                            CoreType sum = CoreType(a[0]) + CoreType(b[0]);
                            s[0]         = static_cast<WordType>(sum);

                            // common size
                            for(size_t i=1;i<nb;++i)
                            {
                                sum >>= WordBits;
                                sum += CoreType(a[i]) + CoreType(b[i]);
                                s[i] = static_cast<WordType>(sum);
                            }

                            // extra size
                            for(size_t i=nb;i<na;++i)
                            {
                                sum >>= WordBits;
                                sum += CoreType(a[i]);
                                s[i] = static_cast<WordType>(sum);
                            }

                            // final
                            sum >>= WordBits;
                            s[na] = static_cast<WordType>(sum);
                        }

                        Coerce(S->words) = ns;
                        S->update();
                        ell += WallTime::Ticks() - mark;
                        return S;
                    }

                }
            }

            static inline
            Proto * Add(const Proto &lhs,
                        const Proto &rhs,
                        uint64_t    &ell)
            {
                return Add(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words,ell);
            }
            
        private:
            Y_DISABLE_ASSIGN(Proto);
        };

    }

}

#endif

