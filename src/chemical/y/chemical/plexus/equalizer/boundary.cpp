#include "y/chemical/plexus/equalizer/boundary.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Boundary::  Boundary(const SBank &_) noexcept :
        SRepo(_),
        xi(0)
        {
        }

        Boundary:: Boundary(const SBank   & b,
                            const xreal_t   x,
                            const Species & s)   :
        SRepo(b),
        xi(x)
        {
            (*this) << s;
        }


        Boundary:: Boundary(const Boundary &other) :
        SRepo(other),
        xi(other.xi)
        {
        }

        Boundary:: ~Boundary() noexcept {}

        void Boundary:: empty() noexcept
        {
            free();
            xi = 0;
        }

        void Boundary:: first(const xreal_t x, const Species &s)
        {
            assert(0==size);
            (*this) << s;
            xi = x;
        }

        std::ostream & operator<<(std::ostream &os, const Boundary &B)
        {
            const SRepo &repo = B;
            if(repo.size>0)
            {
                os << repo << "@" << real_t(B.xi);
            }
            else
            {
                os << "none";
            }
            return os;
        }


        void Boundary:: operator()(const xreal_t x,
                                   const Species &s)
        {
            try
            {
                if(size<=0)
                {
                    first(x,s);
                }
                else
                {
                    assert(x>=0.0);
                    switch( Sign::Of(x,xi) )
                    {
                        case Negative: // new winner
                            free();
                            first(x,s);
                            break;

                        case __Zero__: // same value
                            (*this) << s;
                            break;

                        case Positive: // discard
                            break;
                    }
                }
            }
            catch(...)
            {
                empty();
                throw;
            }
        }
    }

}
