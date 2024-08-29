#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Warden:: zeroLaws(XWritable  &C,
                               const Level L,
                               XMLog      &xml)
        {
            assert(lawz.size>0);
            Y_XML_SECTION_OPT(xml, "enforce", "size='" << lawz.size << "'");
            for(const LNode *ln=lawz.head;ln;ln=ln->next)
            {
                const Conservation::Law &law = **ln;
                Y_XML_SECTION(xml,law.name);
                for(const Actor *a = law->head;a;a=a->next)
                {
                    const Species &      sp = a->sp;
                    const size_t * const id = sp.indx;
                    const size_t         J  = id[L];
                    const xreal_t        cc = C[J];
                    Y_XMLOG(xml, std::setw(15) << real_t(cc) << " = [" << sp << "]");
                    switch( Sign::Of(cc.mantissa) )
                    {
                        case __Zero__:
                            break;
                        case Negative:
                        case Positive:
                            cinj[ id[SubLevel] ] << -cc;
                            C[J].ldz();
                            break;
                    }
                }
            }
        }
    }

}

