
#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(32)
{
    Y_UTEST(context);
    Y_UTEST(char);
    Y_UTEST(module);
    Y_UTEST(fc);
    Y_UTEST(pattern);
    Y_UTEST(posix);
    Y_UTEST(dict);
    Y_UTEST(rx);
    Y_UTEST(scanner);
    Y_UTEST(match);
    Y_UTEST(lexer);
    Y_UTEST(grammar);
    Y_UTEST(parser);
    Y_UTEST(eval);
}
Y_UTEST_EXEC()

