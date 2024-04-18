
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
            explicit Entities() noexcept; //!< setup
            virtual ~Entities() noexcept; //!< cleanup

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

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t maxLength; //!< max name length
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entities);
        };

    }

}

#endif

