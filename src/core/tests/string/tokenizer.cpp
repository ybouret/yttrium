
#include "y/string/tokenizer.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/container/iterator/display-all.hpp"

using namespace Yttrium;


Y_UTEST(string_tokenizer)
{

    const String s = " Hello, World! I'm Happy  ";

    Tokenizer tkn(s);
    while(tkn.next(" \t",2))
    {
        const String t(tkn.token(),tkn.units());
        std::cerr << "-- [" << t << "]" << std::endl;
    }

    List<String> L;
    Tokenizer::AppendTo(L,s," ",1);
    Iterating::DisplayAll::Of(L);
    Tokenizer::AppendTo(L,s," \t",2);
    Iterating::DisplayAll::Of(L);

}
Y_UDONE()
