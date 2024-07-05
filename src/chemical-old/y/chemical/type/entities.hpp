
//! \file

#ifndef Y_Chemical_Entities_Included
#define Y_Chemical_Entities_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Base class to help display entities
        //
        //
        //______________________________________________________________________
        class Entities
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Entities()        noexcept; //!< setup
            virtual ~Entities()        noexcept; //!< cleanup
            Entities(const Entities &) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            //! update maxLengths
            void updateWith(const Entity &entity) noexcept;


            //! pad according to entity
            template <typename OSTREAM> inline
            OSTREAM & pad(OSTREAM &os, const Entity &entity) const
            {
                for(size_t i=entity.name.size();i<maxLength;++i) os << ' ';
                return os;
            }

            //! helper to present array of matching data per species
            template <typename LIST, typename ARRAY> inline
            void show(std::ostream & os,
                      const char *   pfx,
                      LIST &         list,
                      const char *   sfx,
                      ARRAY &        array,
                      const Level    level,
                      const unsigned indent = 0) const
            {
                static const char _[] = "";
                if(!pfx) pfx = _;
                if(!sfx) sfx = _;
                for(const typename LIST::NodeType *node=list.head;node;node=node->next)
                {
                    const Entity &entity = **node;
                    pad( Core::Indent(os,indent)
                        << pfx<< entity.name  << sfx, entity) << " = " << std::setw(15) << real_t(array[ entity.indx[level] ]) << '\n';
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxLength; //!< max name length
            
        private:
            Y_DISABLE_ASSIGN(Entities);

        public:
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

    }

}

#endif

