
//! \file

#ifndef Y_Chemical_Conservation_Wardens_Included
#define Y_Chemical_Conservation_Wardens_Included 1

#include "y/chemical/plexus/conservation/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Mix;

        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            //! Collection of Warden for all acts of a Mix
            //
            //
            //__________________________________________________________________
            class Wardens : public Proxy<const Warden::List>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Wardens(const Mix &); //!< setup
                virtual ~Wardens() noexcept;   //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! apply each warden and update topLevel injected
                void run(XMLog &      xml,
                         XWritable &  C,
                         const Level  L,
                         XWritable  & injected);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wardens);
                Y_PROXY_DECL();
                Warden::List my;
                const Squad &squad; //!< used to zero injected

            };
        }
    }
}

#endif

