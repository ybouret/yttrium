
#include "y/concurrent/engine/in0d.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            NoMapping:: ~NoMapping() noexcept {}
            NoMapping::  NoMapping() noexcept : activated(false) {}
            NoMapping:: NoMapping(const Int2Type<true> &) noexcept : activated(true) {}

            void NoMapping:: xch(NoMapping &other) noexcept
            {
                CoerceSwap(activated,other.activated);
            }

            std::ostream & operator<<(std::ostream &os, const NoMapping &self)
            {
                const char act[] = "activated";
                const char off[] = "lethargic";

                return os << "[" << (self.activated?act:off) << "]";
            }
        }


        Engine0D:: ~Engine0D() noexcept
        {

        }

        Engine0D:: Engine0D() noexcept
        {
            
        }

        void Engine0D:: start(const ThreadContext &cntx)
        {
            static const Int2Type<true> act = {};
            Mapping temp(act);
            this->initiate(cntx,temp);
        }

    }

}


