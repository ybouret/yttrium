#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Warden:: equalize(XWritable &C, const Level L, XMLog &xml)
        {
            if(0==head) return;

            Y_XML_SECTION(xml, "equalize" );


            size_t cycle = 0;
        CYCLE:
            ++cycle;
            Y_XMLOG(xml, "-------- #cycle = " << cycle << " --------");

            //--------------------------------------------------------------
            //
            // process limited equilibria
            //
            //--------------------------------------------------------------
            const size_t unbalanced = getUnbalanced(C, L, xml);
            Y_XMLOG(xml, "(#) unbalanced = " << unbalanced);
            if(unbalanced<=0)
            {
                //----------------------------------------------------------
                //
                // numerical succes, check lawz
                //
                //----------------------------------------------------------
                assert(0==trades.size());
                assert(0==wobbly.size);
                if(lawz.size>0)
                    zeroLaws(C,L,xml); // will set positive concentrations to zero
                else
                {
                    Y_XMLOG(xml, "(#) no z-law to enforce");
                }
                return;
            }

            const size_t tradeCount = trades.size();
            Y_XMLOG(xml, "(#) tradeCount = " << tradeCount << " / " << wobbly);
            for(const SNode *sn=wobbly.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                Y_XMLOG(xml, "(!) " << std::setw(15) << real_t( C[sp.indx[L]]) << " = [" << sp << "]");
            }

            if(tradeCount<=0)
            {
                std::cerr << "No Trade!!" << std::endl;
                if(!lawz.size)
                {
                    // bad!
                    throw Specific::Exception("here", "no lawz");
                }
                zeroLaws(C,L,xml);
                std::cerr << "Emergency Exit!!" << std::endl << std::endl;
                exit(9);
                return;
            }


            optimizeTrade(C, L, xml);
            goto CYCLE;
        }
    }

}

