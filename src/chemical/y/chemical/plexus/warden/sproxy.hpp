
//! \file

#ifndef Y_Chemical_Warden_SProxy_Included
#define Y_Chemical_Warden_SProxy_Included 1

#include "y/chemical/species.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! proxy to an internal dynamic repository of species
        //
        //
        //______________________________________________________________________
        class SProxy : public Proxy<const SRepo>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SProxy(const SBank &sbank) noexcept; //!< setup
            virtual ~SProxy() noexcept;                   //!< cleanup
            explicit SProxy(const SProxy &);              //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            SProxy & operator<<(const SProxy &); //!< append content

        protected:
            SRepo sr; //!< internal repo

        private:
            Y_DISABLE_ASSIGN(SProxy);
            virtual ConstInterface & surrogate() const noexcept { return sr; }
        };
    }

}

#endif

