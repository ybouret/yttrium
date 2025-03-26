
//! \file


#ifndef Y_Chemical_Reactors_Included
#define Y_Chemical_Reactors_Included 1

#include "y/chemical/plexus/reactor.hpp"
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Reactors per each cluster of Clusters
        //
        //
        //______________________________________________________________________
        class Reactors : public Proxy<const Reactor::List>
        {
        public:

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Reactors(const Clusters &); //!< setup
            virtual ~Reactors()        noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! solve system with current clusters K
            void operator()(XMLog &xml, XWritable &C0);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactors);
            Y_PROXY_DECL();
            Reactor::List   my;
            const Clusters &clusters;

        };
    }

}

#endif
