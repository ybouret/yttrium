
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
            XArray(mix->species.size),
            my()
            {
                if( mix.auth.isValid() )
                {
                    for(const Act *act = mix.auth->acts->head;act;act=act->next)
                        my.pushTail( new Warden(*act) );
                }
            }
        }
    }
}
