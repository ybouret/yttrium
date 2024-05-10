
#include "y/chemical/reactive/plexus/limit.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Limit:: Limit(const Species &s,
                      const xreal_t  x,
                      const SBank   &b) :
        SRepo(b),
        extent(x)
        {
            (*this) << s;
        }

        Limit:: ~Limit() noexcept {}
        Limit:: Limit(const Limit &other) : SRepo(other), extent(other.extent) {}
        
        std::ostream & operator<<(std::ostream &os, const Limit &l)
        {
            const SRepo &repo = l;
            os << repo;
            os << '@' << real_t(l.extent);
            return os;
        }

    }

}
