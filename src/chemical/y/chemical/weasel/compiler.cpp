
#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/weasel/linker.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            const char * const Compiler:: CallSign = "Weasel";

            class Compiler:: Code
            {
            public:
                inline  Code() : parser( CallSign ), linker( CallSign ), rx() {}
                inline ~Code() noexcept {}

                Parser         parser;
                Linker         linker;
                Vector<String> rx;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

            static void *          _code[ Y_WORDS_FOR(Compiler::Code) ];
            static Compiler::Code * code = 0;


            Compiler:: Compiler()
            {
                std::cerr << "sizeof(Code)=" << sizeof(_code) << std::endl;
                code = new ( Y_STATIC_ZARR(_code) ) Code();
            }

            Compiler:: ~Compiler() noexcept
            {
                assert(0!=code);
                Destruct(code);
                code = 0;
                Y_STATIC_ZARR(_code);
            }

            void Compiler:: operator()(Library       &usrLib,
                                       LuaEquilibria &usrEqs,
                                       Jive::Module  *input,
                                       const bool     saveAst)
            {
                AutoPtr<Jive::Syntax::XNode> ast = code->parser.load(input);

                if(saveAst)
                    GraphViz::Vizible::DotToPng( "ast.dot", *ast);

                code->rx.free();

                // first pass: full link
                code->linker(*ast,usrLib,usrEqs,&code->rx);

                // second pass: using rx
                std::cerr << "rx=" << code->rx << std::endl;

                while(code->rx.size()>0)
                {
                    const String id = code->rx.pullHead();
                }

            }

        }
    }

}

