
//! \file

#ifndef Y_System_Progress_Included
#define Y_System_Progress_Included 1

#include "y/system/eta.hpp"

#include <iostream>

namespace Yttrium
{ 
    //__________________________________________________________________________
    //
    //
    //
    //! command line progress bar
    //
    //
    //__________________________________________________________________________
    class Progress
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char   Wheel[4];                               //!< for animation
        static const size_t Cycle = sizeof(Wheel)/sizeof(Wheel[0]); //!< for animation

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Progress();          //!< setup
        virtual ~Progress() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void start();                //!< initialize
        void finish(std::ostream &); //!< set ended display finish line

        //! compute status and display it
        template <typename T> inline
        void operator()(std::ostream &os, const T &istep, const T &total)
        {
            ended = istep >= total;
            show(os,eta(istep,total));
        }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        ETA    eta;    //!< time related
        size_t width;  //!< number of chars fir bar
        size_t cycle;  //!< cycle counter for animation
        bool   ended;  //!< completed or not

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Progress);
        void show(std::ostream &os, const double required);

    };


   
}

#endif

