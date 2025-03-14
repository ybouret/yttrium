
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/plexus/cluster/type.hpp"
#include "y/container/matrix.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Clusters;
        class Equilibria;
        
        typedef Matrix<int>      iMatrix; //!< alias
        typedef Matrix<unsigned> uMatrix; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Cluster description
        //
        //
        //______________________________________________________________________
        class Cluster : public Object, public Proxy<const ClusterType>, public Latchable
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
            explicit Cluster(Equilibrium &first); //!< initialize
            virtual ~Cluster() noexcept;          //!< cleanup
            Y_OSTREAM_PROTO(Cluster);             //!< display


            //__________________________________________________________________
            //
            //
            // Construction Methods
            //
            //__________________________________________________________________
            void attach(Equilibrium &);                       //!< sanity check and link equilibrium/species
            bool accepts(const Equilibrium &) const noexcept; //!< shared species
            bool accepts(const Cluster &)     const noexcept; //!< shared species
            void attach(Cluster &);                           //!< steal content (if not latched)

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const iMatrix topology;    //!< topology
            const iMatrix topologyT;   //!< transpose(topology)
            const uMatrix preserved;   //!< preserved matrix for conservation

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            ClusterType my;

            friend class Clusters;
            Y_PROXY_DECL();
            void compile(XMLog &, Equilibria &);
            void conservations(XMLog &);
            void combinatorics(XMLog &, Equilibria &);

        public:
            Cluster *next; //!< for list
            Cluster *prev; //!< for list

        };

    }

}

#endif

