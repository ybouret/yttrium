
#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Warden:: sanitize(XWritable &C, const Level L, XMLog &xml)
        {

            Y_XML_SECTION(xml, "sanitize" );

            //--------------------------------------------------------------
            //
            // inject corrections and detect zero laws
            //
            //--------------------------------------------------------------
            lawz.free();
            for(const Group *g=head;g;g=g->next)
            {
                const Group                    &G   = *g;
                const Conservation::Law * const law = wasInjected(G,C,L,xml);
                if(0!=law)
                    lawz << *law;
            }

            //--------------------------------------------------------------
            //
            // check CONSERVED with LIMITED
            //
            //--------------------------------------------------------------
            if(0!=head)
            {
                if(lawz.size && xml.verbose)
                {
                    Y_XML_SECTION(xml, "z-laws");
                    for(const LNode *ln=lawz.head;ln;ln=ln->next)
                    {
                        xml() << **ln << std::endl;
                    }
                }
                const SNode * const node = mine.conserved.list.head;

                if(xml.verbose)
                {
                    for(const SNode *sn = node;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        xml() << "d[" << sp << "]=" << cinj[sp.indx[SubLevel]] << std::endl;
                    }
                }


                // check is any conserved is negative => equalize
                for(const SNode *sn=node;sn;sn=sn->next)
                {
                    if( C[ (**sn).indx[L]].mantissa < 0 ) goto EQUALIZE;
                }
                goto ROAMING;

            EQUALIZE:
                equalize(C, L, xml);
            }

            //------------------------------------------------------------------
            //
            // and finally adjust UNBOUNDED with ROAMING
            //
            //------------------------------------------------------------------
        ROAMING:

            // collect wobbly unbounded species
            wobbly.free();
            for(const SNode *sn = mine.unbounded.list.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if( C[ sp.indx[L]].mantissa < 0 )
                    wobbly << sp;

            }

            if(wobbly.size<=0) return;

            Y_XML_SECTION_OPT(xml, "unbounded", wobbly);


        }

    }

}


