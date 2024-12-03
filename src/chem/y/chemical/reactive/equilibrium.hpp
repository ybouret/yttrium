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
            typedef ArkPtr<const String,const Equilibrium> Handle; //!< alias
            typedef SuffixSet<const String,const Handle>   Set;    //!< lais

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup with name
            template <typename NAME> inline
            explicit Equilibrium(const NAME & _name,
                                 const size_t _indx) :
            Components(_name,_indx),
            Counted()
            {
            }

        public:

            virtual ~Equilibrium() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Equilibrium);    //!< display name

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
            explicit ConstEquilibrium(const NAME & _name,
                                      const size_t _indx,
                                      const xReal  _value) :
            Equilibrium(_name,_indx),
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


        typedef Small::BareLightList<const Equilibrium> EList;
        typedef EList::NodeType                         ENode;

    }

}

#endif

