
//! \file

#ifndef Y_Chemical_Aftermath_Included
#define Y_Chemical_Aftermath_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Aftermath
        {
        public:
            explicit Aftermath();
            virtual ~Aftermath() noexcept;

            //!
            /**
             \param Cout output concentrations, already filled, transformed
             \param Lout output level
             \param Cinp input concentrations
             \param Linp input level
             \param E    Components part of an equilibrium
             \param K    precomputed constant of E
             */
            bool solve(XWritable       &Cout,
                       const Level      Lout,
                       const XReadable &Cinp,
                       const Level      Linp,
                       const Components &E,
                       const xreal_t     K);

            XMul xmul;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
            

            
        };

    }

}

#endif

