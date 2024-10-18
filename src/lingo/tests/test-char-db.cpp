
#include "y/lingo/pattern/char-db.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Lingo;



Y_UTEST(chardb)
{
    Y_SIZEOF(CharDB);
    Y_USHOW(CharDB::WORDS);
    Y_CHECK(256==CharDB::WORDS * CharDB::WORD_BITS);
}
Y_UDONE()
