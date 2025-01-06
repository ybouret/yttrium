
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
            class Wardens : public Proxy<const Warden::List>
            {
            public:
                explicit Wardens(const Mix &);  
                virtual ~Wardens() noexcept;


                void run(XMLog &      xml,
                         XWritable &  C,
                         const Level  L,
                         XWritable  & injected);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wardens);
                Y_PROXY_DECL();
                Warden::List my;
                const Squad &squad;
                
            };
        }
    }
}

#endif

