#include "y/chemical/weasel/parser.hpp"
#include "y/utest/run.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Weasel:: CallSign = "Chemical::Weasel";


        namespace
        {
            class Compiler
            {
            public:
                inline Compiler(const Lingo::Caption &caption) :
                parser(caption)
                {
                }

                inline ~Compiler() noexcept
                {
                }

                Weasel::Parser parser;
                
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
        }

        Weasel:: ~Weasel() noexcept
        {
            assert(0!=compiler);
            Destruct(compiler);
            compiler=0;
            zeroCompiler();
        }


        void Weasel:: operator()(Lingo::Module * const m)
        {
            assert(0!=m);
            assert(0!=compiler);
            AutoPtr<Lingo::Syntax::XNode> tree = compiler->parser(m);
            GraphViz::Vizible::DotToPng("wtree.dot", *tree);
        }

    }

}
