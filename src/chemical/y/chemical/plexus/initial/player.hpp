

//! \file

#ifndef Y_Plexus_Initial_Player_Included
#define Y_Plexus_Initial_Player_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            class Player : public Object
            {
            public:
                static const char * const First;
                static const char * const Extra;

                explicit Player(const int      n,
                                const Species &s) noexcept;
                virtual ~Player() noexcept;
                
                const int      cf;
                const Species &sp;
                Player *       next;
                Player *       prev;
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Player);
            };
        }

    }

}

#endif

