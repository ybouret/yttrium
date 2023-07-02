
#include "y/text/plural.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace
{
    static inline void display(const char  *text,
                               const size_t count)
    {
        std::cerr << "I have " << count << " " << text << Plural::s(count) << std::endl;
    }
}


Y_UTEST(text_plural)
{
    display("car",0);
    display("pen",1);
    display("guitar",2);

}
Y_UDONE()
