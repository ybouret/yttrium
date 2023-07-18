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
#include "y/system/exception.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"
#include "y/memory/out-of-reach.hpp"

#include <iostream>
#include <iomanip>

namespace Yttrium
{
    namespace Apex
    {

        namespace Nexus
        {
            //__________________________________________________________________
            //
            //
            //
            //! Base class for operations prototype
            //
            //
            //__________________________________________________________________
            class Proto : public Object
            {
            public:
                static const char CallSign[]; //!< "Apex::Natural"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                explicit Proto() noexcept; //!< setup

            public:
                virtual ~Proto() noexcept; //!< cleanup


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! number of bytes to hold numBits
                //______________________________________________________________
                static size_t BitsToBytes(const size_t numBits) noexcept;

                //______________________________________________________________
                //
                //! Debug check
                //______________________________________________________________
                static bool   CheckMetrics(const char  *where,
                                           const size_t nbits,
                                           const size_t bytes,
                                           const size_t words,
                                          const size_t WordSize,
                                           const size_t blockWords);


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
            static const unsigned                        CoreSize  = sizeof(CORE_TYPE);             //!< alias
            static const unsigned                        WordSize  = sizeof(WORD_TYPE);             //!< alias
            static const unsigned                        WordLog2  = iLog2<WordSize>::Value ;       //!< alias
            static const unsigned                        WordBits  = WordSize << 3;                 //!< alias
            static const unsigned                        CoreBits  = CoreSize << 3;                 //!< alias
            typedef typename UnsignedInt<CoreSize>::Type CoreType;                                  //!< alias
            typedef typename UnsignedInt<WordSize>::Type WordType;                                  //!< alias
            typedef Block<WordType>                      DataType;                                  //!< alias
            typedef Split64Into<WordType>                Splitter;                                  //!< alias
            typedef typename SignedInt<CoreSize>::Type   CIntType;                                  //!< alias
            static  const WordType                       WordMaxi = UnsignedInt<WordSize>::Maximum; //!< alias
            static  const CIntType                       Radix    = CIntType(WordMaxi) + 1;         //!< alias

            //__________________________________________________________________
            //
            //
            //! smart pointer for operations
            //
            //__________________________________________________________________
            class Pointer
            {
            public:
                inline  Pointer(Proto *p) noexcept : handle(p) { }
                inline ~Pointer() noexcept
                {
                    if(handle) { delete handle; handle=0; }
                }

                inline Proto &       operator*()        noexcept { assert(0!=handle); return *handle; }
                inline const Proto & operator*()  const noexcept { assert(0!=handle); return *handle; }
                inline Proto *       operator->()       noexcept { assert(0!=handle); return  handle; }
                inline const Proto * operator->() const noexcept { assert(0!=handle); return  handle; }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Pointer);
                Proto *handle;
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! debugging helper
            inline bool Check(const char *fn) const
            {
                assert(0!=fn);
                if( !CheckMetrics(fn,nbits,bytes,words,WordSize,block.words) )
                    return false;

                if( ! Memory::OutOfReach::Are0(block.entry+words,(block.words-words)*WordSize ) )
                {
                    std::cerr << fn << " dirty trailing bytes!" << std::endl;
                    return false;
                }

                return true;
            }

            //__________________________________________________________________
            //
            //! create a Proto with a given minimal BYTES capacity
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            //! create a Proto with a given uint64_t
            //__________________________________________________________________
            inline explicit Proto(const uint64_t qword) :
            Nexus::Proto(),
            nbits( BitCount::For(qword)          ),
            bytes( BitsToBytes(nbits)            ),
            words( Splitter::BytesToWords(bytes) ),
            block( sizeof(qword)                 )
            {
                assert(block.words>=Splitter::MaxWords);
                Splitter::DoSplit(block.entry,words,qword);
                assert(Check("Proto(uint64_t)"));
            }

            //__________________________________________________________________
            //
            //! copy constructor
            //__________________________________________________________________
            inline Proto(const Proto &proto) :
            Nexus::Proto(),
            nbits( proto.nbits ),
            bytes( proto.bytes ),
            words( proto.words ),
            block( proto.block )
            {
                memcpy(block.entry,proto.block.entry,words*WordSize);
                assert(Check("Proto(copy)"));
            }

            //__________________________________________________________________
            //
            //! create a random Proto with exactly userBits
            //__________________________________________________________________
            inline Proto(const size_t userBits, Random::Bits &ran) :
            Nexus::Proto(),
            nbits( userBits ),
            bytes( BitsToBytes(nbits) ),
            words( Splitter::BytesToWords(bytes) ),
            block( bytes )
            {
                if(words>0)
                {
                    const  size_t msi = words-1;
                    size_t        num = nbits;

                    for(size_t i=0;i<msi;++i)
                    {
                        block.entry[i] = ran.to<WordType>();
                        assert(num>WordBits);
                        num -= WordBits;
                    }
                    assert(num>0);
                    block.entry[msi] = ran.to<WordType>(unsigned(num));
                }
                update();
                assert(nbits==userBits);
                assert(Check("Proto(rand)"));
            }

