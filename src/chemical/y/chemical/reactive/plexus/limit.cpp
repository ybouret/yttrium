
#include "y/chemical/reactive/plexus/limit.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Limit:: Limit(const Species &s,
                      const xreal_t  x,
                      const SBank   &b) :
        Proxy<const SRepo>(),
        xi(x),
        sr(b)
        {
            (*this) << s;
        }

        Limit:: ~Limit() noexcept {}
        Limit:: Limit(const Limit &other) :
        Proxy<const SRepo>(),
        xi(other.xi),
        sr(other.sr)
        {
        }
        
        Limit::ConstInterface & Limit:: surrogate() const noexcept { return sr; }

        std::ostream & operator<<(std::ostream &os, const Limit &l)
        {
            os << l.sr;
            os << '@' << real_t(l.xi);
            return os;
        }

        Limit & Limit:: operator<<(const Species &sp)
        {
            assert( !sr.has(sp) );
            sr << sp;
            return *this;
        }


    }

}
