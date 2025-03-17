//! \file

#ifndef Y_Chemical_Conservation_Rule_Included
#define Y_Chemical_Conservation_Rule_Included 1

#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            //__________________________________________________________________
            //
            //
            //
            //! Internal Rule to build Laws
            //
            //
            //__________________________________________________________________
            class Rule : public Quantized, public Actors
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Rule() noexcept; //!< setup
                virtual ~Rule() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Rule *next; //!< for list
                Rule *prev; //!< for list
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
            };

        }

    }

}
#endif
