

//! \file


#ifndef Y_Chemical_Entity_Included
#define Y_Chemical_Entity_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Named entity interface
        //
        //
        //______________________________________________________________________
        class Entity : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from name
            template <typename NAME>
            explicit inline Entity(const NAME &uid) :
            Object(), Counted(), name(uid) {}

            //! copy
            Entity(const Entity &);

            //! cleanup
            virtual ~Entity() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String &key() const noexcept; //!< key for databases

            //__________________________________________________________________
            //
            //
            // Member
            //
            //__________________________________________________________________s
            const String  name; //!< identifier

        private:
            Y_DISABLE_ASSIGN(Entity);
        };
    }
}

#endif

