
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/reactive/equilibrium/aftermath.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Prospect
        {
        public:
            Prospect(const Outcome &_out,
                      const xReal    _xi);
            Prospect(const Prospect &);
            ~Prospect() noexcept;

            const Outcome out;
            const xReal   xi;

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        typedef Small::CoopHeavyList<const Prospect> ProList;
        typedef ProList::NodeType                    ProNode;
        typedef ProList::ProxyType                   ProBank;


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

