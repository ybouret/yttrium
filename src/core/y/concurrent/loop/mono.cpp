
#include "y/concurrent/loop/mono.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        Mono:: ~Mono() noexcept
        {
        }

        Mono:: Mono(const char *id) noexcept :
        access(id),
        context(access)
        {
        }


        const char * const Mono::CallSign = "Concurrent::Mono";

        const char * Mono:: callSign() const noexcept { return CallSign; }

        size_t Mono:: size() const noexcept
        {
            return 1;
        }

        Mono::ConstType & Mono:: operator[](const size_t
#ifndef NDEBUG
                                            indx
#endif
                                            ) const noexcept
        {
            assert(1==indx);
            return context;
        }


        void Mono:: operator()(Kernel &kernel) noexcept
        {
            try 
            {
                kernel(context);
            }
            catch(...)
            {

            }
        }

    }

}

