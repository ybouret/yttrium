
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/plexus/cluster/combinatorics.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Cluster description
        //
        //
        //______________________________________________________________________
        class Cluster : public ClusterCombinatorics
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const  CallSign; //!< "Cluster"
            typedef CxxListOf<Cluster> List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cluster(XMLog                         &xml,
                             const ClusterContent::Pointer &ptr,
                             Equilibria                    &eqs,
                             XWritable                     &tlK); //!< initialize
            virtual ~Cluster() noexcept;          //!< cleanup
            Y_OSTREAM_PROTO(Cluster);             //!< display

            

            Cluster *      next; //!< for list
            Cluster *      prev; //!< for list
            const unsigned uuid; //!< indexing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            
        };

    }

}

#endif

