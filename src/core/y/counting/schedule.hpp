
#ifndef Y_Counting_Schedule_Included
#define Y_Counting_Schedule_Included 1

#include "y/counting/counting.hpp"
#include "y/container/readable.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! base class for counting as array of indices
    //
    //
    //__________________________________________________________________________
    class Schedule : public Counting, public Readable<size_t>
    {
    protected:
        explicit Schedule(const Cardinality) noexcept; //!< setup
    public:
        virtual ~Schedule() noexcept;                  //!< cleanup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Schedule);
    };

}

#endif
