
//! \file

#ifndef Y_Chemical_Equilibria_Batch_Included
#define Y_Chemical_Equilibria_Batch_Included 1

#include "y/chemical/reactive/equilibria/fragment.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{
    namespace Chemical
    {

     

        //______________________________________________________________________
        //
        //
        //
        //! Batch of coupled equilibria and species
        //
        //
        //______________________________________________________________________
        class Batch : public Oversized, public Fragment
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Batch> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Batch(const Equilibrium &first); //!< fill with first equilibrium
            virtual ~Batch() noexcept;                //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool sharesSpeciesWith(const Equilibrium &) const noexcept; //!< check if species are shared
            bool sharesSpeciesWith(const Batch       &) const noexcept; //!< check if species are shared
            void finalize();

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Batch           *next;    //!< for list
            Batch           *prev;    //!< for list
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Batch);
            void prepareSpecies();

        };


    }

}

#endif

