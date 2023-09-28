
//! \file

#ifndef Y_Chemical_Components_Included
#define Y_Chemical_Components_Included 1

#include "y/chem/component.hpp"
#include "y/chem/actors.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Components : public Proxy<const Component::DataBase>
        {
        public:
            static const char * const CallSign;
            
            explicit Components();
            virtual ~Components() noexcept;

            void operator()(const int nu, const Species &sp);

            String toString() const;
            
            const Actors              reac;
            const Actors              prod;
        private:
            Y_DISABLE_ASSIGN(Components);
            Component::DataBase db;
            virtual ConstType & surrogate() const noexcept;

        };

    }

}

#endif


