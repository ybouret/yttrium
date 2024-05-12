
#include "y/chemical/reactive/plexus/limits.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Limits:: Limits(const BBank &bbank,
                        const SBank &sbank) noexcept :
        reac(bbank,sbank),
        prod(bbank,sbank)
        {
        }

        Limits:: Limits(const Limits &other) : reac(other.reac), prod(other.prod) {}

        Limits:: ~Limits() noexcept {}


        void Limits:: reset() noexcept { reac.reset(); prod.reset(); }

        unsigned Limits:: state() const noexcept {
            unsigned         flag  = USE_NONE;
            if(reac->size>0) flag |= USE_REAC;
            if(prod->size>0) flag |= USE_PROD;
            return flag;
        }

        void Limits:: outReac(std::ostream &os) const { os << "reac=" << reac; }
        void Limits:: outProd(std::ostream &os) const { os << "prod=" << prod; }

        std::ostream & operator<<(std::ostream &os, const Limits &self)
        {
            os << '(';
            switch(self.state())
            {
                case Limits:: USE_NONE: break;
                case Limits:: USE_REAC: self.outReac(os); break;
                case Limits:: USE_PROD: self.outProd(os); break;
                case Limits:: USE_BOTH: self.outReac(os); os << '|'; self.outProd(os); break;
            }
            os << ')';
            return os;
        }

    }

}

