
//! \file

#ifndef Y_Chemical_Entities_Included
#define Y_Chemical_Entities_Included 1

#include "y/chemical/type/entity.hpp"

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
            template <typename OSTREAM, typename LIST, typename ARRAY> inline
            void show(OSTREAM &os, const char *pfx, LIST &list, const char *sfx, ARRAY &array, const Level level) const
            {
                static const char _[] = "";
                if(!pfx) pfx = _;
                if(!sfx) sfx = _;
                for(const typename LIST::NodeType *node=list.head;node;node=node->next)
                {
                    const Entity &entity = **node;
                    pad(os << pfx<< entity.name  << sfx, entity) << " = " << array[ entity.indx[level] ] << '\n';
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
        };

    }

}

#endif

