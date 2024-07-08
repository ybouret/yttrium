
//! \file

#ifndef Y_Chemical_Equilibria_Party_Included
#define Y_Chemical_Equilibria_Party_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Party
        {
        public:
            explicit Party() noexcept;
            virtual ~Party() noexcept;

            void           update(const Components &) noexcept;
            std::ostream & display(std::ostream &os, const Components &) const;


            const Assembly uuid;
            const Assembly reac;
            const Assembly prod;

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Party);
        };
    }

}

#endif

