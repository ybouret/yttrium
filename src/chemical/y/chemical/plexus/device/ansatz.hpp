
//! \file

#ifndef Y_Chemical_Device_Ansatz_Included
#define Y_Chemical_Device_Ansatz_Included 1

#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Ansatz for Device
        //
        //
        //______________________________________________________________________
        class Ansatz
        {
            public:  
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxSeries<Ansatz,XMemory> Series; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize from Aftermath computation
            Ansatz(const Equilibrium &_eq,
                   const xreal_t      _ek,
                   const Situation    _st,
                   XWritable &        _cc,
                   const xreal_t      _xi,
                   XWritable &        _dc) noexcept;

            //! duplicater for Series
            Ansatz(const Ansatz &) noexcept;

            //! cleanup
            ~Ansatz() noexcept;

            //! display
            Y_OSTREAM_PROTO(Ansatz);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            static SignType DecreasingAX(const Ansatz &lhs, const Ansatz &rhs) noexcept;         //!< decreasing |xi| for Crucial
            static SignType IncreasingFF(const Ansatz &lhs, const Ansatz &rhs) noexcept;         //!< increasing ff
            static SignType NaturalOrder(const Ansatz &lhs, const Ansatz &rhs) noexcept;         //!< increasing index
            
            xreal_t         objectiveFunction(XMul &X, const XReadable &C, const Level L) const; //!< affinity
            void            step(XSwell &inc) const; //!< append xi * nu to species's incresase

            const Equilibrium &eq; //!< equilibrium
            const xreal_t      ek; //!< its constant
            const Situation    st; //!< its state
            XWritable &        cc; //!< optimal concentrations
            xreal_t            xi; //!< corresponding extent from Cini
            xreal_t            ax; //!< |xi|
            XWritable &        dc; //!< corresponding delta fron Cini
            xreal_t            ff; //!< objective function at cc
            bool               ok; //!< decrease

        private:
            Y_DISABLE_ASSIGN(Ansatz);

        };

    }

}

#endif
