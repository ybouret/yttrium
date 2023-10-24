//! \file


#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chem/type/entity.hpp"
#include "y/ptr/ark.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/solo.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! Base class for species
        //
        //
        //______________________________________________________________________
        class Species : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<String,Species> Pointer; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from name + algebraic charge
            template <typename NAME> inline
            explicit Species(const NAME &uid,
                             const int   algebraicCharge) :
            Entity(uid), z(algebraicCharge) {}
            virtual ~Species() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Species);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String toString() const; //!< "[name]"
            void   viz(OutputStream &) const; //!< name

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const int z; //!< algebraic charge
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };



        //______________________________________________________________________
        //
        //
        // List of species
        //
        //______________________________________________________________________
        typedef Small::BareLightList<const Species>     SpRepo;  //!< alias
        typedef Small::SoloLightList<const Species>     SpList;  //!< alias
        typedef SpRepo::NodeType                        SpNode;  //!< alias
        typedef Small::CoopLightList<const Species>     SpStrip; //!< alias
        typedef SpStrip::CacheType                      SpCache; //!< alias
        typedef SpStrip::ProxyType                      SpProxy; //!< alias

        //! compare Entity-based node by their top-level indices
        template <typename NODE> static inline
        SignType CompareSmallNodes(const NODE *lhs, const NODE *rhs)
        {
            const Entity &L = **lhs;
            const Entity &R = **rhs;
            return Sign::Of(L.indx[TopLevel], R.indx[TopLevel]);
        }

        //! sort any repository of Entity-based nodes
        template <typename LIST> static inline
        void SortIncreasing(LIST &repo)
        {
            MergeSort::Call(repo, CompareSmallNodes<typename LIST::NodeType> );
        }

    }
}

#endif

