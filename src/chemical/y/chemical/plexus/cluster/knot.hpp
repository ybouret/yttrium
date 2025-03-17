
//! \file

#ifndef Y_Chemical_ClusterKnot_Included
#define Y_Chemical_ClusterKnot_Included 1

#include "y/chemical/plexus/cluster/content.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Knot of dependent equilibria
        //
        //
        //______________________________________________________________________
        class ClusterKnot : public Quantized, public ClusterContent::MutablePointer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterKnot(const ClusterContent::MutablePointer &) noexcept; //!< create
            virtual ~ClusterKnot() noexcept;                                       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            ClusterKnot *next; //!< for list
            ClusterKnot *prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterKnot);
        };
    }

}


#endif

