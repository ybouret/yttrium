//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1


#include "y/chemical/reactive/cluster/grouping.hpp"
#include "y/chemical/reactive/conservation/laws.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/ptr/drill-down.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! syncronized list and addresses of list
        //
        //
        //______________________________________________________________________
        template <typename LIST>
        class Roster
        {
        public:

            inline  Roster() : list(), book() {} //!< setup
            inline ~Roster() noexcept         {} //!< cleanup

            //! collect book into list and sort
            void compile() {
                LIST &mine = Coerce(list);
                mine.release();
                book.sendTo(mine);
                DBOps::Revamp<LIST>::Sort(mine);
            }

            const LIST        list; //!< list
            const AddressBook book; //!< addresses

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Roster);
        };

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
            typedef CxxListOf<Cluster>                    List;     //!< alias
            typedef AutoPtr<Conservation::Laws,DrillDown> LawsPtr;  //!< alias
            typedef CxxArray<EList>                       ELists;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cluster(const Equilibrium &first); //!< setup with first equilibria
            virtual ~Cluster() noexcept;                //!< cleanup
            Y_OSTREAM_PROTO(Cluster);                   //!< display as list of eqs

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void addPrimary(const Equilibrium &eq);  //!< add single equilibrium and upgrade
            void addPrimary(Cluster * const);        //!< add content, delete source and upgrade
            bool accepts(const Equilibrium &) const; //!< true if equilibrium is linked to a cluster's one
            bool accepts(const Cluster     &) const; //!< true if some species are shared
            void compile(Equilibria &eqs,
                         XMLog      &xml); //!< post-build, called by Clusters

            bool hasConserved(const Actors &) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            virtual ConstInterface & surrogate() const noexcept;
            void                     classifySpecies(XMLog &);

            Grouping                    my;   //!< content
        public:
            const LawsPtr               laws;        //!< conservation laws
            const Matrix<unsigned>      conservancy; //!< conservation matrix
            const Roster<SList>         conserved;   //!< roster of conserved species
            const Roster<SList>         unbounded;   //!< roster of unbounded species
            const Roster<EList>         reacOnly;    //!< roster of reacOnly equilibria
            const Roster<EList>         prodOnly;    //!< roster of prodOnly equilibria
            const Roster<EList>         rigorous;    //!< roster of rigouros equilibria
            const Roster<EList>         tolerant;    //!< roster of tolerant equilibria

            const ELists                order;       //!< different orders of combined primary equilibria
            Cluster *                   next;        //!< for list
            Cluster *                   prev;        //!< for list
            const unsigned              indx;        //!< index in clusters
            const size_t                maxOrder;    //!< max combination order
        };
    }

}


#endif

