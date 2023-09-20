#include "y/string/env.hpp"
#include "y/utest/run.hpp"

#include <cstring>

#if 0
extern "C" const char **environ;
#endif

namespace Yttrium
{


#if 0
    void getFrom(const char * const env)
    {
        const char *curr = env;


    CYCLE:
        const char *next = curr;
        while(0 != *(++next) )
            ;

        const size_t size = (next-curr);


        std::cerr << "size=" << size << std::endl;
        if(1==size && 0==*next)
        {
            std::cerr << "done" << std::endl;
            return;
        }
        const String content(curr,size);
        std::cerr << "|_[" << content << "]" << std::endl;
        curr = ++next;
        goto CYCLE;

    }
    static const char data[] =
    "hello=world\0"
    "abc=def\0"
    "\0"
    //"hello=world\0"     "abc=def\0"
    ;
#endif


}

using namespace Yttrium;

Y_UTEST(string_env)
{
#if 0
    for(size_t i=0;;++i)
    {
        const char * const entry = environ[i];
        if(0==entry) break;
        std::cerr << entry << std::endl;
    }
#endif
    
    for (int i = 1; i < argc; ++i)
    {
        const String name = argv[1];
        String       value;
        if (Environment::Get(value, name))
        {
            std::cerr << "found '" << name << "' = '" << value << "'" << std::endl;
        }
        else
        {
            std::cerr << "no    '" << name << "'" << std::endl;

        }
    }
    //getFrom(data);

}
Y_UDONE()
