
//! \file

#ifndef Y_Apex_Jigs_Included
#define Y_Apex_Jigs_Included 1

#include "y/apex/block/jig.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        // Global Definitions
        //
        //
        //______________________________________________________________________
        typedef Jig<Plan1> Jig1; //!< alias
        typedef Jig<Plan2> Jig2; //!< alias
        typedef Jig<Plan4> Jig4; //!< alias
        typedef Jig<Plan8> Jig8; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! set of jigs
        //
        //
        //______________________________________________________________________
        class Jigs {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t JigSize = sizeof(Jig1);             //!< size for any Jig
            static const size_t Measure = JigAPI::Plans * JigSize;  //!< memory for all Jigs

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup jigs from shared memory
            explicit Jigs(void * const entry,  const size_t range) noexcept;

            //! cleanup jigs
            virtual ~Jigs() noexcept;


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! return chosen Jig
            template <Plan PLAN> inline
            Jig<PLAN> & as() noexcept {
                return *(Jig<PLAN> *) &addr[JigSize*PLAN];
            }

            //! return chosen Jig, const
            template <Plan PLAN> inline
            const Jig<PLAN> & as() const noexcept {
                return *(const Jig<PLAN> *) &addr[JigSize*PLAN];
            }

            //! API selection
            JigAPI & operator[](const Plan plan) noexcept;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jigs);
            char * const addr;
            void *       wksp[ Y_WORDS_GEQ(Measure) ];
        };


    }

}

#endif

