
//! file

#ifndef Y_Chemical_Reactor_Included
#define Y_Chemical_Reactor_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Set or equilibria
        //
        //
        //______________________________________________________________________
        class Reactor : public Equilibrium::Set, public Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            static const char * const CallSign; //!< "Chemical::Equilibria"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Reactor();
            virtual ~Reactor() noexcept;


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void         mustInsert(const Equilibrium::Handle &); //!< insert/enroll

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XVector K; //!< syncronized constants

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };
    }

}

#endif