            //__________________________________________________________________
            //
            //! create a Proto from W[N], then update
            //__________________________________________________________________
            inline Proto(const WordType *W,
                         const size_t    N) :
            Nexus::Proto(),
            nbits(0),
            bytes(N*WordSize),
            words(N),
            block(bytes)
            {
                assert(0!=W);
                memcpy(block.entry,W,bytes);
                update();
                assert(Check("Proto(W,N)"));
            }


            inline virtual ~Proto() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check if lhs and rhs are different
            static inline bool AreDifferent(const Proto &lhs, const Proto &rhs) noexcept
            {
                const size_t lwords = lhs.words;
                switch(SignOf(lwords,rhs.words))
                {
                    case Negative:
                    case Positive: return true;
                    case __Zero__:
                        break;
                }
                return 0 != memcmp(lhs.block.entry,rhs.block.entry,lwords*WordSize);
            }

            //! check if lhs and rhs are equal
            static inline bool AreEqual(const Proto &lhs, const Proto &rhs) noexcept
            {
                const size_t lwords = lhs.words;
                switch(SignOf(lwords,rhs.words))
                {
                    case Negative:
                    case Positive:
                        return false;
                    case __Zero__:
                        break;
                }
                return 0 == memcmp(lhs.block.entry,rhs.block.entry,lwords*WordSize);
            }

            //__________________________________________________________________
            //
            //! update metrics from CURRENT number of words
            //__________________________________________________________________
            inline void update() noexcept
            {
                while(words>0)
                {
                    const size_t    msi = words-1;
                    const WordType &msw = block.entry[msi];
                    if(msw<=0) { Coerce(words) = msi; continue; }
                    assert(words>0);
                    Coerce(nbits) = BitCount::For(msw) + msi * WordBits;
                    Coerce(bytes) = BitsToBytes(nbits);
                    return;
                }
                assert(0==words);
                Coerce(bytes) = 0;
                Coerce(nbits) = 0;
            }

            //__________________________________________________________________
            //
            //! Least Significant 64 bits
            //__________________________________________________________________
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
            //! clean extraneous words
            //__________________________________________________________________
            inline void ztrim() noexcept
            {
                memset(block.entry+words,0,(block.words-words)*WordSize);
            }

            //! get byte[0..bytes-1]
            uint8_t getByte(const size_t i) const noexcept
            {
                assert(i<bytes);
                WordType w = block.entry[i/WordSize];
                size_t   r = i%WordSize;
                while(r-- > 0) w = UnsignedInt<WordSize>::SHR8(w);
                return uint8_t(w);
            }

            //! Hexadecimal printing
            void printHex(std::ostream &os) const
            {
                if(bytes<=0) { os << '0'; return; }

                bool   first = true;
                size_t i     = bytes;
                while(i>0)
                {
                    const uint8_t b = getByte(--i);
                    const uint8_t l = b>>4;
                    if(first)
                    {
                        assert(b>0);
                        if(l>0) os << Hexadecimal::Upper[l];
                        first = false;
                    }
                    else
                    {
                        os << Hexadecimal::Upper[l];
                    }
                    os << Hexadecimal::Upper[b&0xf];
                }
            }

            //__________________________________________________________________
            //
            //! try to steal proto
            //__________________________________________________________________
            inline bool couldSteal(const Proto &other) noexcept
            {
                if(other.words<=block.words)
                {
                    Coerce(bytes) = other.bytes;
                    Coerce(words) = other.words;
                    Coerce(nbits) = other.nbits;
                    memcpy(block.entry,other.block.entry,words*WordSize);
                    ztrim();
                    assert(Check("couldSteal(other)"));
                    return true;
                }
                else
                    return false;
            }

            //__________________________________________________________________
            //
            //! load 64 bits
            //__________________________________________________________________
            inline void ld64(const uint64_t qword) noexcept
            {
                Coerce(nbits) = BitCount::For(qword);
                Coerce(bytes) = BitsToBytes(nbits);
                Coerce(words) = Splitter::BytesToWords(bytes);
                Splitter::DoSplit(block.entry,words,qword);
                ztrim();
                assert(Check("ld64(qword)"));
            }


