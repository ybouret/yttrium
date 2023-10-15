#include "y/chem/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: makeCanons(XMLog &xml)
        {
            Y_XML_SECTION_OPT(xml, "Canons", " conservations='" << cll.size << "'");
            ListOf<Canon> &L = Coerce(law);

            {
                Y_XMLOG(xml, "Gathering");
                for(const Conservation *node=cll.head;node;node=node->next)
                {
                    bool                accepted = false;
                    const Conservation &cons     = *node;

                    for(Canon *canon=L.head;canon;canon=canon->next)
                    {
                        if(canon->accepts(cons))
                        {
                            (*canon) << cons;
                            Y_XMLOG(xml, " (add) " << cons);
                            accepted = true;
                            break;
                        }
                    }

                    if(accepted)
                    {
                        // check fusion
                        CxxListOf<Canon> temp;
                        while(L.size>0)
                        {
                            AutoPtr<Canon> rhs = L.popHead();
                            for(Canon *lhs=temp.head;lhs;lhs=lhs->next)
                            {
                                if(lhs->accepts(*rhs))
                                {
                                    Y_XMLOG(xml, " (-^-) " << *rhs);
                                    lhs->mergeTail(*rhs);
                                    rhs = 0;
                                    break;
                                }
                            }
                            if(rhs.isValid()) temp.pushTail( rhs.yield() );
                        }
                        L.swapWith(temp);
                    }
                    else
                    {
                        // create a new canon
                        Y_XMLOG(xml, " (new) " << cons);
                        L.pushTail( new Canon(cons) );
                    }
                }
            }

            {
                Y_XML_SECTION_OPT(xml, "Law", " canons='" << law.size << "'");
                for(const Canon *canon=law.head;canon;canon=canon->next)
                {
                    Y_XML_SECTION_OPT(xml, "Canon", " size='" << canon->size << "'");
                    for(const Canon::NodeType *cons=canon->head;cons;cons=cons->next)
                    {
                        Y_XMLOG(xml,**cons);
                    }
                }
            }


        }

    }

}
