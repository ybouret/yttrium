//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chem/species.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Component
        {
        public:
            typedef SuffixSet<String,Component> DataBase;
            
            Component(const Species &which,
                      const int      coeff) noexcept;
            ~Component()                    noexcept;
            Component(const Component &)    noexcept;

            const String &key() const noexcept;

            const Species &sp;
            const int      nu;

        private:
            Y_DISABLE_ASSIGN(Component);
        };

    }

}

#endif
