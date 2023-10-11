
#include "y/chem/weasel.hpp"
#include "y/chem/lang/parser.hpp"
#include "y/chem/lang/linker.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Weasel:: CallSign = "Weasel";
        
        Weasel:: ~Weasel() noexcept
        {
        }


        class Weasel:: Code : public Parser, public Linker
        {
        public:
            inline explicit Code()
            {
            }

            inline virtual ~Code() noexcept
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        static void *Code__[ Y_WORDS_FOR(Weasel::Code) ];
        static void *Code_ = 0;

        Weasel:: Weasel()
        {
            std::cerr << "sizeof(Code)   = " << sizeof(Code) << std::endl;
            std::cerr << "sizeof(Code__) = " << sizeof(Code_) << std::endl;
            try
            {
            }
            catch(...)
            {
                Code_ = 0;
                throw;
            }
        }

        void Weasel:: operator()(Jive::Module *m, Library &lib, LuaEquilibria &eqs)
        {
        }

    
    }

}
