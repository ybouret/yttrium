
#include "y/chemical/reactive/clusters.hpp"
#include "y/lingo/vfs/find.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace Lingo;


        bool Clusters:: haveOrder(const size_t n) const noexcept
        {
            for(const Cluster *cl=my.head;cl;cl=cl->next)
            {
                if(cl->order.size()>=n && cl->order[n].size>0) return true;
            }
            return false;
        }


        void Clusters:: graphViz(const String &root, const bool showConservations) const
        {
            {
                String expr;
                for(size_t i=1;i<=root.size();++i)
                {
                    expr += Formatted::Get("\\x%02x",unsigned(root[i]));
                }
                expr += "[:digit:]+";
                expr += "\\.";
                expr += "(dot|png)";

                LocalFileSystem::TryRemove<VFS::Entry::Base>::In(".", expr);
                
#if 0
                std::cerr << expr << std::endl;
                VFS::Entries entries;
                VirtualFileSystem::Find( LocalFileSystem::Get(), entries, ".", expr, Matching::Exactly, VFS::Entry::Base);
                std::cerr << entries << std::endl;
#endif
            }

            for(size_t i=1;i<=maxOrder;++i)
            {
                if( !haveOrder(i) ) continue;
                const String fn = root + Formatted::Get("%u.dot", unsigned(i));
                {
                    OutputFile   fp(fn);

                    GraphViz::Vizible::Enter(fp,"G");

                    for(const Cluster *cluster=my.head;cluster;cluster=cluster->next)
                    {
                        const Cluster &cl = *cluster;     if(cl.order.size()<i) continue;
                        const EList   &el = cl.order[i];  if(el.size<=0)        continue;

                        fp << "subgraph cluster_" << Formatted::Get("%u",cl.indx) << "{\n";

                        // write species
                        for(const SNode *sn=cl->species.head;sn;sn=sn->next)
                        {
                            const Species &sp = **sn;
                            const String   color = sp.makeColor();
                            sp.viz(fp, color.c_str(), 0);
                        }

                        // write components
                        for(const ENode *en=el.head;en;en=en->next)
                        {
                            const Components &cm    = **en;
                            const String      color = cm.makeColor();
                            cm.viz(fp, color.c_str(), 0);

                        }

                        // write laws
                        if(1==i && showConservations)
                        {
                            for(const Conservation::Law *ln=cl.laws->head;ln;ln=ln->next)
                            {
                                const Conservation::Law &law = *ln;
                                law.viz(fp);
                            }
                        }
                        fp << "}\n";
                    }

                    GraphViz::Vizible::Leave(fp);
                }

                GraphViz::Vizible::RenderPNG(fn,false);

            }



        }
    }

}


