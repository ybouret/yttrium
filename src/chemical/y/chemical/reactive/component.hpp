//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chemical/species.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Component
        {
        public:
            typedef SuffixSet<String,Component> Set;

            Component(const int coef, const Species &spec) noexcept;
            Component(const Component &) noexcept;
            ~Component() noexcept;

            const String &key() const noexcept;


            const int      nu;
            const Species &sp;
        private:
            Y_DISABLE_ASSIGN(Component);
        };

    }

}

#endif
