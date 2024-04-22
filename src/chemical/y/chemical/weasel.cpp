
#include "y/chemical/weasel.hpp"
#include "y/chemical/lang/parser.hpp"
#include "y/chemical/lang/linker.hpp"
#include "y/jive/pattern/matcher.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        static const char *dbEntry[] =
        {
#include "lang/db.hxx"
        };

        static const size_t dbCount = sizeof(dbEntry)/sizeof(dbEntry[0]);


        const char * const Weasel::CallSign = "ChemicalWeasel";

        class Weasel::Compiler
        {
        public:
            explicit Compiler() :
            parser(Weasel::CallSign),
            linker(Weasel::CallSign)
            {
            }

            virtual ~Compiler() noexcept
            {
            }

            inline void process(Jive::Module  *m,
                                Library       &lib,
                                LuaEquilibria &eqs)
            {
                Vector<String>               rxp;

                //--------------------------------------------------------------
                //
                // first pass: build species/equilibria and gather rxp
                //
                //--------------------------------------------------------------
                {
                    AutoPtr<Jive::Syntax::XNode> tree = parser(m); assert(tree.isValid());
                    linker(*tree,lib,eqs,&rxp);
                }

                //--------------------------------------------------------------
                //
                // second pass: find rxp in db
                //
                //--------------------------------------------------------------
                const size_t nxp = rxp.size();
                for(size_t i=1;i<=nxp;++i)
                {
                    const String &rx = rxp[i];
                    std::cerr << "Looking for " << rx << std::endl;
                    Jive::Matcher match(rx);
                    size_t        count = 0;
                    for(size_t i=0;i<dbCount;++i)
                    {
                        // find entry
                        const char * const entry = dbEntry[i];
                        const char * const sep   = strchr(entry, Equilibrium::Separator);
                        if(!sep) throw Specific::Exception(Weasel::CallSign,"corrupted internal database");

                        // find label
                        String label(entry,sep-entry);
                        Algo::Crop(label,isspace);

                        // test label
                        if(match.somehow(label,label))
                        {
                            std::cerr << "match <" << label << ">" << std::endl;
                            AutoPtr<Jive::Syntax::XNode> tree = parser( Jive::Module::OpenData(label,entry) ); assert(tree.isValid());
                            linker(*tree,lib,eqs,0);
                            ++count;
                        }
                    }
                    if(count<=0) throw Specific::Exception(Weasel::CallSign,"no database entry matching '%s'", rx.c_str());
                }

                //--------------------------------------------------------------
                //
                // third pass: finalize input
                //
                //--------------------------------------------------------------
                eqs.finalize();
            }

            
            Parser parser;
            Linker linker;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
        };

        static void *            TheCompiler_[ Y_WORDS_FOR(Weasel::Compiler) ];
        static Weasel::Compiler *TheCompiler = 0;


        Weasel:: Weasel()
        {
            assert(0==TheCompiler);
            try {

                TheCompiler = new ( Y_STATIC_ZARR(TheCompiler_) ) Weasel::Compiler();
            }
            catch(...)
            {
                TheCompiler = 0;
                throw;
            }
        }

        Weasel:: ~Weasel() noexcept
        {
            assert(0!=TheCompiler);
            (void)Memory::OutOfReach::Naught(TheCompiler);
            TheCompiler = 0;
        }


        void Weasel:: operator()(Jive::Module  *m,
                                 Library       &lib,
                                 LuaEquilibria &eqs)
        {
            assert(0!=TheCompiler);
            TheCompiler->process(m,lib,eqs);
        }
    }

}

