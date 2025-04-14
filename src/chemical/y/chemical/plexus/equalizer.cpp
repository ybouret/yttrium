#include "y/chemical/plexus/equalizer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const Clusters &cls) :
        Proxy< const ListOf<Equalizing::Proceeding> >(),
        proc(),
        banks(),
        gbank()
        {
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
                proc.pushTail( new Equalizing::Proceeding(*(cls->head),banks,gbank) );
        }

        Y_PROXY_IMPL(Equalizer,proc)

        void Equalizer:: operator()(XMLog     &xml,
                                    XWritable &C0)
        {
            Y_XML_SECTION(xml, "Equalizer");
            for(Equalizing::Proceeding *p=proc.head;p;p=p->next)
                p->fix(xml, C0);

        }

        void Equalizer:: query(XWritable &dC)
        {
            for(Equalizing::Proceeding *p=proc.head;p;p=p->next)
            {
                for(const SNode *sn = p->cluster()->species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    sp(dC,TopLevel) = sp(p->injected,SubLevel).sum();
                }
            }
        }


    }

}


