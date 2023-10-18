//! \file

#ifndef Y_Chemical_Conservation_Guardian_Included
#define Y_Chemical_Conservation_Guardian_Included 1

#include "y/chem/plexus.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Guardian of conservation laws
        //
        //
        //______________________________________________________________________
        class Guardian
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Guardian();          //!< setup
            virtual ~Guardian() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            const SpList & operator()(const Plexus    &plexus,
                                      Writable<xreal> &Corg,
                                      Writable<xreal> &Cerr,
                                      XMLog           &xml);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guardian);
            class Code;
            Code *code;

            void correct(const Canon     &canon,
                         Writable<xreal> &Corg,
                         Writable<xreal> &Cerr,
                         XMLog           &xml);

            void correct(const Cluster   &cluster,
                         Writable<xreal> &Corg,
                         Writable<xreal> &Cerr,
                         XMLog           &xml);

        };

    }

}

#endif
