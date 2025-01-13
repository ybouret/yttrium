
//! \file

#ifndef Y_Apex_Jigs_Included
#define Y_Apex_Jigs_Included 1

#include "y/apex/jig.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Apex
    {

        typedef Jig<Plan1> Jig1;
        typedef Jig<Plan2> Jig2;
        typedef Jig<Plan4> Jig4;
        typedef Jig<Plan8> Jig8;


        class Jigs {
        public:
            static const size_t JigSize = sizeof(Jig1);
            static const size_t Measure = JigAPI::Plans * JigSize;

            explicit Jigs(void * const entry,  const size_t range) noexcept :
            addr(0),
            wksp()
            {
                Coerce(addr) = static_cast<char *>( Memory::OutOfReach::Addr( &wksp[0] ) );
                char  *p = addr;
                new (p)            Jig1(entry,range);
                new (p += JigSize) Jig2(entry,range);
                new (p += JigSize) Jig4(entry,range);
                new (p += JigSize) Jig8(entry,range);
            }

            template <Plan PLAN> inline
            Jig<PLAN> & as() noexcept {
                return *(Jig<PLAN> *) &addr[JigSize*PLAN];
            }

            template <Plan PLAN> inline
            const Jig<PLAN> & as() const noexcept {
                return *(const Jig<PLAN> *) &addr[JigSize*PLAN];
            }


            JigAPI & operator[](const Plan plan) noexcept {
                switch(plan) {
                    case Plan1: break;
                    case Plan2: return as<Plan2>();
                    case Plan4: return as<Plan4>();
                    case Plan8: return as<Plan8>();
                }
                return as<Plan1>();
            }

            virtual ~Jigs() noexcept
            {

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jigs);
            char * const addr;
            void *       wksp[ Y_WORDS_GEQ(Measure) ];
        };


    }

}

#endif

