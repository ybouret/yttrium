
#include "y/json/parser.hpp"
#include "y/jive/parser.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{

    namespace JSON
    {
        const char * const Parser::CallSign = "JSON";

        namespace
        {

            class Code : public Jive::Parser
            {
            public:

                inline explicit Code() : Jive::Parser(JSON::Parser::CallSign)
                {
                }

                inline virtual ~Code() noexcept
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

            static void * CodeImpl[ Y_WORDS_FOR(Code) ];
            static Code * code = 0;
        }

        Parser:: ~Parser() noexcept
        {
            assert(0!=code);
            (void) Memory::OutOfReach::Naught(code);
            code = 0;
        }

        Parser:: Parser() : Singleton<Parser>()
        {
            code = new (Y_STATIC_ZARR(CodeImpl)) Code();
        }
        
    }

}


