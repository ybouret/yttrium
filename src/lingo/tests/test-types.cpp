
#include "y/lingo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/lexical/add-on/jstring.hpp"
#include "y/lingo/lexical/add-on/single-line-comment.hpp"

#include "y/lingo/syntax/translator.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(types)
{
    Y_SIZEOF(Char);
    Y_SIZEOF(Token);
    Y_SIZEOF(Lexeme);
    Y_SIZEOF(Lexemes);
    std::cerr << std::endl;
    Y_SIZEOF(Lexical::Scanner);
    Y_SIZEOF(Lexical::Analyzer);
    Y_SIZEOF(Lexer);
    Y_SIZEOF(Lexer::History);
    Y_SIZEOF(Lexer::Database);
    Y_SIZEOF(Lexer::AddOns);
    std::cerr << std::endl;

    Y_SIZEOF(Syntax::Grammar);
    Y_SIZEOF(Parser);
    std::cerr << std::endl;

    Y_SIZEOF(Entity);
    Y_SIZEOF(Syntax::Translator);
    Y_SIZEOF(Syntax::Translator::OnTerminalMap);
    Y_SIZEOF(Syntax::Translator::OnInternalMap);
    Y_SIZEOF(AutoPtr<Syntax::Translator::OnInternalMap>);
    std::cerr << std::endl;

}
Y_UDONE()
