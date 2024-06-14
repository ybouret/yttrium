
//! \file

#ifndef Y_System_Progress_Included
#define Y_System_Progress_Included 1

#include "y/system/eta.hpp"

#include <iostream>

namespace Yttrium
{ 
    class Progress
    {
    public:
        static const char   Wheel[4];
        static const size_t Cycle = sizeof(Wheel)/sizeof(Wheel[0]);

        explicit Progress();
        virtual ~Progress() noexcept;

        ETA    eta;
        size_t width;
        size_t cycle;

        void start();
        void finish(std::ostream &);

        template <typename T> inline
        void operator()(std::ostream &os, const T &istep, const T &total)
        {
            show(os,eta(istep,total));
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Progress);
        void show(std::ostream &os, const double required);

    };


   
}

#endif

