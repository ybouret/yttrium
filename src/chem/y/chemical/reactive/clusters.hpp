
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1


#include "y/chemical/reactive/cluster.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Independent clusters of dependents equilibria
        //
        //
        //______________________________________________________________________
        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            static const char * const CallSign;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Clusters(const Equilibria &eqs, XMLog &xml); //!< setup
            virtual ~Clusters() noexcept;                         //!< cleanup
            Y_OSTREAM_PROTO(Clusters);                            //!< eqs+species

        private:
            Cluster::List cls;

            virtual ConstInterface & surrogate() const noexcept;
            void    checkFusion() noexcept;
        };

    }

}

#endif

