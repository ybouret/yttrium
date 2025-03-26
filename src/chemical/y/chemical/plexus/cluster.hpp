
//! \file


#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/plexus/cluster/combinatorics.hpp"
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
            static const char * const  CallSign; //!< "Cluster"
            typedef CxxListOf<Cluster> List;     //!< alias
            typedef Vector<const String,MemoryModel> Strings;

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

            void viz(OutputStream &fp,
                     const size_t   numOrder) const;

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

            //! display associated array and level
            template <typename ARRAY> inline
            std::ostream & show(std::ostream &os,
                                const Level  lvl,
                                const char  *pfx,
                                ARRAY       &arr,
                                const char  *sfx) const
            {
                const ClusterContent &self = **this;
                os << "{" << std::endl;
                for(const SNode *it=self.species->head;it;it=it->next)
                {
                    if(pfx) os << pfx;
                    const Species &sp = **it;
                    os << sp.name;
                    if(sfx) os << sfx;
                    self.sformat.pad(os,sp) << " = " << sp(arr,lvl);
                    os << std::endl;
                }
                os << "}";
                return os;
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
                os << "{" << std::endl;
                for(const SNode *it=self.species->head;it;it=it->next)
                {
                    if(pfx) os << pfx;
                    const Species &sp = **it;
                    os << sp.name;
                    if(sfx) os << sfx;
                    self.sformat.pad(os,sp) << " = " << proc(sp(arr,lvl));
                    os << std::endl;
                }
                os << "}";
                return os;
            }



            Cluster *      next; //!< for list
            Cluster *      prev; //!< for list
            const unsigned uuid; //!< indexing
            const Strings  spColor;
            const Strings  eqColor;
            const Strings  cnColor;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            
        };

    }

}

#endif

