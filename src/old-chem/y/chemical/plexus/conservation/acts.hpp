
//! \file

#ifndef Y_Chemical_Conservation_Acts_Included
#define Y_Chemical_Conservation_Acts_Included 1

#include "y/chemical/plexus/conservation/act.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        class Mix;

        namespace Conservation
        {
            class Laws;

            //__________________________________________________________________
            //
            //
            //
            //! Indepedent Acts of dependent Laws
            //
            //
            //__________________________________________________________________
            class Acts : public Proxy<const Act::List>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Acts(const Laws &laws); //!< create acts
                virtual ~Acts() noexcept;        //!< cleanup


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Acts);
                Y_PROXY_DECL();
                void reconnect();
                
                Act::List my;
            };
        }

    }

}

#endif
