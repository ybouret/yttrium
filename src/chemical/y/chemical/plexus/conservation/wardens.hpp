
//! \file

#ifndef Y_Chemical_Conservation_Wardens_Included
#define Y_Chemical_Conservation_Wardens_Included 1

#include "y/chemical/plexus/conservation/warden.hpp"
#include "y/chemical/plexus/clusters.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            class Wardens : public Proxy<const ListOf<Warden> >
            {
            public:
                explicit Wardens(const Clusters &);
                virtual ~Wardens() noexcept;

                void operator()(XMLog     &xml,
                                XWritable &C0,
                                XWritable &I0);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wardens);
                Y_PROXY_DECL();
                CxxListOf<Warden> my;
            };


        }

    }

}

#endif

