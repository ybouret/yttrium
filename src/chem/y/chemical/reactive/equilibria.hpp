
//! file

#ifndef Y_Chemical_Equilibria_Included
#define Y_Chemical_Equilibria_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Reactor : public Equilibrium::Set, public Fragment
        {
        public:
            explicit Reactor();
            virtual ~Reactor() noexcept;

            // Interface
            virtual void updateFragment() noexcept; //!< update with set

            // Methods
            void         mustInsert(const Equilibrium::Handle &);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };



        class Equilibria : public Proxy<const Reactor>
        {
        public:
            static const char * const      CallSign;      //!< "Chemical::Equilibria"
            typedef Reactor::ConstIterator ConstIterator; //!< alias

            explicit Equilibria();
            virtual ~Equilibria() noexcept;
            Y_OSTREAM_PROTO(Equilibria);

            void decl( Equilibrium * const );


            template <typename EQTYPE> inline
            EQTYPE & operator()(EQTYPE * const eq)
            {
                decl(eq);
                return *eq;
            }

            void update() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual ConstInterface & surrogate() const noexcept;
            Reactor reactor;

        };


    }

}


#endif

