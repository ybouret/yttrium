
//! \file

#ifndef Y_Lingo_Lexical_Action_Included
#define Y_Lingo_Lexical_Action_Included 1

#include "y/lingo/lexical/unit.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Action_ : public Object, public Counted
            {
            protected:
                explicit Action_(const uint32_t, const bool) noexcept;

            public:
                virtual ~Action_() noexcept;
                
                const uint32_t uuid; //!< class uuid
                const bool     endl; //!< propagate endl

                template <typename T> inline
                const T * as() const noexcept
                {
                    assert( T::UUID == uuid );
                    assert( 0 != self);
                    return static_cast<const T *>(self);
                }

            protected:
                void * const self;
                
            private:
                Y_DISABLE_ASSIGN(Action_);
            };


            typedef ArcPtr<const Action_> Action;

#define Y_Lingo_Action(TYPE) Coerce(self) = static_cast<TYPE *>(this)

        }
    }

}

#endif

