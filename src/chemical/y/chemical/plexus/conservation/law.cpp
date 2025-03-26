

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Law:: ~Law() noexcept
            {
            }

            static unsigned ActorsNorm2(const Actors &actors)
            {
                apn n2 = 0;
                for(const Actor *a=actors->head;a;a=a->next)
                {
                    n2 += Squared(a->nu);
                }
                return n2.cast<unsigned>(actors.name->c_str());
            }

            Law:: Law(Rule * const rule) :
            Actors(rule->in),
            norm2(ActorsNorm2(*rule)),
            uuid(0),
            next(0),
            prev(0)
            {
                Actors::Exchange(*this,*rule);
                latch();
            }


            void Law:: viz(OutputStream &fp, const String * const color) const
            {
                Node(fp,this) << '[';
                Label(fp,*name);
                if(color) fp << ',' << *color;
                fp << ",shape=box,style=rounded";
                Endl(fp << ']');
                for(const Actor *a=(*this)->head;a;a=a->next)
                {
                    Arrow(fp,this,&a->sp) << '[';
                    fp << "arrowhead=odot";
                    if(color) fp << ',' << *color;
                    Endl(fp << ']');
                }
            }

        }
    }

}

