
//! \file


#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/chemical/type/xcode.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Base class for named entities
        //
        //
        //______________________________________________________________________
        class Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Entity(const String *_) noexcept; //!< setup
            explicit Entity(const XName & _) noexcept; //!< setup
            virtual ~Entity()                noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept; //!< *name*



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const XName name; //!< shared name

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Entity);
        };
    }
}

#endif
