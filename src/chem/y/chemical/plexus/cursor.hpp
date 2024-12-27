
//! \file

#ifndef Y_Chemical_Cursor_Included
#define Y_Chemical_Cursor_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! one or more species at the same value
        class Cursor : public SRepo
        {
        public:
            explicit Cursor(const Species &_first,
                            const xReal    _value,
                            const SBank &  _sbank);
            virtual ~Cursor() noexcept;
            Cursor(const Cursor &);

            xReal &       operator*()       noexcept;
            const xReal & operator*() const noexcept;

        private:
            Y_DISABLE_ASSIGN(Cursor);
            xReal value;
        };
    }

}

#endif
