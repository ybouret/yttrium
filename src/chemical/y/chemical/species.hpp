//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/ptr/ark.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/data/small/light/list/solo.hpp"
#include "y/data/small/light/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Species : public Entity
        {
        public:
            typedef ArkPtr<String,Species> Handle;
            typedef Small::BareLightList<const Species> BareList; //!< once usage
            typedef Small::SoloLightList<const Species> SoloList; //!< standalone, multiple usage
            typedef Small::CoopLightList<const Species> CoopList; //!< shared, multiple usage
            typedef CoopList::ProxyType                 CoopRepo; //!< memory for CoopList


            template <typename NAME> inline
            explicit Species(const NAME  &uid,
                             const int    charge,
                             const size_t topLevel) :
            Entity(uid,topLevel), z(charge)
            {
            }

            virtual ~Species() noexcept;
            
            void viz(OutputStream &) const;

            const int z;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };

    }
}

#endif
