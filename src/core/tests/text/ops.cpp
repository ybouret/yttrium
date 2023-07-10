

#include "y/text/ops.hpp"
#include "y/utest/run.hpp"

#include <cstring>

using namespace Yttrium;


Y_UTEST(text_ops)
{

    char str[] = "Hello, World!!   \n";
    char buffer[128];
    const char bad[] = { ' ', '\t', '\r', '\n' };
    char  *output = TextOps::CopyMessage(buffer,sizeof(buffer),str);
    std::cerr << TextOps::TrimInvalid(output,strlen(output),bad,sizeof(bad)) << std::endl;
    

}
Y_UDONE()
