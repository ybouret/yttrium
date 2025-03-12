
//! \file


#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1


#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Fragment
        {
        public:


        private:

        };

        typedef SuffixSet<const String,Equilibrium::Pointer> EquilibriaSet;

        //______________________________________________________________________
        //
        //
        //
        //! Set of equilibria
        //
        //
        //______________________________________________________________________
        class Equilibria : public Proxy<const EquilibriaSet>, public Latchable
        {
        public:
            explicit Equilibria();
            virtual ~Equilibria() noexcept;


            size_t nextIndex() const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            Y_PROXY_DECL();
            EquilibriaSet db;
        };


    }

}

#endif

