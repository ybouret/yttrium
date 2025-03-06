
#include "y/chemical/plexus/mixes.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Mixes:: ~Mixes() noexcept
        {
        }


        const char * const Mixes:: CallSign = "Chemical::Mixes";

        Mixes::ConstInterface & Mixes:: surrogate() const noexcept { return my; }

        Mixes:: Mixes(XMLog      &xml,
                      Equilibria &eqs) :
        Proxy<const Mix::List>(),
        my(),
        xbanks(),
        maxOrder(0),
        involved()
        {
            Y_XML_SECTION(xml,CallSign);

            //------------------------------------------------------------------
            //
            // dispatch equilibria
            //
            //------------------------------------------------------------------
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                bool accepted = false;
                for(Mix *mix=my.head;mix;mix=mix->next)
                {
                    if(mix->accepts(eq))
                    {
                        mix->addPrimary(eq);
                        accepted = true;
                        break;
                    }
                }

                if(accepted)
                {
                    // check fusion
                    checkFusion();
                }
                else
                {
                    // create new Mix
                    my.pushTail( new Mix(eq) );
                }
            }


            //------------------------------------------------------------------
            //
            // configure mixes
            //
            //------------------------------------------------------------------
            unsigned hallmark = 0;
            for(Mix *mix=my.head;mix;mix=mix->next)
            {
                Coerce((**mix).hallmark) = ++hallmark;
                mix->buildConfiguration(xml,eqs,xbanks);
                InSituMax( Coerce(maxOrder), mix->order.size() );
                for(const SNode *sn = (*mix)->species.head; sn; sn=sn->next)
                {
                    Coerce(involved.book) += **sn;
                }
            }

            Coerce(involved).compile();
            Y_XML_COMMENT(xml, "maxOrder = " << maxOrder);
            Y_XML_COMMENT(xml, "involved = " << involved.list.size);

        }


        void Mixes:: checkFusion()
        {
            Mix::List store;
            while(my.size>0)
            {
                AutoPtr<Mix> mix = my.popHead();

                for(Mix *host=store.head;host;host=host->next)
                {
                    if( host->accepts(*mix) )
                    {
                        host->addPrimary(*mix);
                        mix.erase();
                        break;
                    }
                }

                if(mix.isValid())
                    store.pushTail( mix.yield() );
            }
            my.swapWith(store);
        }

        std::ostream & operator<<(std::ostream &os, const Mixes &mixes)
        {
            if(mixes->size<=0) return os << "{}";
            os << '{' << std::endl;
            for(const Mix *mix=mixes->head;mix;mix=mix->next)
            {
                os << *mix << std::endl;
            }
            os << '}';

            return os;
        }

        void Mixes:: solve(XMLog &xml, XWritable &C, const Level L, const XReadable &K)
        {
            for(Mix *mix=my.head;mix;mix=mix->next)
                mix->solve(xml,C,L,K);

        }

        void Mixes:: guard(XMLog &xml, XWritable &C, const Level L, XWritable &injected)
        {
            for(Mix *mix=my.head;mix;mix=mix->next)
                mix->guard(xml,C,L,injected);
        }



        void Mixes:: viz(OutputStream &fp, const size_t which) const
        {
            assert(which>0);
            assert(which<=Max<size_t>(1,maxOrder));
            for(const Mix *mix=my.head;mix;mix=mix->next)
                mix->viz(fp, which);
        }

        void Mixes:: graphViz(OutputStream &fp, const size_t which) const
        {
            GraphViz::Vizible::Enter(fp,"G");
            viz(fp,which);
            GraphViz::Vizible::Leave(fp);
        }

    }

}

#include "y/lingo/vfs/find.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Mixes:: renderGraphViz(XMLog &xml, const String &root) const
        {
            Y_XML_SECTION_OPT(xml, "Mixes::renderGraphViz", "maxOrder=" << maxOrder);
            // cleanup temporary/existing files
            {
                const String expr = Lingo::Matching::StringToRegExp(root) + "[:digit:]+.(dot|png)";
                Y_XML_COMMENT(xml, "removing '" << expr << "'");
                Lingo::LocalFileSystem::TryRemove<VFS::Entry::Base>::In(".",expr);
            }

            // render
            for(size_t i=1;i<=maxOrder;++i)
            {
                const String dotFile = root + Formatted::Get("%lu.dot", static_cast<unsigned long>(i));
                Y_XML_COMMENT(xml, "rendering '" << dotFile << "'");
                GraphViz::Vizible::DotToPngEx(dotFile,*this,i);
            }

        }

    }

}

