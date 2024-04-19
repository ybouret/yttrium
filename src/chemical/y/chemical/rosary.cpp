
#include "y/chemical/rosary.hpp"
#include "y/chemical/lang/parser.hpp"
#include "y/chemical/lang/linker.hpp"



namespace Yttrium
{
    namespace Chemical
    {

        const char * const Rosary::CallSign = "ChemicalRosary";

        class Rosary::Compiler
        {
        public:
            explicit Compiler() :
            parser(Rosary::CallSign),
            linker(Rosary::CallSign)
            {
            }

            virtual ~Compiler() noexcept
            {
            }

            inline void process(Jive::Module  *m,
                                Library       &lib,
                                LuaEquilibria &eqs)
            {
                AutoPtr<Jive::Syntax::XNode> tree = parser(m); assert(tree.isValid());
                linker(*tree,lib,eqs);
            }

            
            Parser parser;
            Linker linker;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };

        static void *            RosaryCompiler_[ Y_WORDS_FOR(Rosary::Compiler) ];
        static Rosary::Compiler *RosaryCompiler = 0;


        Rosary:: Rosary()
        {
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


        void Rosary:: operator()(Jive::Module  *m,
                                 Library       &lib,
                                 LuaEquilibria &eqs)
        {
            assert(0!=RosaryCompiler);
            RosaryCompiler->process(m,lib,eqs);
        }
    }

}

