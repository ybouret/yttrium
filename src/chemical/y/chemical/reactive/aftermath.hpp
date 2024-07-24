
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
             \param C pre-loaded concentration with ALL PRELOADED species
             \param L level to compute
             \param E Components part
             \param K precomputed constant of E
             \return false if blocked, true if exists, C is updated
             */
            bool solve(XWritable        &C,
                       const Level       L,
                       const Components &E,
                       const xreal_t     K);

            //! eval extent post solve
            xreal_t eval(XWritable       &  dOut,
                         const XReadable &  Cout,
                         const Level        Lout,
                         const XReadable &  Cinp,
                         const Level        Linp,
                         const Components & E);

            //! eval extent post solve
            xreal_t eval(const XReadable &  Cout,
                         const Level        Lout,
                         const XReadable &  Cinp,
                         const Level        Linp,
                         const Components & E);


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XMul xmul; //!< internal multiplications
            XAdd xadd; //!< internal additions

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
            

            
        };

    }

}

#endif

