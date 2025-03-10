
//! \file


#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/chemical/formula.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Charge
        {
            protected: explicit Charge() noexcept;
            public:    virtual ~Charge() noexcept;

            const int z;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Charge);
        };

        class Species : public Quantized, public Formula, public Charge, public Entity, public Counted
        {
        public:
            struct Parser
            {
                static Formula        Make(const char * const);
                static Formula        Make(const String &);
                static const String * Brew(const Formula &, int &);
            };


            template <typename NAME> inline
            explicit Species(const NAME &description) :
            Formula( Parser:: Make(description) ),
            Charge(),
            Entity(  Parser:: Brew(*this, Coerce(z))  )
            {

            }

            
            virtual ~Species() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };

    }

}

#endif

