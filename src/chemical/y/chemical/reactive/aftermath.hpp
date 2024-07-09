
//! \file

#ifndef Y_Chemical_Aftermath_Included
#define Y_Chemical_Aftermath_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Solving individual
        //
        //
        //______________________________________________________________________
        class Aftermath
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Aftermath();          //!< setup
            virtual ~Aftermath() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
         
            //! solve single components at K
            /**
             \param Cout output concentrations
             \param Lout output level
             \param Cinp input concentrations
             \param Linp input level
             \param E    Components part of an equilibrium
             \param K    precomputed constant of E
             \return false if blocked, true if exists
             */
            bool solve(XWritable       &Cout,
                       const Level      Lout,
                       const XReadable &Cinp,
                       const Level      Linp,
                       const Components &E,
                       const xreal_t     K);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XMul xmul; //!< internal multiplications

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
            

            
        };

    }

}

#endif

