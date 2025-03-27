
//! \file

#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/broken.hpp"
#include "y/chemical/plexus/cluster.hpp"


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
            //! per Canon matter injection
            //
            //
            //__________________________________________________________________
            class Warden
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Warden(const Cluster &, const Canon &); //!< initialize for canon from cluster
                virtual ~Warden() noexcept;                      //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void operator()(XMLog      &xml,
                                XWritable  &C0,
                                const Level L0);

                std::ostream & display(std::ostream &, const Broken &) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Cluster & cluster;
                const Canon   & canon;
                XAdd            xadd;
                BList           blist;
                XMatrix         cproj;
                XWritable     & c0;
                Summator        injected;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };

           

        }

    }

}

#endif

