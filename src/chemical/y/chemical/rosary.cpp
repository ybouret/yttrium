
#include "y/chemical/rosary.hpp"
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
                Vector<String>               rxp;

                // first pass: build species/equilibria and gather rxp
                {
                    AutoPtr<Jive::Syntax::XNode> tree = parser(m); assert(tree.isValid());
                    linker(*tree,lib,eqs,&rxp);
                }

                // second pass: find rxp in db
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
                        if(!sep) throw Specific::Exception(Rosary::CallSign,"corrupted internal database");

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
                    if(count<=0) throw Specific::Exception(Rosary::CallSign,"no database entry matching '%s'", rx.c_str());
                }

                // third pass: finalize
                eqs.finalize();
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

