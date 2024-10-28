


//! \file

#ifndef Y_Lingo_Lexical_Rule_Included
#define Y_Lingo_Lexical_Rule_Included 1

#include "y/lingo/pattern.hpp"
#include "y/lingo/lexical/unit.hpp"

#include "y/object.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {


            class Rule : public Entity
            {
            protected:
                //! setup rule, need to protect motif
                explicit Rule(const Caption &,
                              Pattern * const) noexcept;

            public:
                virtual ~Rule() noexcept;

                template <typename NAME>
                static Rule * Create(const NAME &name, Pattern * const motif)
                {
                    AutoPtr<Pattern> guard( motif ); assert(0!=motif);
                    const Caption    label( name );
                    Rule *rule = new Rule(label,motif);
                    guard.relax();
                    return rule;

                }

                Pattern * const motif;
                Rule *          next;
                Rule *          prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

            typedef CxxListOf<Rule> Rules;
        }

    }

}

#endif

