
//! \file


#ifndef Y_Chemical_Equalizer_OneSided_Included
#define Y_Chemical_Equalizer_OneSided_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/equalizer/cursors.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            //______________________________________________________________
            //
            //
            //
            //! Helper to fix a negative concentration for only[Reac|Prod]
            //
            //
            //______________________________________________________________
            class OneSided
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Equalizer::OneSided"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit OneSided(const Cluster&, const Banks&) noexcept; //!< setup
                virtual ~OneSided() noexcept;                             //!< cleanup


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! fix using only[Reac|Prod] of cluster
                void fix(XMLog &     xml,
                         XWritable & C0,
                         const Level L0);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Cluster &cluster; //!< persistent cluster
                Cursors        cursors; //!< cursors to select extent/species

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OneSided);
            };

        }

    }

}

#endif
