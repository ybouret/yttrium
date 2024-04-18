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
            static const char LeftRightArrow[]; //!< "<=>"

            explicit Components();
            Components(const Components &);
            virtual ~Components() noexcept;
            friend std::ostream & operator<<(std::ostream &, const Components &);

            void operator()(const int nu, const Species &sp);

            std::ostream & display(std::ostream &os, const size_t rmax=0, const size_t pmax=0) const;

            const Actors reac;
            const Actors prod;
            const String rstr;
            const String pstr;


        private:
            Y_DISABLE_ASSIGN(Components);
            Component::Set cdb;
            virtual ConstInterface & surrogate() const noexcept;
        };

    }

}

#endif
