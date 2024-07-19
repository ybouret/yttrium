//! \file

#ifndef Y_Chemical_Eqs_Deps_Included
#define Y_Chemical_Eqs_Deps_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/ranked.hpp"
#include "y/data/small/light/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::CoopLightList<const Equilibrium> ERepo;
        typedef ERepo::ProxyType                        EBank;


        class LinearlyIndependent : public Proxy<const ERepo>
        {
        public:
            static const char * const CallSign;

            typedef Small::Ranked<ERepo,Equilibrium::Comparator> ERank;

            explicit LinearlyIndependent(const size_t capacity);
            virtual ~LinearlyIndependent() noexcept;

            void init() noexcept;
            bool keep(const Equilibrium &eq);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LinearlyIndependent);
            virtual ConstInterface & surrogate() const noexcept;
            EBank bank;
            ERank rank;
            ERepo list;

        };

    }

}


#endif
