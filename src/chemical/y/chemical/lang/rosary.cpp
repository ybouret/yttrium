
#include "y/chemical/lang/rosary.hpp"
#include "y/jive/parser.hpp"
#include "y/jive/syntax/translator.hpp"
#include "y/type/temporary.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Rosary::CallSign = "ChemicalRosary";

        class Rosary::Compiler : public Jive::Parser, Jive::Syntax::Translator
        {
        public:
            explicit Compiler() :
            Jive::Parser(CallSign),
            Jive::Syntax::Translator(),
            lib(0)
            {
                setupParser();
            }

            inline virtual ~Compiler() noexcept
            {
            }

            void process(Jive::Module *m,
                         Library      &l)
            {
                const Temporary<Library *> temp(lib,&l);
                Parser                    &self = *this;
                const AutoPtr<XNode>       tree = self(m);
                if(tree.isValid())
                {
                    std::cerr << "Valid Tree" << std::endl;
                    GraphViz::Vizible::DotToPng( "rosary.dot", *tree);

                }
                else
                {
                    std::cerr << "Empty Tree..." << std::endl;
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            void setupParser();
            Library *lib;
        };

        void Rosary::Compiler::setupParser()
        {
            Agg       &ROSARY   = agg("ROSARY"); // top-level
            //const Rule &PLUS    = term('+');
            //const Rule &MINUS   = term('-');
            const Rule &UUID    = term("UUID","[:alpha:][word]*");
            const Rule &SPECIES = agg("SPECIES") << mark('[') << UUID << mark(']');

            ROSARY += zom(SPECIES);


            renderGraphViz();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");
        }

        static void *            RosaryCompiler_[ Y_WORDS_FOR(Rosary::Compiler) ];
        static Rosary::Compiler *RosaryCompiler = 0;


        Rosary:: Rosary()
        {
            std::cerr << "sizeof(Compiler)=" << sizeof(RosaryCompiler_) << std::endl;
            assert(0==RosaryCompiler);
            try {

                RosaryCompiler = new ( Y_STATIC_ZARR(RosaryCompiler_) ) Rosary::Compiler();
            }
            catch(...)
            {
                RosaryCompiler = 0;
                throw;
            }
        }

        Rosary:: ~Rosary() noexcept
        {
            assert(0!=RosaryCompiler);
            (void)Memory::OutOfReach::Naught(RosaryCompiler);
            RosaryCompiler = 0;
        }


        void Rosary:: operator()(Jive::Module *m,
                                 Library      &lib)
        {
            assert(0!=RosaryCompiler);
            RosaryCompiler->process(m,lib);
        }
    }

}

