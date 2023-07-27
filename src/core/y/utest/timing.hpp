
//! \file

#ifndef Y_UnitTest_Timing_Included
#define Y_UnitTest_Timing_Included 1

#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"


namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! using WallTime to compute Timings
    //
    //
    //__________________________________________________________________________
    class Timing : public WallTime
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit Timing();            //!< setup
        virtual ~Timing() noexcept;   //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void         reset() noexcept;           //!< ticks=0, cycle=0
        uint64_t     speed() const noexcept;     //!< ops / s
        long double  probe() const noexcept;     //!< ticks to seconds
        Timing     & renew(const uint64_t mark); //!< updated ticks and cyle

        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        uint64_t        ticks; //!< cumulative ticks
        size_t          cycle; //!< over the cycle index

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Timing);
    };
}

//______________________________________________________________________________
//
//
//! update TMX using CODE during DURATION
//
//______________________________________________________________________________
#define Y_Timing_Add(TMX,CODE,DURATION) do {     \
/**/    const double   __len__ = (DURATION);     \
/**/    const uint64_t __ini__ = TMX.Ticks();    \
/**/    uint64_t       __ell__ = 0;              \
/**/    do {                                     \
/**/        TMX.cycle++;                         \
/**/        do { CODE; } while(false);           \
/**/        __ell__ = TMX.Ticks() - __ini__;     \
/**/    } while( TMX(__ell__) < __len__ );       \
/**/    TMX.ticks += __ell__;                    \
} while(false)

//__________________________________________________________________________
//
//
//! full CODE sampling during DURATION
//
//__________________________________________________________________________
#define Y_Timing(TMX,CODE,DURATION) do {              \
/**/    TMX.reset(); Y_Timing_Add(TMX,CODE,DURATION); \
} while(false)



#endif

