//! \file

#ifndef Y_Chemical_Eqs_Deps_Included
#define Y_Chemical_Eqs_Deps_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/orthogonal/family.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::CoopLightList<const Equilibrium> ERepo;
        typedef ERepo::ProxyType                        EBank;


        class LinearlyIndependent : public Quantized, public Proxy<const ERepo>
        {
        public:
            static const char * const CallSign;


            explicit LinearlyIndependent(const size_t primary,
                                         const size_t species);
            virtual ~LinearlyIndependent() noexcept;

            const size_t & key() const noexcept; //!< number of species

            void init() noexcept;
            bool keep(const Equilibrium &eq);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LinearlyIndependent);
            virtual ConstInterface & surrogate() const noexcept;
            EBank              bank;
            ERepo              list;
            Orthogonal::Family qfam;
        };

    }

}


#endif
