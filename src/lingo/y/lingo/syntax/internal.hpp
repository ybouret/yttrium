

//! \file

#ifndef Y_Lingo_Syntax_Internal_Included
#define Y_Lingo_Syntax_Internal_Included 1

#include "y/lingo/syntax/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
#if 0
            Wildcard;
            Compound;

            Option;
            Repeat;

            Aggregate;
            Alternate;
#endif
            
            class Internal : public Rule
            {

            protected:
                template <typename NAME>
                explicit Internal(const NAME &   _name,
                                  const uint32_t _uuid) :
                Rule(_name,_uuid)
                {
                }

            public:
                virtual ~Internal() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

        }

    }

}


#endif

