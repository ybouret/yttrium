
#include "y/stream/xmlog.hpp"



#include "y/utest/run.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;


Y_UTEST(io_xmlog)
{
    XMLog xml("root", Environment::Flag("VERBOSE") );

    Y_XMLOG(xml,"Hello");

    {
        XMLog::Markup sub(xml,"level1");
    }

    {
        Y_XMLSUB(xml,"level2");
    }

}
Y_UDONE()
