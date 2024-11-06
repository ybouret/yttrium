
//! \file

#ifndef Y_Lingo_Syntax_Option_Included
#define Y_Lingo_Syntax_Option_Included 1

#include "y/lingo/syntax/internal/wildcard.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Option : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('X','O','P','T');

                template <typename NAME> inline
                explicit Option(const NAME & _name,
                                const Rule & _rule) :
                Wildcard(_name, UUID, _rule)
                {
                }


                virtual ~Option() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Option);
            };

        }

    }

}

#endif

