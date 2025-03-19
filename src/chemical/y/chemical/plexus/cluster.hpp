
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


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! transfer according to species using '=' semantics
            template <typename TARGET, typename SOURCE> inline
            TARGET & transfer(TARGET &target, const Level targetLevel,
                              SOURCE &source, const Level sourceLevel) const
            {
                for(const SNode *sn=(*this)->species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    sp(target,targetLevel) = sp(source,sourceLevel);
                }
                return target;
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





            Cluster *      next; //!< for list
            Cluster *      prev; //!< for list
            const unsigned uuid; //!< indexing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            
        };

    }

}

#endif

