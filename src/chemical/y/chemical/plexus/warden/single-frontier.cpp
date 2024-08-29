
#include "y/chemical/plexus/warden/single-frontier.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        SingleFrontier:: SingleFrontier(const SBank &sbank) noexcept :
        SProxy(sbank),
        xi()
        {
        }


        SingleFrontier:: ~SingleFrontier() noexcept { }

        std::ostream & operator<<(std::ostream &os, const SingleFrontier &self)
        {
            os << "lim=";
            if(self->size<=0)
                os << "none";
            else
                os << real_t(self.xi) << "@" << self.sr;
            return os;
        }


        void  SingleFrontier:: free() noexcept
        {
            sr.free();
            Coerce(xi).ldz();
        }


        void SingleFrontier::  operator()(const xreal_t  x,
                                          const Species &s)
        {
            assert(x>=0.0);
            try {
                if(sr.size<=0)
                {
                    first(x,s);
                }
                else
                {
                    switch( Sign::Of(x,xi) )
                    {
                        case Negative: // new smallest
                            free();
                            first(x,s);
                            break;

                        case __Zero__: // same value
                            sr << s;
                            break;


                        case Positive: // discard
                            break;

                    }
                }
            }
            catch(...)
            {
                free();
                throw;
            }
        }

        
        bool SingleFrontier:: blocking() const noexcept
        {
            assert(xi.mantissa>=0);
            return sr.size > 0 && xi.mantissa <= 0;
        }

        void SingleFrontier:: first(const xreal_t x, const Species &s)
        {
            assert(0==sr.size);
            sr << s;
            Coerce(xi) = x;
        }


    }
}


