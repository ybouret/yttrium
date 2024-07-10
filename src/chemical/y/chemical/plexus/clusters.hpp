
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of independant clusters
        //
        //
        //______________________________________________________________________
        class Clusters : public Proxy<const Cluster::List>
        {
        public:

            explicit Clusters(Equilibria &, XMLog &); //!< setup from equilibria
            virtual ~Clusters() noexcept;             //!< cleanup

            //! in order K evaluation
            const XReadable & K(const xreal_t t);

            void show(std::ostream &os) const;
            void showK(std::ostream &os) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            virtual ConstInterface & surrogate() const noexcept;
            Cluster::List   cls;
            Vector<xreal_t> shK;


        };

    }

}

#endif

