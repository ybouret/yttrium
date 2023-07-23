//! \file

#ifndef Y_Counting_Included
#define Y_Counting_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    typedef uint64_t Cardinality;

    class Counting
    {

    protected:
        explicit Counting(const Cardinality) noexcept;

    public:
        virtual ~Counting() noexcept;

        const Cardinality  index;
        const Cardinality  total;

        void boot() noexcept;
        bool next() noexcept;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Counting);
        virtual void doBoot() noexcept = 0;
        virtual bool doNext() noexcept = 0;
    };

}


#endif

