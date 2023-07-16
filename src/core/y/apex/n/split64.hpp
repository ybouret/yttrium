//! \file

#ifndef Y_Apex_Natural_Split64_Included
#define Y_Apex_Natural_Split64_Included 1

#include "y/calculus/align.hpp"
#include "y/type/ints.hpp"

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
            static inline size_t GetWordsFor(const size_t bytes) noexcept
            {
                return Y_ALIGN_ON(WordSize,bytes)/WordSize;
            }

            //__________________________________________________________________
            //
            //! words to wrap a given u64
            //__________________________________________________________________
            static inline size_t ToWords(const uint64_t X) noexcept { return GetWordsFor( BytesFor(X) ); }

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

    }

}

#endif

