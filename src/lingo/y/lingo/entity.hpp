//! \file

#ifndef Y_Lingo_Entity_Included
#define Y_Lingo_Entity_Included 1

#include "y/lingo/caption.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Entity to share names
        //
        //
        //______________________________________________________________________
        class Entity : public Counted
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
            explicit Entity(const LABEL &id, const AsCaption_ &) :
            Counted(),
            name(id)
            {}

            Entity(const Entity &) noexcept; //!< copy
            virtual ~Entity()      noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Entity);         //!< display name
            
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
            const Caption name; //!< identifier

        private:
            Y_DISABLE_ASSIGN(Entity);
        };

    }

}

#endif

