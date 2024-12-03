
//! \file

#ifndef Y_Chemical_Grouping_Included
#define Y_Chemical_Grouping_Included 1


#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Grouping : public EList, public Fragment
        {
        public:
            static const char * const CallSign;
            explicit Grouping() noexcept;
            virtual ~Grouping() noexcept;

            void         collect(const Equilibrium &);
            virtual void update() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grouping);
        };

    }

}

#endif

