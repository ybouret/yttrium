#include "y/utest/driver.hpp"

Y_UTEST_DECL(32)
{
    Y_UTEST(source);
    Y_UTEST(chardb);
    Y_UTEST(pattern);
    Y_UTEST(makerx);
    Y_UTEST(nbits);
    Y_UTEST(dict);
    Y_UTEST(posix);
    Y_UTEST(rx);
    Y_UTEST(matching);
    Y_UTEST(scanner);
    Y_UTEST(lexer);
    Y_UTEST(syntax);
    Y_UTEST(grammar);
    Y_UTEST(parser);
    Y_UTEST(parser_nl);
    Y_UTEST(types);
    Y_UTEST(eval);
    Y_UTEST(vfs);
}
Y_UTEST_EXEC()
