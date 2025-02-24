
#include "y/osprey/tribe.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        const char * const Tribe::CallSign = "Osprey::Tribe";


        Tribe:: ~Tribe() noexcept { destroy(); }


        std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
        {
            os << tribe.posture << " -> " << *tribe.qfamily;
            return os;
        }

        void Tribe:: destroy() noexcept
        {
            assert(0!=qfamily);
            if(qfamily->liberate())
                qfcache->store(qfamily);
            qfamily = 0;
        }

        void Tribe:: throwDegenerateFamily(const Apex::Ortho::Quality q) const
        {
            throw Specific::Exception(CallSign,"Unfolding %s family!", QVector::QualityText(q));
        }


        void Tribe:: replaceFamilyBy(const Tribe &equivalent) noexcept
        {
            destroy();
            (qfamily=equivalent.qfamily)->withhold();
        }
    }

}


