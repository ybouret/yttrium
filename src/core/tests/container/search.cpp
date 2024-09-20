

#include "y/container/algo/search.hpp"
#include "y/utest/run.hpp"

#include "y/string.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;


Y_UTEST(container_search)
{

    Vector<size_t> output;
    const String   source = "GCATCGCAGAGAGTATACAGTACG";
    const String   figure = "GCAGAGAG";

    Algo::Search(output,source,figure);

    std::cerr << "output: " << output << std::endl;

}
Y_UDONE()