            //__________________________________________________________________
            //
            //! Addition
            /**
             \param lhs left words
             \param lnw left num words
             \param rhs right words
             \param rnw right num words
             \param ell ellapsed ticks in computation
             */
            //__________________________________________________________________
            static inline
            Proto * Add(const WordType * const lhs,
                        const size_t           lnw,
                        const WordType * const rhs,
                        const size_t           rnw,
                        uint64_t *             ell = 0)
            {
                assert(0!=lhs);
                assert(0!=rhs);
                if(lnw<=0)
                {
                    if(rnw<=0)
                        //------------------------------------------------------
                        // 0 + 0
                        //------------------------------------------------------
                        return new Proto(0,AsCapacity);
                    else
                        //------------------------------------------------------
                        // 0 + rhs = rhs
                        //------------------------------------------------------
                        return new Proto(rhs,rnw);
                }
                else
                {
                    assert(lnw>0);
                    if(rnw<=0)
                    {
                        //------------------------------------------------------
                        // lhs + 0 = lhs
                        //------------------------------------------------------
                        return new Proto(lhs,lnw);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // order a >= b
                        //------------------------------------------------------
                        const WordType *a  = lhs; assert(0!=a);
                        const size_t    na = lnw; assert(na>0);
                        const WordType *b  = rhs; assert(0!=b);
                        const size_t    nb = rnw; assert(nb>0);
                        if(na<nb) { Swap(a,b); CoerceSwap(na,nb); }
                        assert(na>=nb);

                        //------------------------------------------------------
                        // acquire resources
                        //------------------------------------------------------
                        const size_t    ns = na+1;
                        Proto          *S  = new Proto(ns << WordLog2,AsCapacity); assert(S->block.words>=ns);
                        WordType       *s  = S->block.entry;
                        const uint64_t  t  = ell ? WallTime::Ticks() : 0;
                        {
                            //--------------------------------------------------
                            // initialize algorithm
                            //--------------------------------------------------
                            CoreType sum = CoreType(a[0]) + CoreType(b[0]);
                            s[0]         = static_cast<WordType>(sum);

                            //--------------------------------------------------
                            // common word(s)
                            //--------------------------------------------------
                            for(size_t i=1;i<nb;++i)
                            {
                                sum >>= WordBits;
                                sum += CoreType(a[i]) + CoreType(b[i]);
                                s[i] = static_cast<WordType>(sum);
                            }

                            //--------------------------------------------------
                            // extra word(s)
                            //--------------------------------------------------
                            for(size_t i=nb;i<na;++i)
                            {
                                sum >>= WordBits;
                                sum += CoreType(a[i]);
                                s[i] = static_cast<WordType>(sum);
                            }

                            //--------------------------------------------------
                            // final word
                            //--------------------------------------------------
                            sum >>= WordBits;
                            s[na] = static_cast<WordType>(sum);
                        }

                        //------------------------------------------------------
                        // update resources
                        //------------------------------------------------------
                        Coerce(S->words) = ns;
                        S->update();
                        if(ell) (*ell) += WallTime::Ticks() - t;
                        return S;
                    }

                }
            }

            //__________________________________________________________________
            //
            //! Addition for two Proto
            //__________________________________________________________________
            static inline
            Proto * Add(const Proto &lhs,
                        const Proto &rhs,
                        uint64_t    *ell=0)
            {
                return Add(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words,ell);
            }

            //__________________________________________________________________
            //
            //! Addition for Proto + uint64_t
            //__________________________________________________________________
            static inline
            Proto * Add(const Proto    &lhs,
                        const uint64_t &rhs )
            {
                const Splitter alias(rhs);
                return Add(lhs.block.entry,lhs.words,alias.w,alias.n);
            }

            //__________________________________________________________________
            //
            //! Add one
            //__________________________________________________________________
            static inline
            Proto *Add1(const Proto &lhs)
            {
                static const WordType One(1);
                return Add(lhs.block.entry,lhs.words,&One,1);
            }


