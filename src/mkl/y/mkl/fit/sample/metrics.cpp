

#include "y/mkl/fit/sample/metrics.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            SampleMetrics:: SampleMetrics(const String &id) : name(id), indx(), vars()
            {

            }

            SampleMetrics:: SampleMetrics(const char *id) : name(id), indx(), vars()
            {
            }
            

            SampleMetrics:: ~SampleMetrics() noexcept
            {

            }

            const String & SampleMetrics:: key() const noexcept
            {
                return name;
            }

        }

    }
}
