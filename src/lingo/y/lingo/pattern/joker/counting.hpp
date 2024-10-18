


//! \file

#ifndef Y_Lingo_Counting_Included
#define Y_Lingo_Counting_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Counting : public Joker
        {
        public:
            static const uint32_t UUID = Y_FOURCC('[','.','.',']');

            virtual ~Counting() noexcept;
            static Pattern * Create(Pattern *, const size_t nmin, const size_t nmax); //!< with STRONG pasttern


            virtual Pattern * clone()             const;          //!< protected clone
            virtual bool      strong()            const noexcept; //!< nmin>0 && motif->stroing
            virtual bool      univocal()          const noexcept; //!< nmin==nmax && motif->univocal
            virtual String    regularExpression()          const; //!< (motif)[nmin,nmax]
            virtual bool      takes(Token &, Source &)     const; //!< if minimalCount <= found <= maximalCount, GREEDY
            virtual size_t    serialize(OutputStream &)    const; //!< uuid + nmin + nmax + motif


            const size_t minimalCount;
            const size_t maximalCount;

        private:
            explicit Counting(Pattern *, const size_t, const size_t) noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Counting);
        };
    }

}

#endif

