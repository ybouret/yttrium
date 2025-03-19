//! \file

#ifndef Y_Chemical_Aftermath_Included
#define Y_Chemical_Aftermath_Included 1

#include "y/chemical/reactive/outcome.hpp"
#include "y/mkl/root/zbis.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //
        //! Solving ONE equilibrium
        //
        //
        //______________________________________________________________________
        class Aftermath
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef MKL::ZBis<xreal_t> RooType; //!< choice of algorithm

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Aftermath() noexcept;
            virtual ~Aftermath() noexcept;



            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! study and solve state if possible
            /**
             \param E   components
             \param K   constant
             \param C   working  concentration (=C0)
             \param L   working  level
             \param C0  starting point
             \param L0  starting level
             \return fully populated Outcome
             */
            Outcome operator()(const Equilibrium & E,
                               const xreal_t       K,
                               XWritable &         C,
                               const Level         L,
                               const XReadable &   C0,
                               const Level         L0);


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XMul               xmul; //!< for internal multiplications
            XAdd               xadd; //!< for internal additions
            RooType            root; //!< 1D root algorithm
            const xreal_t      zero; //!< zero value

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);

            // one cycle improvement
            xreal_t improve(const Components &E,
                            const xreal_t     K,
                            XWritable        &C,
                            const Level       L);
        };

    }

}

#endif

