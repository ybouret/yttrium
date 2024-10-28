#include "y/lingo/pattern/dictionary.hpp"
#include "y/utest/run.hpp"

#include "y/lingo/pattern/all.hpp"

using namespace Yttrium;
using namespace Lingo;


Y_UTEST(dict)
{

    Dictionary dict;
    dict.define("lower", new Lump('a','z') );
    dict.define("upper", new Lump('A','Z') );

    std::cerr << "dict=" << dict << std::endl;
    {
        Dictionary temp(dict);
        std::cerr << "temp=" << temp << std::endl;
    }

    std::cerr << "dict=" << dict << std::endl;




}
Y_UDONE()
