

#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/stream/xmlog.hpp"

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
                static const char * const CallSign;

                explicit Warden(const Mix & _mix,
                                const Act & _act);

                virtual ~Warden() noexcept;

                const Mix & mix;
                const Act & act;
                XAdd        xadd;
                
                void run(XMLog &xml,XWritable &C, const Level L);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }

    }

}

#endif

