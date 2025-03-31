
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

            //__________________________________________________________________
            //
            //
            //
            //! manage all wardens per canons of clusters
            //
            //
            //__________________________________________________________________
            class Wardens : public Proxy<const ListOf<Warden> >
            {
            public:
                //__________________________________________________________________
                //
                //
                // C++
                //
                //__________________________________________________________________
                explicit Wardens(const Clusters &); //!< create all wardens
                virtual ~Wardens() noexcept;        //!< cleanup

                //__________________________________________________________________
                //
                //
                // Methods
                //
                //__________________________________________________________________

                //! fix TopLevel C0 and accumulate injected
                void operator()(XMLog &xml, XWritable &C0);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wardens);
                Y_PROXY_DECL();
                CxxListOf<Warden> my;
                const Clusters   &cs;
            };


        }

    }

}

#endif

