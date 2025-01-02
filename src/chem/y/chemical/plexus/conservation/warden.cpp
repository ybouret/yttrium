
#include "y/chemical/plexus/conservation/warden.hpp"

#include "y/chemical/plexus/conservation/act.hpp"
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace Conservation
        {
            Warden:: ~Warden() noexcept
            {
            }

            Warden:: Warden(const Mix & _mix,
                            const Act & _act) :
            mix(_mix),
            act(_act)
            {

                std::cerr << "Warden for #act=" << act->size << std::endl;

            }

            

        }

    }

}

