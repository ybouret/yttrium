//! \file


#ifndef Y_Chemical_Initial_Axiom_Included
#define Y_Chemical_Initial_Axiom_Included 1

#include "y/chemical/type/defs.hpp"
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            //__________________________________________________________________
            //
            //
            //
            //! Axiom to design initial concentrations
            //
            //
            //__________________________________________________________________
            class Axiom : public Quantized
            {
            protected:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Axiom(const xreal_t value) noexcept; //!< initialize with prescribed value
                explicit Axiom(const Axiom &)       noexcept; //!< duplicate

            public:
                virtual ~Axiom() noexcept;                    //!< cleanup


                //______________________________________________________________
                //
                //
                //  Interface
                //
                //______________________________________________________________
                virtual Axiom *clone()                   const          = 0; //!< clone
                virtual int    weight(const Species &sp) const noexcept = 0; //!< coefficient for given species

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                const xreal_t  amount; //!< prescribed value
                Axiom *        next;   //!< for list
                Axiom *        prev;   //!< for list

            private:
                Y_DISABLE_ASSIGN(Axiom);
            };

            //__________________________________________________________________
            //
            //! helper to declare Axiom Interface
            //__________________________________________________________________
#define Y_Chemical_Axiom_Interface()  \
virtual Axiom * clone() const;        \
virtual int     weight(const Species &) const noexcept

        }
    }
}

#endif

