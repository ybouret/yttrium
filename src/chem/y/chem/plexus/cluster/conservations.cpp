#include "y/chem/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: buildConservations(XMLog &xml)
        {
            const size_t Nc = Qm.rows;
            const size_t M  = Qm.cols; assert(lib.size == M); assert(sdb->size()==M);
            if(Qm.rows>0)
            {
                Y_XML_SECTION_OPT(xml, "Conservation", " count='" << Nc << "'");
                for(size_t i=1;i<=Nc;++i)
                {
                    const Readable<unsigned> &q = Qm[i];
                    std::cerr << "0=d_(";
                    for(size_t j=1;j<=M;++j)
                    {
                        const unsigned nu = q[j];
                        if(nu>0)
                        {
                            const Species &sp = *((*sdb)[j]);
                            std::cerr << "+" << nu << sp;
                        }
                    }
                    std::cerr << ")" << std::endl;
                }
            }
            else
            {
                Y_XMLOG(xml,"No conserved species...");
            }
        }
    }

}
