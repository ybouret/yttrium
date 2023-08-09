//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/token.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Pattern : public Object, public Token
        {
        public:
            Pattern       *next;
            Pattern       *prev;
            void          *self;
            const uint32_t uuid;

            virtual Pattern * clone() const = 0;

            virtual ~Pattern() noexcept;
        protected:
            explicit Pattern(const uint32_t ) noexcept;
            explicit Pattern(const Pattern &) noexcept;

        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

    }

}

#endif
