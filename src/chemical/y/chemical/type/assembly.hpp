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

