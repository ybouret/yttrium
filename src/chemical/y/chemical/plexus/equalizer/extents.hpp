//! \file

#ifndef Y_Chemical_Extents_Included
#define Y_Chemical_Extents_Included 1

#include "y/chemical/plexus/equalizer/extent.hpp"
#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Extents : public Restartable
        {
        public:
            explicit Extents(const EqzBanks &banks) noexcept;
            virtual ~Extents() noexcept;


            virtual void restart() noexcept;

            void operator()(const Components &  E,
                            const XReadable &   C,
                            const Level         L,
                            const AddressBook * const wanders);

            Extent reac;
            Extent prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Extents);
        };

    
    }

}

#endif

