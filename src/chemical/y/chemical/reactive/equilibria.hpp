
//! \file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibria/party.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<String,Equilibrium::Ptr> EqSet;

        class Equilibria : public Proxy<const EqSet>
        {
        public:
            typedef EqSet::ConstIterator ConstIterator;
            
            explicit Equilibria();
            virtual ~Equilibria() noexcept;




        protected:
            EqSet db;

        public:
            Party party;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
        };

    }

}

#endif


