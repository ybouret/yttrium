//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/reactive/component.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Components : public Proxy<const Component::Set>
        {
        public:
            
            explicit Components();
            Components(const Components &);
            virtual ~Components() noexcept;

            void operator()(const int nu, const Species &sp);

            const Actors reac;
            const Actors prod;

        private:
            Y_DISABLE_ASSIGN(Components);
            Component::Set cdb;
            virtual ConstInterface & surrogate() const noexcept;
        };

    }

}

#endif
