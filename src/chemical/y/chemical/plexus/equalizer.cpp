#include "y/chemical/plexus/equalizer.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {

            Proceedings:: ~Proceedings() noexcept
            {
            }

            Proceedings:: Proceedings(const Clusters &cls) :
            Proxy< const ListOf<Proceeding> >(),
            proc(),
            banks(),
            gbank()
            {
                for(const Cluster *cl=cls->head;cl;cl=cl->next)
                    proc.pushTail( new Proceeding(*(cls->head),banks,gbank) );
            }

            Y_PROXY_IMPL(Proceedings,proc)

            void Proceedings:: session(XMLog     &xml,
                                       XWritable &C0,
                                       Summator  &I0)
            {
                Y_XML_SECTION(xml, "Equalizing::Proceedings");
                I0.forEach( & XAdd::free );
                for(Proceeding *p=proc.head;p;p=p->next)
                {
                    p->session(xml, C0, I0);
                }
            }

        }

    }

}
