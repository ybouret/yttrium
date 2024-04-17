//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Species : public Entity
        {
        public:
            typedef ArkPtr<String,Species> Handle;

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
