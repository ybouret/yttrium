#include "y/jive/pattern/regexp.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/jive/pattern.hpp"
#include "y/system/exception.hpp"

#include "y/concurrent/singulet.hpp"
#include "y/lockable.hpp"

#include "y/jive/pattern/first-chars.hpp"
#include "y/jive/pattern/dictionary.hpp"

using namespace Yttrium;

Y_UTEST(rx)
{
    Jive::Dictionary dict;

    dict("INT","[:digit:]+");

    if(argc>1)
    {
        std::cerr << "-- compiling '" << argv[1] << "'" << std::endl;
        AutoPtr<Jive::Pattern> P = Jive::RegExp::Compile(argv[1],&dict);
        std::cerr << "-- " << (P->isFragile()  ? "FRAGILE" : "REGULAR" ) << std::endl;
        std::cerr << "-- " << (P->isUnivocal() ? "UNIVOCAL" : "MULTIPLE") << std::endl;
        std::cerr << "-- saving..." << std::endl;
        P->toBinary("rx.dat");
        std::cerr << "-- rendering..." << std::endl;
        GraphViz::Vizible::DotToPng("rx.dot",*P);
        Jive::FirstChars fc;
        P->query(fc);

        std::cerr << "-- first chars : " << fc << std::endl;
        if(true)
        {
            std::cerr << "-- rendering..." << std::endl;
            const AutoPtr<Jive::Pattern> q = fc.makeOr();
            GraphViz::Vizible::DotToPng("fc.dot",*q);
        }
    }

}
Y_UDONE()
