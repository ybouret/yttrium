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


    for(Dictionary::Iterator it = dict.begin(); it != dict.end(); ++it)
    {
        std::cerr << it->key << std::endl;
        const PatternPtr &pp = *it;
        Y_CHECK(pp.isValid());
    }

    Y_CHECK( dict.search("lower") );
    Y_CHECK( dict.search("upper") );



}
Y_UDONE()