            //__________________________________________________________________
            //
            //! Subtraction
            /**
             \param lhs left words
             \param lnw left num words
             \param rhs right words
             \param rnw right num words
             */
            //__________________________________________________________________
            static inline
            Proto * Sub(const WordType * const lhs,
                        const size_t           lnw,
                        const WordType * const rhs,
                        const size_t           rnw)
            {
                assert(0!=lhs);
                assert(0!=rhs);
                if(lnw<=0)
                {
                    if(rnw<=0)
                    {
                        //------------------------------------------------------
                        // 0 - 0
                        //------------------------------------------------------
                        return new Proto(0,AsCapacity);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // 0 - (>0)
                        //------------------------------------------------------
                        throw Specific::Exception(CallSign,"subtract positive from 0");
                    }
                }
                else
                {
                    assert(lnw>0);
                    if(rnw<=0)
                    {
                        //------------------------------------------------------
                        // lhs  - 0
                        //------------------------------------------------------
                        return new Proto(lhs,lnw);
                    }
                    else
                    {
                        assert(rnw>0);
                        //------------------------------------------------------
                        // lhs  - rhs, generic case
                        //------------------------------------------------------
                        if(rnw>lnw) throw Specific::Exception(CallSign,"rhs>lhs level-1");

                        Proto    *D = new Proto(lnw*WordSize,AsCapacity);
                        WordType *d = D->block.entry;
                        assert(lnw>=rnw);

                        {
                            CIntType carry = 0;
                            for(size_t i=0;i<rnw;++i)
                            {
                                const CIntType del = CIntType(lhs[i]) - CIntType(rhs[i]) - carry;
                                if(del<0)
                                {
                                    d[i]  = WordType(del+Radix);
                                    carry = 1;
                                }
                                else
                                {
                                    d[i]  = WordType(del);
                                    carry = 0;
                                }
                            }

                            for(size_t i=rnw;i<lnw;++i)
                            {
                                CIntType del = CIntType(lhs[i]) - carry;
                                if(del<0)
                                {
                                    d[i]  = WordType(del+Radix);
                                    carry = 1;
                                }
                                else
                                {
                                    d[i]  = WordType(del);
                                    carry = 0;
                                }
                            }

                            if(0!=carry)
                            {
                                delete D;
                                throw Specific::Exception(CallSign,"rhs>lhs level-2");
                            }
                        }

                        Coerce(D->words) = lnw;
                        D->update();


                        return D;
                    }
                }

            }

            //__________________________________________________________________
            //
            //! Subtraction for Proto - Proto
            //__________________________________________________________________
            static inline
            Proto * Sub(const Proto &lhs,
                        const Proto &rhs)
            {
                return Sub(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words);
            }

            //__________________________________________________________________
            //
            //! Subtraction for Proto - uint64_t
            //__________________________________________________________________
            static inline
            Proto * Sub(const Proto    &lhs,
                        const uint64_t &rhs)
            {
                const Splitter alias(rhs);
                return Sub(lhs.block.entry,lhs.words,alias.w,alias.n);
            }

            //__________________________________________________________________
            //
            //! Subtraction for uint64_t - proto
            //__________________________________________________________________
            static inline
            Proto * Sub(const uint64_t lhs,
                        const Proto   &rhs)
            {
                const Splitter alias(lhs);
                return Sub(alias.w,alias.n,rhs.block.entry,rhs.words);
            }

            //__________________________________________________________________
            //
            //! Subtract one
            //__________________________________________________________________
            static inline
            Proto *Sub1(const Proto &lhs)
            {
                static const WordType One(1);
                return Sub(lhs.block.entry,lhs.words,&One,1);
            }

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! default comparison
            //__________________________________________________________________
            static inline
            SignType Compare(const WordType * const lhs, const size_t lnw,
                             const WordType * const rhs, const size_t rnw) noexcept
            {
                assert(0!=lhs);
                assert(0!=rhs);
                if(lnw<rnw)
                {
                    assert(rhs[rnw-1]>0);
                    return Negative;
                }
                else
                {
                    if(rnw<lnw)
                    {
                        assert(lhs[lnw-1]>0);
                        return Positive;
                    }
                    else
                    {
                        assert(lnw==rnw);
                        for(size_t i=lnw;i>0;)
                        {
                            const WordType L = lhs[--i];
                            const WordType R = rhs[i];
                            switch(SignOf(L,R))
                            {
                                case Negative: return Negative;
                                case Positive: return Positive;
                                case __Zero__: continue;;
                            }
                        }
                        return __Zero__;
                    }
                }
            }

            //__________________________________________________________________
            //
            //! compare lhs,rhs
            //__________________________________________________________________
            static inline
            SignType Compare(const Proto &lhs,
                             const Proto &rhs) noexcept
            {
                return Compare(lhs.block.entry,lhs.words,rhs.block.entry,rhs.words);
            }

            //__________________________________________________________________
            //
            //! compare lhs, uint
            //__________________________________________________________________
            static inline
            SignType Compare(const Proto    &lhs,
                             const uint64_t &rhs)
            {
                const Splitter alias(rhs);
                return Compare(lhs.block.entry,lhs.words,alias.w,alias.n);
            }

            //__________________________________________________________________
            //
            //! compare uint, rhs
            //__________________________________________________________________
            static inline
            SignType Compare(const uint64_t lhs,
                             const Proto   &rhs)
            {
                const Splitter alias(lhs);
                return Compare(alias.w,alias.n,rhs.block.entry,rhs.words);
            }

        private:
            Y_DISABLE_ASSIGN(Proto);
        };

    }

}

#endif

