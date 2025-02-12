
#include "y/apex/api/ortho/metrics.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {

            const char * Metrics:: QualityText(const Quality quality) noexcept
            {
                switch(quality)
                {
                    case Degenerate: return "Degenerate";
                    case Generating: return "Generating";
                    case Hyperplane: return "Hyperplane";
                    default: break;
                }
                assert(Fragmental==quality);
                return "Fragmental";
            }

            Metrics:: Metrics(const size_t n) noexcept :
            dimensions(n),
            concluding(dimensions-1)
            {
                assert(dimensions>0);
            }

            Metrics:: ~Metrics() noexcept
            {
            }

            Metrics:: Metrics(const Metrics &_) noexcept :
            dimensions(_.dimensions),
            concluding(_.concluding)
            {
            }

            Quality Metrics:: getQuality(const size_t n) const noexcept
            {
                if(n<=0) return Degenerate;

                assert(n<=dimensions);
                if(n>=dimensions)
                    return Generating;
                
                if(n>=concluding)
                    return Hyperplane;

                return  Fragmental;

            }

        }

    }
}

