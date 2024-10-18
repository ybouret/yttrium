
//! \file

#ifndef Y_Lingo_Optional_Included
#define Y_Lingo_Optional_Included 1

#include "y/lingo/pattern/joker.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Optional : public Joker
        {
        public:
            static const uint32_t UUID = Y_FOURCC('O', 'P', 'T', '?');

            virtual ~Optional() noexcept;
            static Pattern * Create(Pattern *);


            virtual Pattern * clone()             const;          //!< protected clone
            virtual bool      strong()            const noexcept; //!< false
            virtual bool      univocal()          const noexcept; //!< true
            virtual String    regularExpression()          const; //!< (motif)?
            virtual bool      takes(Token &, Source &)     const; //!< always true
            virtual size_t    serialize(OutputStream &)    const; //!< uuid + motif

        private:
            explicit Optional(Pattern *) noexcept;;
            Y_DISABLE_COPY_AND_ASSIGN(Optional);
        };
    }

}

#endif

