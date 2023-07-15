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

        template <typename WordType>
        class Split64Into
        {
        public:
            static const unsigned WordSize  = sizeof(WordType);
            static const unsigned WordBits  = WordSize << 3;
            static const unsigned SelfSize = sizeof(uint64_t);
            static const unsigned MaxWords = Y_ALIGN_ON(WordSize,SelfSize)/WordSize;

            static inline size_t WordsFor(const size_t bytes) noexcept
            {
                return Y_ALIGN_ON(WordSize,bytes)/WordSize;
            }

            static inline size_t ToWords(const uint64_t X) noexcept { return WordsFor( BytesFor(X) ); }
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

            inline explicit Split64Into(uint64_t X) noexcept :
            n( ToWords(X)  ),
            w()
            {
                std::cerr << "Split(" << Hexadecimal(X) << ") : n = " << n << " :";
                DoSplit(Coerce(w),n,X);
                for(size_t i=n;i<MaxWords;++i) Coerce(w[i]) = 0;
            }

            inline ~Split64Into() noexcept {}

            const size_t   n;
            const WordType w[MaxWords];

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Split64Into);

        };


        template <typename CORE_TYPE, typename WORD_TYPE>
        class Proto : public Object
        {
        public:
            static const unsigned                        CoreSize  = sizeof(CORE_TYPE);
            static const unsigned                        WordSize  = sizeof(WORD_TYPE);
            static const unsigned                        WordBits  = WordSize << 3;
            typedef typename UnsignedInt<CoreSize>::Type CoreType;
            typedef typename UnsignedInt<WordSize>::Type WordType;
            typedef Block<WordType>                      DataType;
            typedef Split64Into<WordType>                Splitter;

            static inline size_t WordsFor(const size_t num) noexcept
            {
                return Y_ALIGN_ON(WordSize,num)/WordSize;
            }


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
            bytes( BytesFor(qword)  ),
            words( WordsFor(bytes)  ),
            block( sizeof(qword)    )
            {
                assert(block.words>=Splitter::MaxWords);
                Splitter::DoSplit(block.entry,words,qword);
            }


            inline virtual ~Proto() noexcept
            {}


            size_t   bytes; //!< exact bytes
            size_t   words; //!< aligned to bytes
            DataType block; //!< resources

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Proto);
        };

    }

}

#endif

