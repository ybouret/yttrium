#include "y/stream/xmlog.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;


Y_UTEST(io_xmlog)
{
    bool verb = Environment::Flag("VERBOSE");

    XMLog xml(verb);
    Y_XMLOG(xml,"Hello");
    {
        XMLog::Section sec(xml,"data",true);
        Y_XMLOG(xml,"blah blah");

        {
            Y_XML_SECTION(xml, "subsec");
            Y_XMLOG(xml,"blah blah");
            {
                Y_XML_SECTION_OPT(xml,"again"," count='1'");
            }
        }


    }

    

}
Y_UDONE()
