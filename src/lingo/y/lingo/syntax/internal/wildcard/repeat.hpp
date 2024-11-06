

//! \file

#ifndef Y_Lingo_Syntax_Repeat_Included
#define Y_Lingo_Syntax_Repeat_Included 1

#include "y/lingo/syntax/internal/wildcard.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Repeat : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('X','R','E','P');

                explicit Repeat(const Rule & _rule,
                                const size_t  nmin) :
                Wildcard(NameFor(_rule,nmin), UUID, _rule),
                atLeast(nmin)
                {
                }


                virtual ~Repeat() noexcept;

                const size_t atLeast;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);
                static Caption NameFor(const Rule &, const size_t);
            };

        }

    }

}

#endif

