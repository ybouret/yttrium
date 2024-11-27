#include "y/chemical/weasel/formula/linker.hpp"
#include "y/chemical/weasel/equilibrium/linker.hpp"
#include "y/utest/run.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {
#define Y_Chemical_Weasel "Chemical::Weasel"

        const char * const Weasel:: CallSign = Y_Chemical_Weasel;
        const char * const Weasel:: SYMBOL   = "<=>";

        namespace
        {


#define Y_Chemical_FORMULA     0
#define Y_Chemical_EQUILIBRIUM 1

            class Compiler
            {
            public:
                inline Compiler(const Lingo::Caption &caption) :
                genericParser(caption),
                formulaLinker(genericParser),
                ldEquilibrium(genericParser),
                treeNameIndex()
                {
                    treeNameIndex(*genericParser.FORMULA.name,     Y_Chemical_FORMULA);
                    treeNameIndex(*genericParser.EQUILIBRIUM.name, Y_Chemical_EQUILIBRIUM);

                }

                inline ~Compiler() noexcept
                {
                }

                Weasel::Parser      genericParser;
                Formula::Linker     formulaLinker;
                Equilibrium::Linker ldEquilibrium;
                Hashing::Perfect    treeNameIndex;


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

        void Weasel:: setupFormula(Formula &formula)
        {
            assert(0!=compiler);
            Formula::Linker &linker = compiler->formulaLinker;

            //------------------------------------------------------------------
            //
            // apply linker to Formula tree
            //
            //------------------------------------------------------------------
            linker(*formula.tree);

            //------------------------------------------------------------------
            //
            // fetch compiled info
            //
            //------------------------------------------------------------------
            Coerce( formula.name ).swapWith(linker.elements.tail());
            Coerce( formula.z    ) = linker.z;

            //------------------------------------------------------------------
            //
            // cleanup
            //
            //------------------------------------------------------------------
            linker.clear();
        }




        void Weasel:: operator()(Library &             lib,
                                 Equilibria &          eqs,
                                 Lingo::Module * const inp)
        {
            //------------------------------------------------------------------
            //
            // parse module
            //
            //------------------------------------------------------------------
            AutoPtr<XNode> root = parse(inp);

            assert( root.isValid() );
            assert(0!=compiler);
            assert( *(compiler->genericParser.WEASEL.name) == root->name());
            assert( XNode::Internal == root->type );

            //------------------------------------------------------------------
            //
            // parse module
            //
            //------------------------------------------------------------------
            XList &list = root->branch();
            while(list.size>0)
            {
                XTree         tree     = list.popHead();
                const String &treeName = tree->name();
                switch(compiler->treeNameIndex(treeName))
                {
                    case Y_Chemical_FORMULA:
                        (void) lib(tree);
                        break;

                    case Y_Chemical_EQUILIBRIUM:
                        compiler->ldEquilibrium.process(tree,lib,eqs);
                        break;

                    default:
                        throw Specific::Exception(CallSign, "unhandled '%s'", treeName.c_str() );
                }
            }

            //------------------------------------------------------------------
            //
            // update for format
            //
            //------------------------------------------------------------------
            eqs.update();
        }
    }

}
