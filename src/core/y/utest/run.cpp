#include "y/utest/run.hpp"
#include <cstring>
#include <iomanip>

namespace Yttrium
{

    size_t UnitTestDisplay::Width = 32;

    void UnitTestDisplay:: SizeOf(const char *className, const size_t classSize)
    {
        assert(0!=className);
        const size_t length = strlen(className);
        std::cerr << "sizeof(" << className << ")";
        for(size_t i=length;i<Width;++i) std::cerr << ' ';
        std::cerr << " = ";
        std::cerr << std::setw(6) << classSize << std::endl;
    }

    void UnitTestDisplay::AsU64(const char    *field,
                                const uint64_t value)
    {
        assert(0!=field);
        const size_t length = strlen(field);
        std::cerr << field;
        for(size_t i=length;i<Width+8;++i) std::cerr << ' ';
        std::cerr << " = ";
        std::cerr << std::setw(18) << value << std::endl;
    }

}

