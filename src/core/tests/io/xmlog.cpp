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

    {
        XMLog::Markup sub(xml,"toto",false);
        *xml << " value='" << 1 << "'";
        sub.endl();
    }

    {
        Y_XMLSUB_OPT(xml, "level3", " value='3'");
        
    }

}
Y_UDONE()
