
//! \file

#ifndef Y_Chemical_Limits_Included
#define Y_Chemical_Limits_Included 1

#include "y/chemical/reactive/plexus/limit.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        typedef Small::CoopHeavyList<Limit> LimitsType;
        typedef LimitsType::NodeType        LimitsNode;
        typedef LimitsType::ProxyType       LimitsBank;


        //! list of limits
        class Limits : public LimitsType
        {
        public:
            explicit Limits(const LimitsBank &lbank,
                            const SBank      &sbank) noexcept;
            virtual ~Limits() noexcept;

            bool contains(const Species &s) const noexcept;

            //! the extent make species disappear
            void operator()(const Species &s,
                            const xreal_t  x);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Limits);
            const SBank repo;
        };

    }

}

#endif

