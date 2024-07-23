
//! \file

#ifndef Y_Chemical_Squad_Included
#define Y_Chemical_Squad_Included 1

#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/data/pool/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Squad : public Quantized, public PList
        {
        public:
            typedef CxxListOf<Squad> List;
            typedef CxxPoolOf<Squad> Pool;

            explicit Squad(const PBank &) noexcept;
            virtual ~Squad()              noexcept;

            bool     accept(const Prospect     &pro,
                            const Matrix<bool> &attached) noexcept;


            Squad *next;
            Squad *prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Squad);
        };
    }

}

#endif

