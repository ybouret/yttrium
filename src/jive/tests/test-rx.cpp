#include "y/jive/pattern/regexp.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/jive/pattern.hpp"
#include "y/system/exception.hpp"

#include "y/concurrent/singulet.hpp"
#include "y/lockable.hpp"

#include "y/jive/pattern/first-chars.hpp"

using namespace Yttrium;

Y_UTEST(rx)
{

    //Jive::RegExpCompiler::Instance();

    if(argc>1)
    {
        std::cerr << "-- compiling '" << argv[1] << "'" << std::endl;
        AutoPtr<Jive::Pattern> P = Jive::RegExp::Compile(argv[1]);
        std::cerr << "-- " << (P->isFragile() ? "FRAGILE" : "REGULAR" ) << std::endl;
        std::cerr << "-- saving..." << std::endl;
        P->toBinary("rx.dat");
        std::cerr << "-- rendering..." << std::endl;
        Vizible::GraphViz("rx.dot",*P);
        Jive::FirstChars fc;
        P->query(fc);
        std::cerr << "-- first chars : " << fc << std::endl;
    }

}
Y_UDONE()
