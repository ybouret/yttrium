
#include "y/chemical/plexus/warden/frontier.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Frontier:: Frontier(const SBank &sbank,
                            const xreal_t  x,
                            const Species &s) :
        SProxy(sbank),
        xi(x)
        {
            sr << s;
        }
        
        Frontier:: Frontier(const Frontier &F) :
        SProxy(F),
        xi(F.xi)
        {
        }

        Frontier:: ~Frontier() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Frontier &self)
        {
            assert(self.sr.size>0);
            os << real_t(self.xi) << "@" << self.sr;
            return os;
        }


        Frontier & Frontier:: operator<<(const Species &s)
        {
            sr << s;
            return *this;
        }

    }

}


