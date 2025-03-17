
#include "y/chemical/plexus/conservation/laws.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Laws:: ~Laws() noexcept
            {

            }


            Laws:: Laws(Rule * rule) :
            Proxy<const ListOf<Law> >(),
            Assembly(),
            my()
            {
                for(;rule;rule=rule->next)
                    enroll( *my.pushTail( new Law(rule) ) );
            }

            Y_PROXY_IMPL(Laws,my)

            void Laws:: xch(Laws &other) noexcept
            {
                my.swapWith(other.my);
                CoerceSwap(maxNameLength,other.maxNameLength);
            }

            std::ostream & operator<<(std::ostream &os, const Laws &laws)
            {
                os << '{';
                if(laws->size>0)
                {
                    os << " #=" << laws->size << std::endl;
                    for(const Law *law=laws->head;law;law=law->next)
                    {
                        os << '\t' << law->name << std::endl;
                    }
                }
                os << '}';
                return os;
            }

            bool Laws:: got(const Species &sp) const noexcept
            {
                for(const Law *law=my.head;law;law=law->next)
                {
                    if(law->has(sp)) return true;
                }
                return false;
            }
        }

    }
}


