
#include "y/chemical/plexus/janitor/broken.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Broken:: Broken(const Conservation::Law &l,
                        XWritable               &c) noexcept :
        gain(0),
        claw(l),
        cfix(c)
        {
        }

        Broken::  Broken(const Broken &b) noexcept :
        gain(b.gain),
        claw(b.claw),
        cfix(b.cfix)
        {

        }

        Broken:: ~Broken() noexcept {}

        int  Broken:: Compare(const Broken &lhs, const Broken &rhs) noexcept
        {
            return Comparison::Decreasing(lhs.gain, rhs.gain);
        }

        std::ostream & operator<<(std::ostream &os, const  Broken &self)
        {
            const xreal_t zero;
            os << real_t(self.gain) << " @" << (self.claw);
            if(self.gain>zero) { os << "->"; self.claw.displayCompact(os,self.cfix,SubLevel); }
            return os;
        }


        bool  Broken:: still(const XReadable &C, const Level L,  XAdd &xadd)
        {
            return claw.broken(gain, cfix, SubLevel, C, L, xadd);
        }

        
    }

}
