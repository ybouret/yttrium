#include "y/chemical/weasel/formula/linker.hpp"
#include "y/chemical/weasel/formula/html.hpp"
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
#define Y_Chemical_SEARCH      2
#define Y_Chemical_INSTR       3

            class Compiler
            {
            public:
                inline Compiler(const Lingo::Caption   &caption,
                                const Hashing::Perfect &hashing) :
                genericParser(caption,hashing),
                formulaLinker(genericParser),
                ldEquilibrium(genericParser),
                formulaToHTML(genericParser),
                treeNameIndex()
                {
                    treeNameIndex(genericParser.FORMULA.name,      Y_Chemical_FORMULA);
                    treeNameIndex(*genericParser.EQUILIBRIUM.name, Y_Chemical_EQUILIBRIUM);
                    treeNameIndex(*genericParser.SEARCH.name,      Y_Chemical_SEARCH);
                    treeNameIndex(*genericParser.INSTR.name,       Y_Chemical_INSTR);

                }
                

                inline ~Compiler() noexcept
                {
                }


                Weasel::Parser      genericParser;
                Formula::Linker     formulaLinker;
                Equilibrium::Linker ldEquilibrium;
                Formula::ToHTML     formulaToHTML;
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


        static const char * actorsTable[] = 
        {
            "REAC",
            "PROD"
        };

        static inline
        void DestructCompiler() noexcept
        {
            assert(0!=compiler);
            Destruct(compiler);
            compiler=0;
            zeroCompiler();
        }

        Weasel:: Weasel() :
        Singleton<Weasel>(),
        luaVM( new Lua::State() ),
        caption( CallSign ),
        hashing( Y_Hashing_Perfect_Table(actorsTable) ),
        schemes()
        {

            // initialization
            assert(0==compiler);
            try {
                zeroCompiler();
                compiler = new (compiler_) Compiler(caption,hashing);
            }
            catch(...)
            {
                compiler = 0;
                zeroCompiler();
                throw;
            }

            // post-init
            try
            {
                on("lua", *this, & Weasel::executeLuaCode );
            }
            catch(...)
            {
                DestructCompiler();
                throw;
            }


        }

        Weasel:: ~Weasel() noexcept
        {
            DestructCompiler();
        }




        XNode * Weasel:: parse(Lingo::Module * const m)
        {
            assert(0!=m);
            assert(0!=compiler);
            Parser       & parser = compiler->genericParser;
            AutoPtr<XNode> tree   = parser.postProcess( parser(m) );
            //GraphViz::Vizible::DotToPng("wtree.dot", *tree);
            return tree.yield();
        }

        XNode * Weasel:: parseSingle(const Lingo::Syntax::Rule &rule,
                                     const String              &expr,
                                     const char * const         func)
        {
            assert(0!=compiler);
            //const Parser & parser  = compiler->genericParser;
            const String & NAME    = *rule.name;
            AutoPtr<XNode> tree    = parse( Lingo::Module::OpenData(NAME, expr) );

            assert( tree->name() == *compiler->genericParser.WEASEL.name );
            assert( tree->type   == XNode::Internal);

            XList &        list = tree->branch();
            if( list.size != 1  )
                throw Specific::Exception(func, "'%s' is not a single %s",expr.c_str(),NAME.c_str());

            XNode * const  node = list.head;
            const String & uuid = node->name();
            if( NAME != uuid )
                throw Specific::Exception(func,"'%s' is not a %s",expr.c_str(),NAME.c_str());

            // return extracted node
            return list.pop(node);
        }


        XNode * Weasel:: parseFormula(const String &expr) {

            static const char func[] = Y_Chemical_Weasel "::parseFormula";
            assert(0!=compiler);
            return parseSingle(compiler->genericParser.FORMULA,expr,func);
        }

        XNode * Weasel:: parseFormula(const char * const expr)
        {
            const String _(expr);
            return parseFormula(_);
        }

        XNode  * Weasel:: parseEquilibrium(const String &expr)
        {
            static const char func[] = Y_Chemical_Weasel "::parseEquilibrium";
            assert(0!=compiler);
            return parseSingle(compiler->genericParser.EQUILIBRIUM,expr,func);
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


        void  Weasel:: setupEquilibrium(XTree &tree, Library &lib, Equilibria &eqs)
        {
            assert(0!=compiler);
            compiler->ldEquilibrium.process(tree,lib,eqs);
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
            // process results
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
                        setupEquilibrium(tree,lib,eqs);
                        break;

                    case Y_Chemical_SEARCH:
                        queryEquilibrium(tree,lib,eqs);
                        break;

                    case Y_Chemical_INSTR:
                        queryInstruction(tree,lib,eqs);
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
            eqs.updateFragment();
        }


        String Weasel:: toHTML(const Formula &formula)
        {
            assert(0!=compiler);
            Formula::ToHTML &linker = compiler->formulaToHTML;
            //linker.verbose = true;
            //linker.policy  = Lingo::Syntax::Permissive;

            linker( *formula.tree );
            String ans;
            ans = linker.elements.head();
            return ans;
        }

        String Formula:: html() const
        {
            static Weasel &weasel = Weasel:: Instance();
            return weasel.toHTML(*this);
        }

    }

}

