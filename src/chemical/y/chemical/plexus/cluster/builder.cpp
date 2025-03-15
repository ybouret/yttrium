
#include "y/chemical/plexus/cluster/builder.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Y_PROXY_IMPL(ClusterBuilder,cls)

        ClusterBuilder:: ~ClusterBuilder() noexcept
        {
        }

        void ClusterBuilder:: startClusterFor(Equilibrium &eq)
        {
            ClusterContent::MutablePointer  ptr = new ClusterContent(eq);
            cls.pushTail( new ClusterNode(ptr) );
        }

        ClusterBuilder:: ClusterBuilder(XMLog &xml, Equilibria &eqs)
        {
            Y_XML_SECTION(xml, "ClusterBuilder");

            for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
            {
                Equilibrium &eq = **it;
                for(ClusterNode *cl=cls.head;cl;cl=cl->next)
                {
                    ClusterContent &cc = **cl;
                    if(cc.accepts(eq))
                    {
                        cc.attach(eq);
                        goto CHECK_FUSION;
                    }
                }

                Y_XML_COMMENT(xml, "start cluster for '" << eq.name << "'");
                startClusterFor(eq);
                continue;

                CHECK_FUSION:;
                {
                    CxxListOf<ClusterNode> ok;
                    while(cls.size>0)
                    {
                        AutoPtr<ClusterNode> lhs = cls.popHead();
                        for(ClusterNode *rhs=ok.head;rhs;rhs=rhs->next)
                        {
                            if( (**rhs).accepts(**lhs))
                            {
                                Y_XML_COMMENT(xml, "fusion " << *rhs << " and " << *lhs);
                                (**rhs).fusion(**lhs);
                                lhs.erase();
                                break;
                            }
                        }

                        if(lhs.isValid()) ok.pushTail(lhs.yield());
                    }
                    cls.swapWith(ok);
                }
            }
            Y_XML_COMMENT(xml, "built #cluster=" << cls.size );
        }

    }

}
