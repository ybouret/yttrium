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
#include <iomanip>

namespace Yttrium
{
    namespace Apex
    {

        //______________________________________________________________________
        //
        //
        //
        //! Algorithm prototypes
        //
        //
        //______________________________________________________________________
        template <typename CORE_TYPE, typename WORD_TYPE>
        class Proto : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned                        CoreSize  = sizeof(CORE_TYPE); //!< alias
            static const unsigned                        WordSize  = sizeof(WORD_TYPE); //!< alias
            static const unsigned                        WordBits  = WordSize << 3;     //!< alias
            static const unsigned                        CoreBits  = CoreSize << 3;     //!< alias
            typedef typename UnsignedInt<CoreSize>::Type CoreType;                      //!< alias
            typedef typename UnsignedInt<WordSize>::Type WordType;                      //!< alias
            typedef Block<WordType>                      DataType;                      //!< alias
            typedef Split64Into<WordType>                Splitter;                      //!< alias

            static inline size_t GetBytesFor(const size_t numBits) noexcept
            {
                return Y_ALIGN_ON(8,numBits)/8;
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create a Proto with a given minimal capacity
            inline explicit Proto(const size_t       n,
                                  const AsCapacity_ &)  :
            Object(),
            bytes(0),
            words(0),
            nbits(0),
            block(n)
            {
                Y_STATIC_CHECK(WordSize<CoreSize,InvalidMetrics);
            }

            inline explicit Proto(const uint64_t qword) :
            Object(),
            bytes( BytesFor(qword)               ),
            words( Splitter::GetWordsFor(bytes)  ),
            nbits( BitCount::For(qword)          ),
            block( sizeof(qword)                 )
            {
                assert(block.words>=Splitter::MaxWords);
                Splitter::DoSplit(block.entry,words,qword);
            }

            inline Proto(const Proto &proto) :
            Object(),
            bytes( proto.bytes ),
            words( proto.words ),
            nbits( proto.nbits ),
            block( proto.block )
            {
            }

            inline Proto(const WordType *W, const size_t N) :
            Object(),
            bytes(N*sizeof(words)),
            words(N),
            nbits(0),
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
            //! update metrics
            //__________________________________________________________________
            inline void update() noexcept
            {
                while(words>0)
                {
                    const size_t    msi = words-1;
                    const WordType &msw = block.entry[msi];
                    if(msw<=0) { Coerce(words) = msi; continue; }
                    Coerce(nbits) = BitCount::For(msw) + msi * WordBits;
                    Coerce(bytes) = GetBytesFor(nbits);
                    return;
                }
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
            const size_t bytes; //!< exact number of bytes
            const size_t words; //!< aligned to bytes
            const size_t nbits; //!< exact number of bits
            DataType     block; //!< resources

            //__________________________________________________________________
            //
            //
            // API
            //
            //__________________________________________________________________

            static inline
            Proto * Add(const WordType *lhs, const size_t lnw,
                        const WordType *rhs, const size_t rnw)
            {
                assert(0!=lhs);
                assert(0!=rhs);
                if(lnw<=0)
                {
                    if(rnw<=0)
                    {

                    }
                    else
                    {

                    }
                }
                else
                {
                    assert(lnw>0);

                }
            }

        private:
            Y_DISABLE_ASSIGN(Proto);
        };

    }

}

#endif

