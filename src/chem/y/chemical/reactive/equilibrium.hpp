//! file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Equilibrium : public Components, public Counted
        {
        public:
            class Linker;

        protected:
            template <typename NAME> inline
            explicit Equilibrium(const NAME & _name) :
            Components(_name),
            Counted()
            {
            }

        public:
            virtual ~Equilibrium() noexcept;


            xReal K(xReal t) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xReal getK(xReal) const = 0;
        };

        class ConstEquilibrium : public Equilibrium
        {
        public:
            template <typename NAME> inline
            explicit ConstEquilibrium(const NAME & _name, const xReal _value) :
            Equilibrium(_name),
            value(_value)
            {
            }

            virtual ~ConstEquilibrium() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const xReal value;
            virtual xReal getK(xReal) const;
        };

    }

}

#endif

