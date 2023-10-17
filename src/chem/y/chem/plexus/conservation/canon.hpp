//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chem/plexus/conservation.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        // List of species
        //
        //______________________________________________________________________
        typedef Small::BareLightList<const Species>     SpRepo; //!< alias
        typedef SpRepo::NodeType                        SpNode; //!< alias

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



        //______________________________________________________________________
        //
        //
        //! light list of DEPENDENT conservations
        //
        //______________________________________________________________________
        typedef Small::BareLightList<const Conservation> Conservations;
        
        //______________________________________________________________________
        //
        //
        //
        //! list of dependent conservations
        //
        //
        //______________________________________________________________________
        class Canon : public Object, public Conservations
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Canon(const Conservation &first); //!< setup with first conservation
            virtual ~Canon() noexcept;                 //!< cleanup

            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool accepts(const Conservation &) const noexcept; //!< true if sharing a species
            bool accepts(const Canon &)        const noexcept; //!< true if sharing a species
            void compile();                                    //!< make repo

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SpRepo repo; //!< involved species
            Canon *      next; //!< for list
            Canon *      prev; //!< for list
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Canon);
        };

    }

}

#endif

