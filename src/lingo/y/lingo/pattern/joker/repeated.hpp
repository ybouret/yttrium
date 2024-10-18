

//! \file

#ifndef Y_Lingo_Repeated_Included
#define Y_Lingo_Repeated_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Repeated : public Joker
        {
        public:
            static const uint32_t UUID = Y_FOURCC('[', '>', '=', ']');

            virtual ~Repeated() noexcept;
            static Pattern * Create(Pattern *, const size_t nmin); //!< with STRONG pattern


            virtual Pattern * clone()             const;          //!< protected clone
            virtual bool      strong()            const noexcept; //!< minimalCount>0
            virtual bool      univocal()          const noexcept; //!< false
            virtual String    regularExpression()          const; //!< (motif)(*|+|[n,])*
            virtual bool      takes(Token &, Source &)     const; //!< if found minimalCount
            virtual size_t    serialize(OutputStream &)    const; //!< uuid + nmin + motif


            const size_t minimalCount;

        private:
            explicit Repeated(Pattern *, const size_t) noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Repeated);
        };
    }

}

#endif

