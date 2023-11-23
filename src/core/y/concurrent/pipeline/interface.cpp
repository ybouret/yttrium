
#include "y/concurrent/pipeline/interface.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Pipeline:: ~Pipeline() noexcept
        {
        }

        Pipeline:: Pipeline() noexcept : jobID(0)
        {
        }

        void Pipeline:: upgrade() noexcept
        {
            static const JobID j1 = 1;
            Coerce(jobID) = Max<JobID>(j1,j1+jobID);
        }

#if 0
        JobID Pipeline:: push(Job *J)
        {
            assert(0!=J);
            try {
                suspend(); // no-throw
                const JobID ans = enqueue(J,jobID);
                restart(); // no-throw
                upgrade(); // no-throw
                return ans;
            }
            catch(...)
            {
                restart();
                throw;
            }

        }
#endif
        

    }

}

