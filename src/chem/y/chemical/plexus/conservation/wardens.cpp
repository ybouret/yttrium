
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            Y_PROXY_IMPL(Wardens,my)

            Wardens:: ~Wardens() noexcept
            {
            }

            Wardens:: Wardens(const Mix &mix) :
            Proxy<const Warden::List>(),
            my(),
            squad(*mix)
            {
                
                if(mix.auth.isValid())
                {
                    for(const Act *act = mix.auth->acts->head;act;act=act->next)
                        my.pushTail( new Warden(*act) );
                }

            }

            void Wardens:: run(XMLog &     xml,
                               XWritable & C,
                               const Level L,
                               XWritable & injected)
            {
                Y_XML_SECTION_OPT(xml,"Conservation::Wardens", "count=" << my.size);
                squad.ldz(injected,TopLevel);
                for(Warden *warden=my.head;warden;warden=warden->next)
                    warden->run(xml,C,L,injected);
            }
        }
    }
}
