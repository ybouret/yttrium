//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity-set.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/solo.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/ptr/ark.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Species = Entity + charge
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
            //typedef Small::BareLightList<const Species> BareList; //!< once usage
            //typedef Small::SoloLightList<const Species> SoloList; //!< standalone, multiple usage
            //typedef Small::CoopLightList<const Species> CoopList; //!< shared, multiple usage
            //typedef CoopList::ProxyType                 CoopRepo; //!< memory for CoopList

            typedef ArkPtr<String,Species>              Handle;   //!< alias
            typedef SuffixSet<String,Handle>            Set;      //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            template <typename NAME> inline
            explicit Species(const NAME  &uid,
                             const int    charge,
                             const size_t topLevel) :
            Entity(uid,topLevel), z(charge)
            {
            }

            //! cleanup
            virtual ~Species() noexcept;

            //! ddisplay
            friend std::ostream & operator<<(std::ostream &, const Species &);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void viz(OutputStream &) const; //!< write GraphViz code

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

        typedef Small::BareLightList<const Species> SList;     //!< alias
        typedef SList::NodeType                     SNode;     //!< alias
        typedef EntitySet<Species,SubLevel>         SpSubSet;  //!< alias

        //______________________________________________________________________
        //
        //
        //! helper to sort Light Lists
        //
        //______________________________________________________________________
        struct LightSort
        {

            //! comparison by TopLevel index
            template <typename TNODE, const Level LEVEL>
            static inline SignType CompareNodes(const TNODE *lhs, const TNODE *rhs) noexcept
            {
                return Sign::Of( (**lhs).indx[LEVEL], (**rhs).indx[LEVEL] );
            }



            //! sort by increasing top level
            template <typename TLIST> static inline
            void ByTopLevel(TLIST &list) noexcept
            {
                MergeSort::Call(list,CompareNodes<typename TLIST::NodeType,TopLevel>);
            }

            //! sort by increasing top level
            template <typename TLIST> static inline
            void BySubLevel(TLIST &list) noexcept
            {
                MergeSort::Call(list,CompareNodes<typename TLIST::NodeType,SubLevel>);
            }
            
            //! sort by increasing TopLevel and deduce SubLevel
            template <typename TLIST, const Level LEVEL> static inline
            void MakeLevel(TLIST &list) noexcept
            {
                ByTopLevel(list);
                size_t indx = 0;
                for(typename TLIST::NodeType *node=list.head;node;node=node->next)
                {
                    Coerce((**node).indx[LEVEL]) = ++indx;
                }
            }

            //! create sub-indices
            template <typename TLIST> static inline
            void MakeSubLevel(TLIST &list) noexcept
            {
                MakeLevel<TLIST,SubLevel>(list);
            }

            //! create aux-indices
            template <typename TLIST> static inline
            void MakeAuxLevel(TLIST &list) noexcept
            {
                MakeLevel<TLIST,AuxLevel>(list);
            }

        };

    }
}

#endif
