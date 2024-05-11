
//! \file

#ifndef Y_Chemical_Limit_Included
#define Y_Chemical_Limit_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/type/proxy.hpp"
namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Limit = extent with one or more species
        //
        //
        //______________________________________________________________________
        class Limit : public Proxy<const SRepo>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! record first species with its extent
            explicit Limit(const Species &s,
                           const xreal_t  x,
                           const SBank   &bank);
            virtual ~Limit() noexcept; //!< cleanup
            Limit(const Limit &other); //!< full copy
            Y_OSTREAM_PROTO(Limit);    //!< display list+xi
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Limit & operator<<(const Species &); //!< append another species

            //__________________________________________________________________
            //
            //
            //  Members
            //
            //__________________________________________________________________
            const xreal_t xi; //!< extent

        private:
            Y_DISABLE_ASSIGN(Limit);
            virtual ConstInterface & surrogate() const noexcept;
            SRepo sr;
        };

       

    }

}

#endif
