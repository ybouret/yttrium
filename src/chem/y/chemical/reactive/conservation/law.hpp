
//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {



            class Law : public Actors
            {
            public:
                static const char * const CallSign;
                explicit Law(const Actor::List &);
                virtual ~Law() noexcept;

                const xReal   denom; //!< alpha^2
                const XArray  alpha; //!< weights of actors
                const XMatrix xproj; //!< Id * alpha^2 - |alpha><alpha|
                Law * next;
                Law * prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };

        }

    }

}

#endif
