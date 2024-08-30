#include "y/chemical/plexus/warden.hpp"
#include "y/system/exception.hpp"

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

            //------------------------------------------------------------------
            //
            // process limited equilibria
            //
            //------------------------------------------------------------------
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


            //------------------------------------------------------------------
            //
            assert(unbalanced>0);
            //
            //------------------------------------------------------------------
            const size_t tradeCount = trades.size();
            if(xml.verbose)
            {
                xml() << "(#) tradeCount = " << tradeCount << " / " << wobbly << std::endl;
                for(const SNode *sn=wobbly.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    xml() << "(!) " << std::setw(15) << real_t( C[sp.indx[L]]) << " = [" << sp << "]" << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            // check if best effort was done
            //
            //------------------------------------------------------------------
            if(tradeCount<=0)
            {
                if(lawz.size<=0)
                {
                    // bad!
                    throw Specific::Exception(CallSign, "No Fail-Safe Conservation Law");
                }
                zeroLaws(C,L,xml);
                return;
            }

            //------------------------------------------------------------------
            //
            // improve concentrations
            //
            //------------------------------------------------------------------
            optimizeTrade(C, L, xml);
            goto CYCLE;
        }
    }

}

