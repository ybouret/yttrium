#include "y/text/cxx-identifier.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(text_cxx_id)
{
    const String s = CxxIdentifier::From("<Hello< double>>");
    std::cerr << s << std::endl;
}
Y_UDONE()

