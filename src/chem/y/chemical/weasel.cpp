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
        const char * const Weasel:: EqSep    = "<=>";
        
        namespace
        {
            class Compiler
            {
            public:
                inline Compiler(const Lingo::Caption &caption) :
                genericParser(caption),
                formulaLinker(genericParser)
                {
                }

                inline ~Compiler() noexcept
                {
                }

                Weasel::Parser  genericParser;
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

            compiler->genericParser.printRules();

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
            Parser       & parser = compiler->genericParser;
            AutoPtr<XNode> tree   = parser.postProcess( parser(m) );
            GraphViz::Vizible::DotToPng("wtree.dot", *tree);
            return tree.yield();
        }

        XNode * Weasel:: parseFormula(const String &expr) {

            static const char fn[] = Y_Chemical_Weasel "::parseFormula";
            assert(0!=compiler);
            const Parser & parser  = compiler->genericParser;
            const String & FORMULA = *parser.FORMULA.name;
            AutoPtr<XNode> tree    = parse( Lingo::Module::OpenData(FORMULA, expr) );

            assert( tree->name() == *parser.WEASEL.name );
            assert( tree->type   == XNode::Internal);

            XList &        list = tree->branch(); if( list.size != 1  ) throw Specific::Exception(fn, "'%s' is not a single %s",expr.c_str(),FORMULA.c_str());
            XNode * const  node = list.head;
            const String & uuid = node->name();   if( FORMULA != uuid ) throw Specific::Exception(fn,"'%s' is not a %s",expr.c_str(),FORMULA.c_str());

            // return extracted node
            return list.pop(node);
        }

        XNode * Weasel:: parseFormula(const char * const expr)
        {
            const String _(expr);
            return parseFormula(_);
        }

        void Weasel:: buildFormula(Formula &formula)
        {
            assert(0!=compiler);
            Formula::Linker &linker = compiler->formulaLinker;
            linker.policy  = Lingo::Syntax::Permissive;
            linker.verbose = true;

            // link formula
            linker(*formula.tree);

            // fetch compiled nfo
            Coerce( formula.name ).swapWith(linker.elements.tail());
            Coerce( formula.z    ) = linker.z;

            // cleanup
            linker.clear();
        }

    }

}
