
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/reactive/conservation/canon.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Cluster;

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
                explicit Laws(Cluster &, XMLog &); //!< setup
                virtual ~Laws() noexcept;          //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                virtual ConstInterface & surrogate() const noexcept;

                Canon canon; //!< internal list
            };


        }

    }


}

#endif

