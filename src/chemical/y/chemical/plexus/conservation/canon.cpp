
#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            Canon:: Canon(const Law &first) :
            LList(),
            species(),
            anxious(),
            sformat(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

            Canon:: ~Canon() noexcept
            {
            }

            bool Canon:: accepts(const Law &law) const noexcept
            {
                for(const LNode *ln=head;ln;ln=ln->next)
                {
                    if( (**ln).hasCommonActorWith(law) ) return true;
                }
                return false;
            }


            bool Canon:: accepts(const Canon &rhs) const noexcept
            {
                for(const LNode *l=head;l;l=l->next)
                {
                    if(rhs.accepts(**l)) return true;
                }
                return false;
            }

        }

    }

}

#include "y/text/plural.hpp"
namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            void Canon:: compile(XMLog       & xml,
                                 const EList & definite)
            {
                Y_XML_SECTION_OPT(xml, "Conservation::Canon", "size=" << size);
                assert( 0 == maxNameLength );
                assert( 0 == species->size );
                
                // collect species
                Y_XML_COMMENT(xml, "law" << Plural::s(size));
                for(const LNode *l=head;l;l=l->next)
                {
                    const Law &law = **l; enroll( law );
                    Y_XMLOG(xml,"(+) " <<law.name);
                    for(const Actor *a=law->head;a;a=a->next)
                    {
                        Coerce(species) << a->sp;
                        Coerce(sformat).enroll(a->sp);
                    }
                }

                Y_XML_COMMENT(xml, "species");;
                Y_XMLOG(xml, "(#) " << species);


                // collect definite into anxious
                Y_XML_COMMENT(xml, "anxious");
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }

            }

        }

    }

}
