#include "y/chemical/weasel.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Weasel:: CallSign = "Weasel";
        const char * const Weasel:: Coef     = "Coef";
        const char * const Weasel:: RegExp   = "RegExp";
        const char * const Weasel:: StringID = "String";

    }
}


#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/weasel/formula/to-text.hpp"
#include "y/lua++/state.hpp"
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
            formulaToText(),
            lvm( new Lua::State() )
            {
            }

            inline ~Code() noexcept {}

            Parser        parser;
            FormulaToText formulaToText;
            Lua::VM       lvm;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        static void *         Code__[ Y_WORDS_FOR(Weasel::Code) ];

        static inline
        Weasel::Code * initCode()
        {
            return new ( memset(Code__,0,sizeof(Code__)) ) Weasel::Code();
        }


        Weasel:: Weasel() :
        code( initCode() ) 
        {

        }


        Weasel:: ~Weasel() noexcept
        {
            assert(0!=code);
            Destruct(code);
            memset(Code__, 0, sizeof(Code__) );
            Coerce(code)=0;
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
            static const char func[] = "parseFormula1";
            AutoPtr<XNode>    node = parse(inputModule); assert(node->is(Weasel::CallSign));
            XList &           list = node->branch();

            if(1!=list.size)                       throw Specific::Exception(CallSign,"%s(forbidden multiple entries)",func);
            if(!list.head->is(Formula::CallSign))  throw Specific::Exception(CallSign,"%s(forbidden '%s'')",func,list.head->name().c_str());
            return Formula(list.popHead());
        }

        XNode * Weasel:: singleFormula(Lingo::Module * const inputModule)
        {
            static const char func[] = "singleFormula";
            AutoPtr<XNode>    node = parse(inputModule); assert(node->is(Weasel::CallSign));
            XList &           list = node->branch();

            if(1!=list.size)                       throw Specific::Exception(CallSign,"%s(forbidden multiple entries)",func);
            if(!list.head->is(Formula::CallSign))  throw Specific::Exception(CallSign,"%s(forbidden '%s'')",func,list.head->name().c_str());
            return list.popHead();
        }

        template <typename DESCRIPTION> static inline
        XNode * singleFormulaFrom(const DESCRIPTION & description)
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.singleFormula( Lingo::Module::OpenData(description,description) );
        }


        Formula:: Formula(const String & description) :
        code( singleFormulaFrom(description) )
        {
        }

        Formula:: Formula(const char * const  description) :
        code( singleFormulaFrom(description) )
        {
        }


        const String * Formula:: text(int &z) const
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.formulaToText(*this,z);
        }

        
        String Formula:: uuid() const
        {
            int z = 0;
            AutoPtr<const String> uuid = text(z);
            return *uuid;
        }

    }

}


namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Weasel:: eval(const String &expr)
        {
            assert(0!=code);
            return code->lvm->eval<real_t>(expr);
        }
    }

}

namespace Yttrium
{
    namespace Chemical
    {
        Equilibrium  * Weasel:: compile(XNode * const root)
        {
            assert(0!=root);
            assert(root->defines<Equilibrium>());
            XList &list = root->branch(); assert(4==list.size);
            

        }
    }

}


