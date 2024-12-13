
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/plexus/conservation/canon.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Mix;

        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            //! Laws for a cluster
            //
            //
            //__________________________________________________________________
            class Laws : public Object, public Proxy<const Canon>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Chemical::Conservation::Laws"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Laws(Mix &, XMLog &); //!< setup
                virtual ~Laws() noexcept;      //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                virtual ConstInterface & surrogate() const noexcept;
                Canon my; //!< internal list
            };


        }

    }


}

#endif

