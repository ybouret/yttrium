
//! \file

#ifndef Y_Chemical_Cursor_Included
#define Y_Chemical_Cursor_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! one or more species at the same value
        //
        //
        //______________________________________________________________________
        class Cursor : public SRepo
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with first species/value and SBank
            explicit Cursor(const Species &_first,
                            const xReal    _value,
                            const SBank &  _sbank);
            virtual ~Cursor() noexcept; //!< cleanup
            Cursor(const Cursor &);     //!< duplicate
            Y_OSTREAM_PROTO(Cursor);    //!< dispay

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            xReal &       operator*()       noexcept; //!< access
            const xReal & operator*() const noexcept; //!< access, const

        private:
            Y_DISABLE_ASSIGN(Cursor);
            xReal value;
        };
    }

}

#endif
