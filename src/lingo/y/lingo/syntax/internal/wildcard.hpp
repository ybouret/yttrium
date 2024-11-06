
//! \file

#ifndef Y_Lingo_Syntax_Wildcard_Included
#define Y_Lingo_Syntax_Wildcard_Included 1

#include "y/lingo/syntax/internal.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Wildcard : public Internal
            {
            protected:
                template <typename NAME> inline
                explicit Wildcard(const NAME   & _name,
                                  const uint32_t _uuid,
                                  const Rule   & _rule) :
                Internal(_name,_uuid),
                rule(_rule)
                {
                    checkRobustness();
                }

            public:
                virtual ~Wildcard() noexcept;

                const Rule & rule;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
                void checkRobustness() const;
            };
        }

    }

}

#endif


