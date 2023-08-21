
#include "y/concurrent/singulet.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/text/justify.hpp"
#include <cerrno>
#include <iostream>
#include <iomanip>

namespace Yttrium
{
    namespace Concurrent
    {

        bool Singulet:: Verbose = false;
        size_t Singulet:: Width = 32;

        Singulet:: ~Singulet() noexcept
        {
        }

        Singulet:: Singulet() noexcept
        {
        }


        void Singulet:: CheckLifeTime(const char             *whose,
                                      const AtExit::Longevity which)
        {
            if(which>=AtExit::MaximumLongevity)
            {
                if(!whose) whose = Core::Unknown;
                throw Specific::Exception(whose,"LifeTime cannot be Maxium Longevity!");
            }
        }

        void Singulet:: CheckInstance(const char *whose,
                                      void       *where)
        {
            if(!where)
            {
                if(!whose) whose = Core::Unknown;
                Libc::CriticalError(EINVAL, "No %s::Instance !!", whose);
            }
        }

        void Singulet:: OnInitDisplay(const char             *name,
                                      const AtExit::Longevity last) noexcept
        {
            if(!Verbose) return;
            try {
                assert(0!=name);
                std::cerr << "+[" << Justify(name,Width,Justify::Center) << "] @" << last << std::endl;
            }
            catch(...) {}
        }


        void Singulet:: OnQuitDisplay(const char             *name,
                                      const AtExit::Longevity last) noexcept
        {
            if(!Verbose) return;
            try {
                assert(0!=name);
                std::cerr << "~[" << Justify(name,Width,Justify::Center) << "] @" << last << std::endl;
            }
            catch(...) {}
        }

    }


}