#include "y/chemical/weasel/equilibrium/db.hpp"
#include "y/lingo/pattern/matching.hpp"
#include "y/string/tokenizer.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        void Weasel:: queryEquilibrium(XTree &       search,
                                       Library &     lib,
                                       Equilibria &  eqs)
        {
            assert(0!=compiler);
            assert(search->name() == *(compiler->genericParser.SEARCH.name) );

            //------------------------------------------------------------------
            //
            // prepare search
            //
            //------------------------------------------------------------------
            const String               expr = search->lexeme().toString(1,0);
            Lingo::Matching            matching(expr);
            Vector<String,MemoryModel> words(4,AsCapacity);
            unsigned                   count = 0;

            //------------------------------------------------------------------
            //
            // loop over pre-compiled descriptions
            //
            //------------------------------------------------------------------
            for(size_t i=0;i<EDB::Count;++i)
            {
                const String data = EDB::Table[i];

                // split with ':'
                words.free();
                Tokenizer::AppendTo(words,data,COLON);
                if(words.size()<=0) continue;

                // prepare label
                String &label = words[1];
                Algo::Crop(label,isblank);
                if(label.size()<=0) continue;
                if('@' != label[1]) continue;
                label.skip(1);

                // check label
                if( matching.exactly(search->name(),label) )
                {
                    XTree  tree = parseEquilibrium(data);
                    setupEquilibrium(tree,lib,eqs);
                    ++count;
                }
            }

            if(count<=0)
                throw Specific::Exception(CallSign,"no matching '%s' in database", expr.c_str() );
        }


        void Equilibria :: operator()(Library &lib, const String &expr)
        {
            static Weasel &weasel = Weasel::Instance();
            weasel(lib,*this, Lingo::Module::OpenData(expr,expr) );
        }

        void Equilibria:: operator()(Library &lib, const char * const expr)
        {
            const String _(expr);
            (*this)(lib,_);
        }



    }

}

namespace Yttrium
{

    namespace Chemical
    {

        void Weasel:: queryInstruction(XTree &instr, Library &lib, Equilibria &eqs)
        {
            assert(0!=compiler);
            assert(instr->name() == *(compiler->genericParser.INSTR.name) );
            const XNode *node = instr->branch().head; assert("LABEL"==node->name());


            //------------------------------------------------------------------
            //
            // get label
            //
            //------------------------------------------------------------------
            const String label = node->lexeme().toString(1,0);

            //------------------------------------------------------------------
            //
            // aggregate instructions
            //
            //------------------------------------------------------------------
            Strings strings;
            for(node=node->next;node;node=node->next)
            {
                strings << node->lexeme().toString(1,1);
            }

            //std::cerr << "processing '" << label << "(" << strings << ")'" << std::endl;

            //------------------------------------------------------------------
            //
            // look for scheme
            //
            //------------------------------------------------------------------
            Scheme * const pScheme = schemes.search( label );
            if(0==pScheme)
                throw Specific::Exception(CallSign,"no recorded instruction '#%s'", label.c_str());

            //------------------------------------------------------------------
            //
            // prepare arguments
            //
            //------------------------------------------------------------------
            Args args = {
                strings,
                lib,
                eqs
            };

            //------------------------------------------------------------------
            //
            // and execute
            //
            //------------------------------------------------------------------
            Scheme &F = *pScheme;
            F(args);
        }

        void Weasel:: record(const String &label, const Scheme &scheme)
        {
            if( !schemes.insert(label,scheme) ) {
                throw Specific::Exception(CallSign, "record multiple instruction label='%s'", label.c_str());
            }
        }


        void Weasel:: record(const char *const label, const Scheme &scheme)
        {
            const String _(label); record(_,scheme);
        }

        void Weasel:: executeLuaCode(Args &args)
        {
            //std::cerr << "Executing Lua Code..." << std::endl;
            const size_t n = args.arg.size();
            for(size_t i=1;i<=n;++i)
            {
                luaVM->dostring(args.arg[i]);
            }
        }

    }

}
