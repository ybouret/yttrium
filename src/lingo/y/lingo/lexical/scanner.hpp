//! \file

#ifndef Y_Lingo_Lexical_Scanner_Included
#define Y_Lingo_Lexical_Scanner_Included 1

#include "y/lingo/lexical/rule.hpp"
#include "y/lingo/pattern/dictionary.hpp"
#include "y/type/proxy.hpp"
#include "y/data/small/light/list/bare.hpp"

#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            typedef Small::BareLightList<Rule>        RList;
            typedef RList::NodeType                   RNode;
            typedef Memory::Wad<RList,Memory::Dyadic> RMaps;

            class Scanner :
            public Quantized,
            public Entity,
            public RMaps,
            public Proxy<const Rules>
            {
            public:

                static const unsigned                     CHARS = 256;
                typedef ArkPtr<String,Scanner>            Pointer;

                template <typename CAPTION>
                explicit Scanner(const CAPTION    &_name,
                                 const Dictionary &_dict) noexcept :
                Entity(_name,AsCaption),
                RMaps(CHARS),
                rules(),
                rlist(lead()),
                dict(_dict)
                {
                    initialize();
                }

                virtual ~Scanner() noexcept; //!< clean


                template <typename RX> inline
                Pattern * compile(const RX &rx)
                {
                    return dict.compile(rx);
                }

                void operator()(Rule * const rule);



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules         rules;
                RList * const rlist;

                void                      initialize() noexcept;
                 virtual ConstInterface & surrogate() const noexcept;
            public:
                Dictionary dict;
            };

        }

    }

}

#endif
