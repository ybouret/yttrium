
//! file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Reactor : public Equilibrium::Set, public Assembly
        {
        public:
            explicit Reactor();
            virtual ~Reactor() noexcept;

            void mustInsert(const Equilibrium::Handle &);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };

        

        class Equilibria : public Proxy<const Reactor>
        {
        public:
            static const char * const CallSign; //!< "Chemical::Equilibria"

            explicit Equilibria();
            virtual ~Equilibria() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
            Reactor reactor;

        };


    }

}


#endif

