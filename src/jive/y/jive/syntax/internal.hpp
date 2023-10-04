//! \file

#ifndef Y_Jive_Syntax_Internal_Included
#define Y_Jive_Syntax_Internal_Included 1

#include "y/jive/syntax/rule.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            class Internal : public Rule
            {
            public:


                virtual ~Internal() noexcept;


            private:
                template <typename ID>
                explicit Internal(const ID &id, const uint32_t dw) : Rule(id,dw)
                {
                }

                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

        }

    }

}

#endif

