//! file

#ifndef Y_Chemical_Equilibrium_Included
#define Y_Chemical_Equilibrium_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! An equilibrium is a set of components with a constant
        //
        //
        //______________________________________________________________________
        class Equilibrium : public Components, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            class Linker;
            typedef ArkPtr<const String,const Equilibrium> Handle;
            typedef SuffixSet<const String,const Handle>   Set;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup with name
            template <typename NAME> inline
            explicit Equilibrium(const NAME & _name) :
            Components(_name),
            Counted()
            {
            }

        public:
            //! cleanup
            virtual ~Equilibrium() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            xReal K(xReal t) const; //!< get positive constant at time t

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual xReal getK(xReal) const = 0;
        };

        //______________________________________________________________________
        //
        //
        //
        //! Failsafe constant equilibrium
        //
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:

            //! setup with name and constant constant
            template <typename NAME> inline
            explicit ConstEquilibrium(const NAME & _name, const xReal _value) :
            Equilibrium(_name),
            value(_value)
            {
            }

            //! cleanup
            virtual ~ConstEquilibrium() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const xReal value;
            virtual xReal getK(xReal) const;
        };

    }

}

#endif

