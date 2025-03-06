

//! \file

#ifndef Y_Chemical_Conservation_Authority_Included
#define Y_Chemical_Conservation_Authority_Included 1

#include "y/chemical/plexus/conservation/acts.hpp"
#include "y/chemical/plexus/conservation/laws.hpp"


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
            //! laws and their associated acts
            //
            //
            //__________________________________________________________________
            class Authority : public Oversized
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Authority(Mix &, XMLog &); //!< setup
                virtual ~Authority() noexcept;      //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Laws laws; //!< laws from mix topology
                const Acts acts; //!< organized acts

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Authority);
            };

        }

    }

}

#endif
