

#include "y/check/claim.hpp"
#include <cstring>
#include <iostream>

namespace Yttrium
{

    size_t Claim::Width = 64;

    void   Claim:: XRaise(const char *expr, const int line)
    {
        assert(NULL!=expr);
        Exception excp;
        excp << Core::Failure << ':' << '[' << expr << ']';
        excp.add(" @line %d", line);
        throw excp;
    }

    void Claim:: Prolog(const char *expr)
    {
        assert(0!=expr);
        const size_t size = strlen(expr);
        std::cerr << '[' << expr << ']';
        for(size_t i=size;i<Width;++i) std::cerr << ' ';
    }

    void Claim:: Epilog(const bool flag)
    {
        std::cerr << '[' << (flag? Core::Success : Core::Failure) << ']' << std::endl;
    }
}

