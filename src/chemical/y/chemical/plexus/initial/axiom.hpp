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

            class Axiom : public Quantized
            {
            protected:
                explicit Axiom(const xreal_t value) noexcept;
                explicit Axiom(const Axiom &)       noexcept;

            public:
                virtual ~Axiom() noexcept;

                virtual Axiom *clone()                   const          = 0;
                virtual int    weight(const Species &sp) const noexcept = 0;
                const xreal_t  amount;

                Axiom *next;
                Axiom *prev;

            private:
                Y_DISABLE_ASSIGN(Axiom);
            };

         
#define Y_Chemical_Axiom_Interface()  \
virtual Axiom * clone() const;        \
virtual int     weight(const Species &) const noexcept

        }
    }
}

#endif

