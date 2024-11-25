#include "y/chemical/weasel/formula/linker.hpp"
#include "y/utest/run.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {
#define Y_Chemical_Weasel "Chemical::Weasel"

        const char * const Weasel:: CallSign = Y_Chemical_Weasel;


        namespace
        {
            class Compiler
            {
            public:
                inline Compiler(const Lingo::Caption &caption) :
                parser(caption),
                formulaLinker(parser)
                {
                }

                inline ~Compiler() noexcept
                {
                }

                Weasel::Parser  parser;
                Formula::Linker formulaLinker;
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            };

            static void     * compiler_[ Y_WORDS_FOR(Compiler) ];
            static Compiler * compiler = 0;
            static inline void zeroCompiler() noexcept
            {
                memset(compiler_,0,sizeof(compiler_));
            }
        }




        Weasel:: Weasel() :
        Singleton<Weasel>(),
        luaVM( new Lua::State() ),
        caption( CallSign )
        {
            assert(0==compiler);
            try {
                zeroCompiler();
                compiler = new (compiler_) Compiler(caption);
            }
            catch(...)
            {
                compiler = 0;
                zeroCompiler();
                throw;
            }

            compiler->parser.printRules();

        }

        Weasel:: ~Weasel() noexcept
        {
            assert(0!=compiler);
            Destruct(compiler);
            compiler=0;
            zeroCompiler();
        }


        

        XNode * Weasel:: parse(Lingo::Module * const m)
        {
            assert(0!=m);
            assert(0!=compiler);
            AutoPtr<XNode> tree = compiler->parser(m);
            GraphViz::Vizible::DotToPng("wtree.dot", *tree);
            return tree.yield();
        }

        XNode * Weasel:: parseFormula(const String &expr) {

            static const char fn[] = Y_Chemical_Weasel "::parseFormula";
            assert(0!=compiler);

            const String & FORMULA = *(compiler->parser).FORMULA.name;
            AutoPtr<XNode> tree    = parse( Lingo::Module::OpenData(FORMULA, expr) );

            assert( tree->name() == *(compiler->parser).WEASEL.name );
            assert( tree->type   == XNode::Internal);

            XList &        list = tree->branch(); if(list.size!=1) throw Specific::Exception(fn, "'%s' is not a single %s",expr.c_str(),FORMULA.c_str());
            XNode * const  node = list.head;
            const String & uuid = node->name();   if( FORMULA != uuid ) throw Specific::Exception(fn,"'%s' is not a %s",expr.c_str(),FORMULA.c_str());

            return list.pop(node);
        }


    }

}
