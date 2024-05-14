
#include "y/chemical/plexus/boundary.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Boundary:: Boundary(const Species &s,
                      const xreal_t  x,
                      const SBank   &b) :
        Proxy<const SRepo>(),
        xi(x),
        sr(b)
        {
            (*this) << s;
        }

        Boundary:: ~Boundary() noexcept {}
        
        Boundary:: Boundary(const Boundary &other) :
        Proxy<const SRepo>(),
        xi(other.xi),
        sr(other.sr)
        {
        }
        
        Boundary::ConstInterface & Boundary:: surrogate() const noexcept { return sr; }

        std::ostream & operator<<(std::ostream &os, const Boundary &l)
        {
            os << '{';
            {
                const SNode *node=l.sr.head;
                if(node)
                {
                    os << (**node).name;
                }
                for(node=node->next;node;node=node->next)
                {
                    os << ',' << (**node).name;
                }
            }
            os << '}';
            os << '@' << real_t(l.xi);
            return os;
        }

        Boundary & Boundary:: operator<<(const Species &sp)
        {
            assert( !sr.has(sp) );
            sr << sp;
            return *this;
        }


    }

}
