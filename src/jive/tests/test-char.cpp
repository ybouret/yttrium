

#include "y/jive/token.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;


Y_UTEST(char)
{

    Y_SIZEOF(Jive::Char);
    const Jive::Context ctx("here",Jive::AsTag);
    Jive::Token         tkn;
    tkn.pushTail( new Jive::Char(ctx,'a') );
    tkn.pushTail( new Jive::Char(ctx,'b') );
    tkn.pushTail( new Jive::Char(ctx,'c') );

    std::cerr << tkn << std::endl;
    
}
Y_UDONE()

