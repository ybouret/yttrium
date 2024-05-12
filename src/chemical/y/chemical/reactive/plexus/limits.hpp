
//! \file

#ifndef Y_Chemical_Limits_Included
#define Y_Chemical_Limits_Included 1

#include "y/chemical/reactive/plexus/boundaries.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Limits
        {
        public:
            static const unsigned USE_NONE = 0x00;
            static const unsigned USE_REAC = 0x01;
            static const unsigned USE_PROD = 0x02;
            static const unsigned USE_BOTH = USE_REAC | USE_PROD;

            explicit Limits(const BBank &, const SBank &) noexcept;
            Limits(const Limits &other);
            ~Limits() noexcept;
            Y_OSTREAM_PROTO(Limits);

            void     reset()       noexcept;
            unsigned state() const noexcept;

            Boundaries reac;
            Boundaries prod;
        private:
            Y_DISABLE_ASSIGN(Limits);
            void outReac(std::ostream &os) const;
            void outProd(std::ostream &os) const;

        };

    }

}

#endif
