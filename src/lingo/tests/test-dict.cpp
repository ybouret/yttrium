#include "y/lingo/pattern/dictionary.hpp"
#include "y/utest/run.hpp"

#include "y/lingo/pattern/all.hpp"

using namespace Yttrium;
using namespace Lingo;


Y_UTEST(dict)
{

    Dictionary dict;
    dict("lower", new Range('a','z') );
    dict("upper", new Range('A','Z') );


    



}
Y_UDONE()
