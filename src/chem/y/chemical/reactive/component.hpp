
//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chemical/reactive/actor.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        enum Acting
        {
            Reactant,
            Product
        };

        const char * ActingToText(const Acting) noexcept;

        class Component
        {
        public:
            Component(const Actor &, const Acting) noexcept;
            Component(const Component &)           noexcept;
            ~Component()                           noexcept;

            const Actor &actor;
            const Acting role;

            const String & key()  const noexcept;
            const char *   side() const noexcept;



        private:
            Y_DISABLE_ASSIGN(Component);
        };

    }

}

#endif

