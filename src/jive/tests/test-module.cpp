


#include "y/jive/source.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;


Y_UTEST(module)
{
    Y_SIZEOF(Jive::Context);
    Y_SIZEOF(Jive::Char);
    Y_SIZEOF(Jive::Module);
    Y_SIZEOF(Jive::Source);

    if(argc>1)
    {
        Jive::Source source( Jive::Module::OpenFile(argv[1]) );
        std::cerr << "open " << source->tag << std::endl;

        Jive::Token  token;
        while(source.guess(token))
        {
            std::cerr << token << std::endl;
        }
    }

}
Y_UDONE()

