

#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/law.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        class Mix;

        namespace Conservation
        {

            class Act;

            class Warden
            {
            public:

                explicit Warden(const Mix & _mix,
                                const Act & _act);

                virtual ~Warden() noexcept;

                const Mix & mix;
                const Act & act;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }

    }

}

#endif

