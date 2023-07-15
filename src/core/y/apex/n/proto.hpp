//! \file

#ifndef Y_Apex_Natural_Proto_Included
#define Y_Apex_Natural_Proto_Included 1

#include "y/object.hpp"
#include "y/apex/number.hpp"
#include "y/apex/m/block.hpp"
#include "y/check/static.hpp"
#include "y/calculus/align.hpp"
#include "y/type/capacity.hpp"
#include "y/text/hexadecimal.hpp"

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

            //! words to wrap a given number of bytes
            static inline size_t WordsFor(const size_t bytes) noexcept
            {
                return Y_ALIGN_ON(WordSize,bytes)/WordSize;
            }

            //! words to wrap a given u64
            static inline size_t ToWords(const uint64_t X) noexcept { return WordsFor( BytesFor(X) ); }

            //! split algorithm to given W[] with precomped N = ToWords(X)
            static inline void   DoSplit(WordType *W, const size_t N, uint64_t X) noexcept
            {
                assert(N==ToWords(X));
                assert(N<=MaxWords);
                for(size_t i=0;i<N;++i)
                {
                    W[i] = static_cast<WordType>(X);
                    X >>= WordBits;
                }
                for(size_t i=0;i<N;++i) std::cerr << " " << Hexadecimal(W[i]);
                std::cerr << std::endl;
            }

            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________

            //! setup by using local memory to perform algorithm
            inline explicit Split64Into(uint64_t X) noexcept :
            n( ToWords(X)  ),
            w()
            {
                std::cerr << "Split(" << Hexadecimal(X) << ") : n = " << n << " :";
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
            typedef typename UnsignedInt<CoreSize>::Type CoreType;                      //!< alias
            typedef typename UnsignedInt<WordSize>::Type WordType;                      //!< alias
            typedef Block<WordType>                      DataType;                      //!< alias
            typedef Split64Into<WordType>                Splitter;                      //!< alias



            inline explicit Proto(const size_t       n,
                                  const AsCapacity_ &)  :
            Object(),
            bytes(0),
            words(0),
            block(n)
            {
                Y_STATIC_CHECK(WordSize<CoreSize,InvalidMetrics);
            }

            inline explicit Proto(const uint64_t qword) :
            Object(),
            bytes( BytesFor(qword)            ),
            words( Splitter::WordsFor(bytes)  ),
            block( sizeof(qword)              )
            {
                assert(block.words>=Splitter::MaxWords);
                Splitter::DoSplit(block.entry,words,qword);
            }


            inline virtual ~Proto() noexcept {}


            size_t   bytes; //!< exact bytes
            size_t   words; //!< aligned to bytes
            DataType block; //!< resources

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Proto);
        };

    }

}

#endif

