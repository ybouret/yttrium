
#ifndef Y_Counting_Schedule_Included
#define Y_Counting_Schedule_Included 1

#include "y/counting/counting.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{
    
    class Schedule : public Counting, public Readable<size_t>
    {
    protected:
        explicit Schedule(const Cardinality) noexcept;

    public:
        virtual ~Schedule() noexcept;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Schedule);
    };

}

#endif
