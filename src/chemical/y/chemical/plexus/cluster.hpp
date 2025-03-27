
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/plexus/cluster/combinatorics.hpp"
#include "y/chemical/plexus/conservation/canons.hpp"
#include "y/sequence/vector.hpp"

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
            static const char * const                CallSign; //!< "Cluster"
            typedef CxxListOf<Cluster>               List;     //!< alias
            typedef Vector<const String,MemoryModel> Strings;  //!< alias

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



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! output GrapViz code for given order
            void viz(OutputStream &fp, const size_t numOrder) const;

            //! transfer according to species using '=' semantics
            template <typename TARGET, typename SOURCE> inline
            TARGET & transfer(TARGET &target, const Level targetLevel,
                              SOURCE &source, const Level sourceLevel) const
            {
                return TransferList(*((*this)->species),target,targetLevel,source,sourceLevel);
            }


            //! transfer(target,SubLevel,source,TopLevel)
            template <typename TARGET, typename SOURCE> inline
            TARGET & gather( TARGET &target, SOURCE &source) const
            {
                return transfer(target,SubLevel,source,TopLevel);
            }

            //! transfer(target,TopLevel,source,SubLevel)
            template <typename TARGET, typename SOURCE> inline
            TARGET & expand( TARGET &target, SOURCE &source) const
            {
                return transfer(target,TopLevel,source,SubLevel);
            }

            //! display associated array and level
            template <typename ARRAY> inline
            std::ostream & show(std::ostream &os,
                                const Level  lvl,
                                const char  *pfx,
                                ARRAY       &arr,
                                const char  *sfx) const
            {
                const ClusterContent &self = **this;
                return ShowList(os,*self.species,self.sformat,lvl,pfx,arr,sfx);
            }

            //! display associated proc(array) and level
            template <typename ARRAY, typename PROC> inline
            std::ostream & show(std::ostream &os,
                                const Level  lvl,
                                const char  *pfx,
                                ARRAY       &arr,
                                const char  *sfx,
                                PROC        &proc) const
            {
                const ClusterContent &self = **this;
                return ShowList(os,*self.species,self.sformat,lvl,pfx,arr,sfx,proc);
            }


            const Conservation::Canons   canons;  //!< independent canons of dep. laws
            Cluster *                    next;    //!< for list
            Cluster *                    prev;    //!< for list
            const unsigned               uuid;    //!< indexing
            const Strings                spColor; //!< colors for species
            const Strings                eqColor; //!< colors for equilibria
            const Strings                cnColor; //!< colros for laws

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            
        };

    }

}

#endif

