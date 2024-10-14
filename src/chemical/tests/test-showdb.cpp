
#include "y/chemical/plexus.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(showdb)
{
    Plexus plexus(true);
    plexus.wpc.showDB();
}
Y_UDONE()

