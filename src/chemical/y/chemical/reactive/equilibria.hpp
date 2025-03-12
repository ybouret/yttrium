
//! \file


#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1


#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        typedef SuffixSet<const String,Equilibrium::Pointer> EquilibriaSet;

        class EquilibriaType : public EquilibriaSet, public Fragment
        {
        public:
            explicit EquilibriaType();
            virtual ~EquilibriaType() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EquilibriaType);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Set of equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria : public Proxy<const EquilibriaType>, public Latchable
        {
        public:
            explicit Equilibria();
            virtual ~Equilibria() noexcept;


            size_t nextIndex() const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            Y_PROXY_DECL();
            EquilibriaType db;
        };


    }

}

#endif

