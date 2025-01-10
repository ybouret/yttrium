

//! \file

#ifndef Y_Chemical_Marker_Included
#define Y_Chemical_Marker_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for multiple species at one given value
        //
        //
        //______________________________________________________________________
        class Marker : public Proxy<const SRepo>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Marker(const SBank & _sbank) noexcept; //!< setup
            virtual ~Marker()                     noexcept; //!< cleanup
            Marker(const Marker &);                         //!< duplicate
            Y_OSTREAM_PROTO(Marker);                        //!< dispay

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        protected:
            SRepo srepo; //!< repository
            xReal value; //!< shared value amongst species

        private:
            Y_DISABLE_ASSIGN(Marker);
            Y_PROXY_DECL();

        };
    }

}

#endif
