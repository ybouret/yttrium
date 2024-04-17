
#include "y/chemical/lang/rosary.hpp"
#include "y/jive/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Rosary::CallSign = "Chemical::Rosary";

        class Rosary::Compiler : public Jive::Parser
        {
        public:
            explicit Compiler() : Jive::Parser(CallSign)
            {

                lexer.drop("[:blank:]");
                
            }

            inline virtual ~Compiler() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };


        Rosary:: Rosary()
        {
        }

        Rosary:: ~Rosary() noexcept
        {

        }

    }

}

