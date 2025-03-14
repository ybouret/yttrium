#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Y_PROXY_IMPL(Clusters,my)

        Clusters:: ~Clusters() noexcept
        {
            
        }

        
        Clusters:: Clusters(XMLog &xml, Equilibria &eqs) :
        my()
        {

            Y_XML_SECTION_OPT(xml, "Clusters", "#eqs=" << eqs->size());
            setup(xml,eqs);
            compile(xml);
        }

        void Clusters:: setup(XMLog &xml, Equilibria &eqs)
        {
            Y_XML_SECTION(xml, "setup");

            for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
            {
                Equilibrium &eq = **it;
                for(Cluster *cl=my.head;cl;cl=cl->next)
                {
                    if(cl->accepts(eq))
                    {
                        cl->attach(eq);
                        goto CHECK_FUSION;
                    }
                }

                Y_XML_COMMENT(xml, "new cluster with '" << eq.name << "'");
                my.pushTail( new Cluster(eq) );
                continue;

            CHECK_FUSION:
                {
                    Cluster::List ok;
                    while(my.size>0)
                    {
                        AutoPtr<Cluster> lhs = my.popHead();
                        for(Cluster *rhs=ok.head;rhs;rhs=rhs->next)
                        {
                            if(rhs->accepts(*lhs))
                            {
                                Y_XML_COMMENT(xml, "fusion " << *rhs << " and " << *lhs);
                                rhs->attach(*lhs);
                                lhs.erase();
                                break;
                            }
                        }

                        if(lhs.isValid()) ok.pushTail(lhs.yield());
                    }
                    my.swapWith(ok);
                }
            }
            Y_XML_COMMENT(xml, "constructed #cluster=" << my.size );
            for(Cluster *cl=my.head;cl;cl=cl->next)
            {
                cl->latch();
                Y_XMLOG(xml, "(+) " << *cl);
            }
        }


        void Clusters:: compile(XMLog &xml)
        {
            Y_XML_SECTION(xml, "compile");
            for(Cluster *cl=my.head;cl;cl=cl->next)
            {
                assert(cl->latched);
                cl->compile(xml);
            }
        }

    }

}

