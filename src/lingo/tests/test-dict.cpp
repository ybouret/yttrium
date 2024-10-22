#include "y/lingo/pattern/dictionary.hpp"
#include "y/utest/run.hpp"

#include "y/lingo/pattern/all.hpp"

using namespace Yttrium;
using namespace Lingo;


Y_UTEST(dict)
{

    Dictionary dict;
    dict("lower", new Lump('a','z') );
    dict("upper", new Lump('A','Z') );


    



}
Y_UDONE()
