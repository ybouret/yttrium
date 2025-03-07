#include "y/chemical/weasel.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Weasel:: CallSign = "Weasel";
        const char * const Weasel:: Coef     = "Coef";

    }
}


#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/weasel/formula/to-text.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        class Weasel:: Code
        {
        public:
            inline Code() :
            parser(),
            formulaToText()
            {
            }

            inline ~Code() noexcept {}

            Parser        parser;
            FormulaToText formulaToText;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        static void *         Code__[ Y_WORDS_FOR(Weasel::Code) ];
        static Weasel::Code * code = 0;
        static inline void    zCode() noexcept {  code=0; memset(Code__,0,sizeof(Code__)); }


        Weasel:: Weasel()
        {
            std::cerr << "sizeof(Code)=" << sizeof(Code__) << std::endl;
            zCode();
            try { code = new ( Code__ ) Code(); }
            catch(...) {  zCode(); throw; }
        }


        Weasel:: ~Weasel() noexcept
        {
            assert(0!=code);
            Destruct(code);
            zCode();
        }


        XNode * Weasel:: parse(Lingo::Module * const inputModule)
        {
            assert(0!=code);
            return code->parser.preprocess(inputModule);
        }

    }

}

