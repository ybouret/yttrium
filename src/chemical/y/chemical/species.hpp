//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity-set.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/solo.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/ptr/ark.hpp"
#include "y/random/bits.hpp"

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
            typedef ArkPtr<String,Species>              Handle;   //!< alias
            typedef SuffixSet<String,Handle>            Set;      //!< alias
            static  const int                           PMIN=-10; //!< Cmin = 10^(-PMIN)
            static  const int                           PMAX=  1; //!< Cmax = 10^(-PMAX)
            static xreal_t         Concentration(Random::Bits &); //!< 10^(rand(PMIN,PMAX))

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

            virtual ~Species() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Species);    //!< display

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

        //______________________________________________________________________
        //
        //
        //
        // global aliases
        //
        //
        //______________________________________________________________________

        typedef Small::BareLightList<const Species>  SList_;     //!< alias
        typedef SList_::NodeType                     SNode;     //!< alias
        typedef EntitySet<Species,SubLevel>          SpSubSet;  //!< alias
        typedef Small::CoopLightList<const Species>  SRepo;     //!< alias
        typedef SRepo::ProxyType                     SBank;     //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Species list
        //
        //
        //______________________________________________________________________
        class SList : public SList_
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SList() noexcept; //!< setup
            virtual ~SList() noexcept; //!< cleanup
            SList(const SList &);      //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! transfer for each species
            void transfer(XWritable       &target,
                          const Level      tgtlvl,
                          const XReadable &source,
                          const Level     &srclvl) const;
            
        private:
            Y_DISABLE_ASSIGN(SList);
        };

#if 0
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
            template <typename TLIST, const Level LEVEL> static inline
            void By(TLIST &list) noexcept
            {
                MergeSort::Call(list,CompareNodes<typename TLIST::NodeType,LEVEL>);
            }


            //! sort by increasing top level
            template <typename TLIST> static inline
            void ByTopLevel(TLIST &list) noexcept
            {
                By<TLIST,TopLevel>(list);
            }

            //! sort by increasing top level
            template <typename TLIST> static inline
            void BySubLevel(TLIST &list) noexcept
            {
                By<TLIST,SubLevel>(list);
            }


            //! sort by increasing TopLevel and deduce SubLevel
            template <typename TLIST, const Level LEVEL> static inline
            void MakeLevel(TLIST &list) noexcept
            {
                By<TLIST,TopLevel>(list);
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
#endif

    }
}

#endif
