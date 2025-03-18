//! \file


#ifndef Y_Chemical_Assembly_Included
#define Y_Chemical_Assembly_Included 1

#include "y/chemical/type/indexed.hpp"
#include "y/text/justify.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Utilities for assembly of (indexed) entities
        //
        //
        //______________________________________________________________________
        class Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Assembly() noexcept;                    //!< setup
            virtual ~Assembly() noexcept;                    //!< cleanup
            Assembly(const Assembly &) noexcept;             //!< duplicate
            Assembly & operator=(const Assembly &) noexcept; //!< assign

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! update maxNameLength
            void enroll(const Entity &) noexcept;

            //! pad stream with entity.name->size() to maxNameLength
            std::ostream &pad(std::ostream &os, const Entity &entity) const
            {
                for(size_t i=entity.name->size();i<maxNameLength;++i) os << ' ';
                return os;
            }

            template <typename LIST, typename ARRAY> inline
            void show(std::ostream &os,
                      const char *  pfx,
                      const LIST   &list,
                      const char *  sfx,
                      const Level   level,
                      ARRAY        &array)
            {

                for(const typename LIST::NodeType *node=list.head;node;node=node->next)
                {
                    if(pfx) os << pfx;
                    os << **node;
                    os << sfx;
                    pad(os,**node);
                    os << " = ";
                    os << (**node)(array,level);
                    os << std::endl;
                }
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxNameLength; //!< max name length of entities


        };
    }

}

#endif

