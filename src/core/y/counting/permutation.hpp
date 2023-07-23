
#ifndef Y_Counting_Permutation_Included
#define Y_Counting_Permutation_Included 1

#include "y/counting/schedule.hpp"
#include "y/apex/natural.hpp"

namespace Yttrium
{

    class Permutation : public Schedule
    {
    public:
        static const char * const CallSign;
        class Code;
        static apn Cardinal(const size_t n);

        explicit Permutation(const size_t n);
        virtual ~Permutation() noexcept;

        virtual size_t         size()                   const noexcept;
        virtual size_t         capacity()               const noexcept;
        virtual const size_t & operator[](const size_t) const noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Permutation);
        Code *code;

        virtual void doBoot() noexcept;
        virtual bool doNext() noexcept;
    };

}

#endif

