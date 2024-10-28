//! \file

#ifndef Y_Lingo_Lexical_Scanner_Included
#define Y_Lingo_Lexical_Scanner_Included 1

#include "y/lingo/lexical/rule.hpp"
#include "y/lingo/pattern/dictionary.hpp"
#include "y/type/proxy.hpp"
#include "y/data/small/light/list/bare.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Scanner : public Entity, public Proxy<const Rules>
            {
            public:
                typedef Small::BareLightList<Rule> RList;
                typedef RList::NodeType            RNode;
                
                template <typename CAPTION>
                explicit Scanner(const CAPTION    &_name,
                                 const Dictionary &_dict) noexcept :
                Entity(_name,AsCaption),
                rules(),
                dict(_dict)
                {
                }


                virtual ~Scanner() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules rules;
                virtual ConstInterface & surrogate() const noexcept;
            public:
                Dictionary dict;
            };

        }

    }

}

#endif
