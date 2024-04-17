
#include "y/chemical/lang/rosary.hpp"
#include "y/jive/parser.hpp"
#include "y/jive/syntax/translator.hpp"
#include "y/type/temporary.hpp"
#include "y/system/exception.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Rosary::CallSign = "ChemicalRosary";

        class Rosary::Compiler : public Jive::Parser, Jive::Syntax::Translator
        {
        public:
            enum ZSign {
                ZPositive,
                ZNegative
            };
            typedef Vector<String> Strings;
            typedef Vector<ZSign>  Signs;
            typedef Vector<int>    Charges;

            explicit Compiler() :
            Jive::Parser(Rosary::CallSign),
            Jive::Syntax::Translator(),
            l(0)
            {
                setupParser();
                setupLinker();
            }

            inline virtual ~Compiler() noexcept
            {
            }

            void process(Jive::Module *m,
                         Library      &lib)
            {
                const Temporary<Library *> temp(l,&lib);
                Parser                    &self = *this;
                const AutoPtr<XNode>       tree = self(m);
                if(tree.isEmpty()) throw Specific::Exception(Rosary::CallSign, "Unexpected Empty Syntax Tree!");

                GraphViz::Vizible::DotToPng( "rosary.dot", *tree);
                translate(*tree,Jive::Syntax::Permissive);
            }


            Strings           uuids;
            Signs             signs;
            Charges           charges;
            Species::SoloList species;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            Library *l;

            void setupParser();
            void setupLinker();
            void clearState() noexcept
            {
                uuids.free();
                signs.free();
                charges.free();
                species.free();
            }

            virtual void initialize()
            {
                std::cerr << "*** Initializing..." << std::endl;
                clearState();
            }

            void onUUID(  const Jive::Token &token) { uuids << token.toString(); }
            void onPLUS(  const Jive::Token &) { signs << ZPositive; }
            void onMINUS( const Jive::Token &) { signs << ZNegative; }
            void onZPOS(size_t n)
            {
                assert(n>0);
                int count = 0;
                while(n-- > 0)
                {
                    assert(signs.size()>0);
                    assert(ZPositive == signs.tail());
                    signs.popTail();
                    ++count;
                }
                std::cerr << "charge +" << count << std::endl;
                charges << count;
            }

            void onZNEG(size_t n)
            {
                assert(n>0);
                int count = 0;
                while(n-- > 0)
                {
                    assert(signs.size()>0);
                    assert(ZNegative == signs.tail());
                    signs.popTail();
                    --count;
                }
                charges << count;
            }

            void onSPECIES(const size_t n)
            {
                assert(1==n||2==n);
                assert(uuids.size()>0);
                const String name = uuids.pullTail();
                int          z    = 0;
                if(2==n)
                {
                    assert(charges.size()>0);
                    z = charges.pullTail();
                }
                Library       &lib  = *l;
                const Species &sp   = lib(name,z);
                species << sp;
            }


        };

        // setting up parser
        void Rosary::Compiler::setupParser()
        {
            Agg       &ROSARY   = agg("ROSARY"); // top-level
            const Rule &PLUS    = term('+');
            const Rule &MINUS   = term('-');
            const Rule &UUID    = term("UUID","[:alpha:][:word:]*");
            const Rule &ZPOS    = (agg("ZPOS") << oom(PLUS));
            const Rule &ZNEG    = (agg("ZNEG") << oom(MINUS));
            const Rule &SPECIES = (agg("SPECIES") << mark('[') << UUID << opt( pick(ZPOS,ZNEG) ) << mark(']'));
            
            ROSARY += zom(SPECIES);


            renderGraphViz();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");
        }

        // setting up linker
        void Rosary::Compiler:: setupLinker()
        {
            Y_Jive_OnTerminal(Compiler,UUID);
            forTerminal('+', *this, &Compiler::onPLUS);
            forTerminal('-', *this, &Compiler::onMINUS);
            Y_Jive_OnInternal(Compiler,ZPOS);
            Y_Jive_OnInternal(Compiler,ZNEG);
            Y_Jive_OnInternal(Compiler,SPECIES);
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

