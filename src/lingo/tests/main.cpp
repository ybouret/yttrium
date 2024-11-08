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
    Y_UTEST(scanner);
    Y_UTEST(lexer);
    Y_UTEST(syntax);
    Y_UTEST(grammar);
}
Y_UTEST_EXEC()
