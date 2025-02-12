

#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_sizes)
{
    std::cerr << "Basic Types" << std::endl;
    Y_SIZEOF(Natural);
    Y_SIZEOF(Integer);
    Y_SIZEOF(Rational);
    std::cerr << std::endl;

    std::cerr << "Ortho Types" << std::endl;
    Y_SIZEOF(Ortho::Vector);
    Y_SIZEOF(Ortho::Vector::Cache);
    Y_SIZEOF(Ortho::Family);

    std::cerr << std::endl;

}
Y_UDONE()
