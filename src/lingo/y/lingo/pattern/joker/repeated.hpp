

//! \file

#ifndef Y_Lingo_Repeatedl_Included
#define Y_Lingo_Repeated_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Repeated : public Joker
        {
        public:
            static const uint32_t UUID = Y_FOURCC('R', 'P', '>', '=');

            virtual ~Repeated() noexcept;
            static Pattern * Create(Pattern *, const size_t nmin);


            virtual Pattern * clone()             const;          //!< protected clone
            virtual bool      strong()            const noexcept; //!< false
            virtual bool      univocal()          const noexcept; //!< false
            virtual String    regularExpression()          const; //!< (motif)(*|+|[n,])*
            virtual bool      takes(Token &, Source &)     const; //!< if found minimalCount
            virtual size_t    serialize(OutputStream &)    const; //!< uuid + motif


            const size_t minimalCount;

        private:
            explicit Repeated(Pattern *, const size_t) noexcept;
            Y_DISABLE_COPY_AND_ASSIGN(Repeated);
        };
    }

}

#endif

