

#include "y/concurrent/pipeline/alone.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        Alone:: ~Alone() noexcept
        {
        }

        Alone:: Alone(const char *id) noexcept :
        sync(id),
        self(sync)
        {
        }


        const char * const Alone::CallSign = "Concurrent::Alone";

        const char * Alone:: callSign() const noexcept { return CallSign; }

        size_t Alone:: size() const noexcept
        {
            return 1;
        }

        Alone::ConstType & Alone:: operator[](const size_t
#ifndef NDEBUG
                                              indx
#endif
        ) const noexcept
        {
            assert(1==indx);
            return self;
        }




    }

}

