
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

            bool solve(XWritable       &Cout,
                       const Level      out,
                       const XReadable &Cinp,
                       const Level      inp,
                       const Components &E,
                       const xreal_t     K);

            XMul xmul;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aftermath);
        };

    }

}

#endif

