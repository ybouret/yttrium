
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/reactive/equilibrium/aftermath.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Solver
        {
        public:
            static const char * const CallSign;
            explicit Solver(const Mix &);
            virtual ~Solver() noexcept;

            void run(XMLog &xml, XWritable &C, const Level L, const XReadable &K);

            const Mix   & mix;
            XMatrix       Csolve;
            XMatrix       deltaC;
            Aftermath     aftermath;
            ProBank       pbank;
            ProList       plist;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
        };
    }

}

#endif

