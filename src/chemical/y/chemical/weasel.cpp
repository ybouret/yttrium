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
#include "y/chemical/weasel/formula/to-html.hpp"
#include "y/lua++/function.hpp"
#include <cstring>
#include "y/chemical/weasel/initial/compiler.hpp"

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
            formulaToHTML(),
            initialCompiler(),
            lvm( new Lua::State() )
            {
            }

            inline ~Code() noexcept {}

            Parser            parser;
            FormulaToText     formulaToText;
            FormulaToHTML     formulaToHTML;
            Initial::Compiler initialCompiler;
            Lua::VM           lvm;

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

        const String * Weasel:: formulaToHTML(const Formula &f)
        {
            assert(0!=code);
            return code->formulaToHTML.get(*f);
        }

        Initial::Design * Weasel:: compileDesign(const XNode &root, Library &lib)
        {
            assert(0!=code);
            return code->initialCompiler(root,lib);
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
            AutoPtr<XNode>    node   = parse(inputModule); assert(node->is(Weasel::CallSign));
            XList &           list   = node->branch();

            if(1!=list.size)                       throw Specific::Exception(CallSign,"%s(forbidden multiple entries)",func);
            if(!list.head->is(Formula::CallSign))  throw Specific::Exception(CallSign,"%s(forbidden '%s'')",func,list.head->name().c_str());
            return Formula(list.popHead());
        }

        XNode * Weasel:: singleFormula(Lingo::Module * const inputModule)
        {
            static const char func[] = "singleFormula";
            AutoPtr<XNode>    node   = parse(inputModule); assert(node->is(Weasel::CallSign));
            XList &           list   = node->branch();

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


        const String * Formula:: toText(int &z) const
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.formulaToText(*this,z);
        }

        const String * Formula:: toHTML() const
        {
            static Weasel &weasel = Weasel::Instance();
            return weasel.formulaToHTML(*this);
        }

        
        String Formula:: uuid() const
        {
            int z = 0;
            AutoPtr<const String> uuid = toText(z);
            return *uuid;
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {
        Weasel & Weasel:: operator<<(const String &luaCode)
        {
            code->lvm->dostring(luaCode);
            return *this;
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

#include "y/chemical/reactive/equilibrium/raw.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace {

            class LuaEquilibrium : public RawEquilibrium
            {
            public:
                inline explicit LuaEquilibrium(Library &     lib,
                                               XNode * const root,
                                               const size_t  eidx,
                                               Lua::VM &     lvm) :
                RawEquilibrium(lib,root,eidx),
                vm(lvm)
                {
                }

                inline virtual ~LuaEquilibrium() noexcept
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(LuaEquilibrium);
                Lua::VM vm;

                virtual xreal_t getK(xreal_t t)
                {
                    lua_State  * const L = **vm;
                    const char * const f = Kdata->c_str();
                    lua_settop(L,0);
                    lua_getglobal(L,f);
                    lua_pushnumber(L,static_cast<lua_Number>(t));

                    /* do the call (1 arguments, 1 result) */
                    if (lua_pcall(L, 1, 1, 0) != 0)
                        throw Specific::Exception(name->c_str(),"call '%s': %s",f, lua_tostring(L, -1));

                    /* retrieve result */
                    if (!lua_isnumber(L, -1))
                        throw Specific:: Exception(name->c_str(),"function '%s' must return a number, no %s", f, luaL_typename(L,-1));

                    const xreal_t k = static_cast<xreal_t>(lua_tonumber(L,-1));
                    lua_pop(L,1);
                    return k;
                }
            };

        }


    }

}

#include "y/chemical/reactive/equilibrium/const.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Equilibrium  * Weasel:: compile(Library &     lib,
                                        XNode * const root,
                                        const size_t  eidx)
        {
            assert(0!=root);
            assert(root->defines<Equilibrium>());
            XList &list = root->branch(); assert(4==list.size);

            const String &data = list.tail->lexeme().toString();
            //std::cerr << "data=" << data << std::endl;
            if(data.size()<=0)
            {
                const String  uuid = list.head->lexeme().toString(1,0);
                throw Specific::Exception(uuid.c_str(),"empty constant description");
            }

            if( isdigit(data[1]) )
            {
                // assuming const
                return new ConstEquilibrium(lib,root,eidx);
            }

            // assuming lua
            return new LuaEquilibrium(lib,root,eidx,code->lvm);
        }
    }

}


