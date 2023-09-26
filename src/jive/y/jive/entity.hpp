//! \file
#ifndef Y_Jive_Entity_Included
#define Y_Jive_Entity_Included 1


#include "y/jive/tag.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Entity to share names
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
            
            //! setup
            template <typename LABEL> inline
            explicit Entity(const LABEL &id) :
            Object(),
            Counted(),
            name(id)
            {}

            Entity(const Entity &) noexcept; //!< copy
            virtual ~Entity()      noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const String & key() const noexcept; //!< generic key for associative

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Tag name; //!< identifier

        private:
            Y_DISABLE_ASSIGN(Entity);
        };

    }

}

#endif

