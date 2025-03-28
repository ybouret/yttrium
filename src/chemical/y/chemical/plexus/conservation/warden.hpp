
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

                //! fix broken values, setup injected
                void operator()(XMLog      &xml,
                                XWritable  &C0,
                                const Level L0);

                //! helper to format broken value
                std::ostream & display(std::ostream &, const Broken &) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Cluster & cluster;  //!< parent cluster
                const Canon   & canon;    //!< parent canon
                XAdd            xadd;     //!< helper for additions
                BList           blist;    //!< list of broken value
                XMatrix         cproj;    //!< storage of projected values
                XWritable     & c0;       //!< temporary value
                Summator        injected; //!< injected values per species
                Warden *        next;     //!< for list
                Warden *        prev;     //!< for list
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };

           

        }

    }

}

#endif

