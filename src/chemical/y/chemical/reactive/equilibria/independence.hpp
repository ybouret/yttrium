//! \file

#ifndef Y_Chemical_Eqs_Dependy_Included
#define Y_Chemical_Eqs_Dependy_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Independence
        {
        public:
            explicit Independence(const size_t capacity);
            virtual ~Independence() noexcept;

            void start(const Equilibrium &eq);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Independence);
            EBank bank;
            ERepo repo;

            bool grow(const Equilibrium &eq);

        };

    }

}


#endif
