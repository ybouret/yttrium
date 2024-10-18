


//! \file

#ifndef Y_Lingo_Counting_Included
#define Y_Lingo_Counting_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Counting STRONG motif, greedy style
        //
        //
        //______________________________________________________________________
        class Counting : public Joker
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('[',':',':',']'); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Counting() noexcept;
            static Pattern * Create(Pattern *, const size_t nmin, const size_t nmax); //!< with STRONG pasttern

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual Pattern * clone()             const;          //!< [Pattern] Create(motif,min,max)
            virtual bool      strong()            const noexcept; //!< [Pattern] nmin>0 && motif->stroing
            virtual bool      univocal()          const noexcept; //!< [Pattern] nmin==nmax && motif->univocal
            virtual String    regularExpression()          const; //!< [Pattern] (motif)[nmin,nmax]
            virtual bool      takes(Token &, Source &)     const; //!< [Pattern] if minimalCount <= found <= maximalCount, GREEDY
            virtual size_t    serialize(OutputStream &)    const; //!< [Serializable] uuid + nmin + nmax + motif

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t minimalCount; //!< at least  minimalCount
            const size_t maximalCount; //!< and won't go further than maximal count

        private:
            explicit Counting(Pattern *, const size_t, const size_t) noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Counting);
        };
    }

}

#endif

