
//! \file

#ifndef Y_Chemical_Component_Included
#define Y_Chemical_Component_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Component : public Object, public Counted
        {
        public:
            typedef ArkPtr<String,const Component> Ptr;
            
            explicit Component(const int n, const Species &s);
            virtual ~Component() noexcept;

            const String & key() const noexcept;

            const int      nu;
            const Species &sp;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Component);
        };

    }

}

#endif
