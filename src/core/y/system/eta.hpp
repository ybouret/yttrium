
//! \file

#ifndef Y_System_ETA_Included
#define Y_System_ETA_Included 1

#include "y/system/wtime.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Estimated Time of Arrival
    //
    //
    //__________________________________________________________________________
    class ETA
    {
    public:

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit ETA();
        virtual ~ETA() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        
        //! restart
        void start();


        //! compute (clamped) ETA from current infornation
        /**
         \param istep number of step (should be >0)
         \param total total number of steps
         \return estimated time to complete, in seconds
         */
        template <typename T>
        inline double operator()(const T &istep,
                                 const T &total)
        {

            getEllapsed();
            Coerce(fraction) = static_cast<double>( static_cast<long double>(istep) / total );
            return getEstimate();
        }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        WallTime             wallTime; //!< internal wall time
        const uint64_t       iniTicks; //!< initial ticks
        const uint64_t       nowTicks; //!< probed ticks
        const double         fraction; //!< fraction of work
        const double         ellapsed; //!< computed while update


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ETA);
        void   getEllapsed();
        double getEstimate() const noexcept;
    };

}

#endif

