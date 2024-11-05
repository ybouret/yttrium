//! \file

#ifndef Y_Lingo_Syntax_Rule_Included
#define Y_Lingo_Syntax_Rule_Included 1

#include "y/lingo/source.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            class Rule : public Entity
            {
            protected:
                template <typename NAME> inline
                explicit Rule(const NAME &   _name,
                              const uint32_t _uuid):
                Entity(_name,AsCaption),
                uuid(_uuid),
                self(0)
                {
                }

            public:
                virtual ~Rule() noexcept;

            public:
                const uint32_t uuid;

            protected:
                void * const   self;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };
        }
    }
}

#endif
