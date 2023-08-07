
#include "y/jive/tags.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;


Y_UTEST(tags)
{
    Jive::Tag tag = new String("Hello");
    std::cerr << tag << std::endl;
}
Y_UDONE()

