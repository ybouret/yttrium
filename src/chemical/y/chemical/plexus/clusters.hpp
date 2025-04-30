

//! \file


#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Vector<xreal_t,MemoryModel> XVector; //!< alias

        class Listing
        {
        public:
            explicit Listing() noexcept : neutral(), charged() {}
            virtual ~Listing() noexcept {}

            void   sort()
            {
                MergeSort::Call( Coerce(neutral), MetaList<SList>::Compare);
                MergeSort::Call( Coerce(charged), MetaList<SList>::Compare);
            }
            size_t size() const noexcept { return neutral.size+charged.size; }

            const SList neutral;
            const SList charged;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Listing);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Independent Clusters of dependent species
        //
        //
        //______________________________________________________________________
        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Chemical::Clusters

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! full setup
            /**
             \param xml for verbosity
             \param eqs primary equilibria
             \param lib latched library holding all species
             \param t0  time to evaluate constants
             */
            explicit Clusters(XMLog &         xml,
                              Equilibria &    eqs,
                              const Library & lib,
                              const xreal_t   t0);

            //! cleanup
            virtual ~Clusters() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! evaluate K(t0)
            const XReadable &operator()(const xreal_t);

            //! output all orders in basename[1..maxOrder].png
            void graphViz(const String &baseName) const;

            //! set zero in TopLevel data for species
            template <typename DATA> inline
            void zset(DATA &data) const {
                ZeroList(species,data,TopLevel);
            }

            //! find unique law preseserving matching expression
            const Conservation::Law *preserving(Lingo::Matching &) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            Y_PROXY_DECL();
            Cluster::List   my;
            XVector         tlK;

        public:
            const XReadable &K;         //!< last computed constant
            const size_t     primary;   //!< primary equilibria
            const Listing    conserved; //!< CONSERVED, REACTIVE species
            const Listing    unbounded; //!< UNBOUNDED, REACTIVE species
            const SList      species;   //!< all REACTIVE species
            const Listing    witness;   //!< all SPECTATOR species
            const size_t     maxOrder;  //!< max order size thru clusters
        };

    }

}

#endif

