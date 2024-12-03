//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1


#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        //______________________________________________________________________
        //
        //
        //
        //! Cluster of Linked equilibria
        //
        //
        //______________________________________________________________________
        class Cluster :  public Proxy<const Grouping>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Cluster> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cluster(const Equilibrium &first); //!< setup with first equilibria
            virtual ~Cluster() noexcept;                //!< cleanup
            Y_OSTREAM_PROTO(Cluster);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void add(const Equilibrium &eq);
            void add(Cluster * const) noexcept;      //!< add content and delete cluster
            bool accepts(const Equilibrium &) const; //!< true is equilibrium is linked to a cluster's one
            bool accepts(const Cluster     &) const; //!< true is some species are shared



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            Grouping eqs; //!< content

            virtual ConstInterface & surrogate() const noexcept;
        public:
            Cluster *next; //!< for list
            Cluster *prev; //!< for list
        };
    }

}


#endif

