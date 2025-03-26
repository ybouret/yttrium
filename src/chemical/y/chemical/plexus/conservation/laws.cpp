
#include "y/chemical/plexus/conservation/laws.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"
#include "y/chemical/type/meta-list.hpp"

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
            my(),
            species()
            {
                SList &L = Coerce(species);

                size_t uuid=0;
                for(;rule;rule=rule->next) {
                    Law * const law = my.pushTail( new Law(rule) ); assert(law->latched);
                    for(const Actor *a=(*law)->head;a;a=a->next)
                    {
                        const Species &sp = a->sp;
                        if(L.has(sp)) continue;
                        L << sp;
                    }
                    enroll(*law);
                    Coerce(law->uuid) = ++uuid;
                }

                MetaList<SList>::Sort(L);
            }

            Y_PROXY_IMPL(Laws,my)

            void Laws:: xch(Laws &other) noexcept
            {
                my.swapWith(other.my);
                CoerceSwap(maxNameLength,other.maxNameLength);
                Coerce(species).swapWith( Coerce(other.species) );
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

            
        }

    }
}


