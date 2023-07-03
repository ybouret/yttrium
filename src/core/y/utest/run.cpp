#include "y/utest/run.hpp"
#include <cstring>
#include <iomanip>

namespace Yttrium
{

    size_t SizeOf::Width = 32;

    void SizeOf::Display(const char *className, const size_t classSize)
    {
        assert(className);
        const size_t length = strlen(className);
        std::cerr << "sizeof(" << className << ") = ";
        for(size_t i=length;i<Width;++i) std::cerr << ' ';
        std::cerr << std::setw(6) << classSize << std::endl;
    }

}

