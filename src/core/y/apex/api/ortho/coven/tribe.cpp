#include "y/apex/api/ortho/coven/tribe.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {

                SignType Tribe:: Compare(const Tribe * const lhs, const Tribe * const rhs) noexcept
                {
                    return Natural::Compare(lhs->qfamily->weight(),rhs->qfamily->weight());
                }



                void Tribe:: replaceFamilyByFamilyOf(Tribe &better) noexcept
                {
                    assert(qfamily!=better.qfamily);
                    destroy();
                    (qfamily=better.qfamily)->withhold();
                }

                void Tribe:: destroy() noexcept {
                    assert( 0 != qfamily );
                    if( qfamily->liberate() ) qfcache->store(qfamily);
                    qfamily = 0;
                }

                Tribe:: ~Tribe() noexcept
                {
                    destroy();
                }

                std::ostream & operator<<(std::ostream &os, const Tribe &tribe)
                {
                    const Posture &posture = tribe;
                    os << posture << "->" << *(tribe.qfamily);
                    return os;
                }

                const char * const Tribe:: CallSign = "Coven::Tribe";

                void Tribe:: throwDegenerate() const
                {
                    throw Specific::Exception(CallSign, "Unexpected %s Family!", Family::QualityText(Degenerate) );
                }
            }

        }
    }
}

