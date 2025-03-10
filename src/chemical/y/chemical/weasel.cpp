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

        const String * Weasel:: formulaToText(const Formula &f, int &z)
        {
            assert(0!=code);
            return code->formulaToText.get(*f,z);
        }

    }

}

#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Formula Weasel:: parseFormula1(Lingo::Module * const inputModule)
        {
            static const char fn[] = "parseFormula1";
            AutoPtr<XNode> node = parse(inputModule); assert(node->is(Weasel::CallSign));
            XList &        list = node->branch();

            if(1!=list.size)                       throw Specific::Exception(CallSign,"%s(forbidden multiple entries)",fn);
            if(!list.head->is(Formula::CallSign))  throw Specific::Exception(CallSign,"%s(forbidden '%s'')",fn,list.head->name().c_str());
            return Formula(list.popHead());
        }

        Formula Species:: Parser:: Make(const char * const id)
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.parseFormula1( Lingo::Module::OpenData(id,id) );
        }

        Formula Species:: Parser:: Make(const String & id)
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.parseFormula1( Lingo::Module::OpenData(id,id) );
        }

        const String * Species:: Parser:: Brew(const Formula &f, int &z)
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.formulaToText(f,z);
        }



    }

}


