
//! \file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibria/party.hpp"
#include "y/chemical/reactive/equilibrium/constant.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef SuffixSet<String,Equilibrium::Ptr> EqSet;

        class Equilibria : public Proxy<const EqSet>, public Party
        {
        public:
            typedef EqSet::ConstIterator ConstIterator;
            
            explicit Equilibria();
            virtual ~Equilibria() noexcept;
            Y_OSTREAM_PROTO(Equilibria);

            template <typename ID>
            Equilibrium & newConstant(const ID &name, const xreal_t K)
            {
                return append( new ConstantEquilibrium(name,db.size()+1,K) );
            }



        protected:
            EqSet db;

            Equilibrium &append(Equilibrium * const);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
        };

    }

}

#endif


