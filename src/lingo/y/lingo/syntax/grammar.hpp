
//! \file

#ifndef Y_Lingo_Syntax_Grammar_Included
#define Y_Lingo_Syntax_Grammar_Included 1

#include "y/lingo/syntax/rules.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Grammar : public Entity, public Proxy<const Rules>
            {
            public:
                template <typename NAME> inline
                explicit Grammar(const NAME & _name) :
                Entity(_name,AsCaption),
                rules(),
                locked(false)
                {
                }

                virtual ~Grammar() noexcept;

                void add(Rule * const  rule);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Rules rules;
                virtual ConstInterface & surrogate() const noexcept;
            public:
                const bool  locked;
            };

        }

    }

}

#endif
