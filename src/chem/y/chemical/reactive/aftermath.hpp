
//! \file

#ifndef Y_Chemical_Aftermath_Included
#define Y_Chemical_Aftermath_Included 1

#include "y/chemical/reactive/outcome.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        //______________________________________________________________________
        //
        //
        //
        //! Algorithm for single equilibrium outcome
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
            explicit Aftermath();
            virtual ~Aftermath() noexcept;


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! compute
            Outcome solve(const Components &eq,
                          const xReal       eK,
                          XWritable        &C1,
                          const Level       L1,
                          const XReadable  &C0,
                          const Level       L0);
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XMul        xmul;    //!< internal computation
            XAdd        xadd;    //!< internal computation
            const xReal zero;    //!< named constant

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
        };

    }

}

#endif

