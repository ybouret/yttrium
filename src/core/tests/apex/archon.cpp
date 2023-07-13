
#include "y/apex/m/archon.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;


Y_UTEST(apex_archon)
{
    Apex::Archon &mgr = Apex::Archon::Instance();
    std::cerr << mgr.callSign() << std::endl;
}
Y_UDONE()
