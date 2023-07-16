//! \file

#ifndef Y_Apex_Natural_Proto_Included
#define Y_Apex_Natural_Proto_Included 1

#include "y/object.hpp"
#include "y/apex/number.hpp"
#include "y/apex/m/block.hpp"
#include "y/check/static.hpp"
#include "y/calculus/align.hpp"
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
        //! splitting unisgned 64bits into smaller words
        //
        //
        //______________________________________________________________________
        template <typename WordType>
        class Split64Into
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned WordSize  = sizeof(WordType);                       //!< alias
            static const unsigned WordBits  = WordSize << 3;                          //!< alias
            static const unsigned SelfSize  = sizeof(uint64_t);                       //!< alias
            static const unsigned MaxWords  = Y_ALIGN_ON(WordSize,SelfSize)/WordSize; //!< alias

            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! words to wrap a given number of bytes
            //__________________________________________________________________
            static inline size_t WordsFor(const size_t bytes) noexcept
            {
                return Y_ALIGN_ON(WordSize,bytes)/WordSize;
            }

            //__________________________________________________________________
            //
            //! words to wrap a given u64
            //__________________________________________________________________
            static inline size_t ToWords(const uint64_t X) noexcept { return WordsFor( BytesFor(X) ); }

            //__________________________________________________________________
            //
            //! split algorithm to given W[] with precomped N = ToWords(X)
            //__________________________________________________________________
            static inline void   DoSplit(WordType *W, const size_t N, uint64_t X) noexcept
            {
                assert(N==ToWords(X));
                assert(N<=MaxWords);
                for(size_t i=0;i<N;++i)
                {
                    W[i] = static_cast<WordType>(X);
                    X >>= WordBits;
                }
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup by using local memory to perform algorithm
            inline Split64Into(uint64_t X) noexcept : n( ToWords(X)  ), w()
            {
                DoSplit(Coerce(w),n,X);
                for(size_t i=n;i<MaxWords;++i) Coerce(w[i]) = 0;
            }

            //! cleanup
            inline ~Split64Into() noexcept {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t   n;            //!< 0..MaxWords
            const WordType w[MaxWords];  //!< resulting data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Split64Into);

        };


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
            bytes( BytesFor(qword)            ),
            words( Splitter::WordsFor(bytes)  ),
            nbits( 0 ),
            block( sizeof(qword)              )
            {
                assert(block.words>=Splitter::MaxWords);
                Splitter::DoSplit(block.entry,words,qword);
                updateBitCount();
            }

            inline Proto(const Proto &proto) :
            Object(),
            bytes( proto.bytes ),
            words( proto.words ),
            nbits( proto.nbits ),
            block( proto.block )
            {
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
            //! update bit count
            //__________________________________________________________________
            inline void updateBitCount() noexcept
            {
                if(words) {
                    const size_t    msi = words-1;                         // most significant index
                    const WordType &msw = block.entry[msi]; assert(msw>0); // most significant word
                    Coerce(nbits) = BitCount::For(msw) + msi * WordBits;
                }
                else
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



        private:
            Y_DISABLE_ASSIGN(Proto);
        };

    }

}

#endif

