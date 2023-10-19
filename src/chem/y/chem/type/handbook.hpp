//! \file

#ifndef Y_Chemical_Handbook_Included
#define Y_Chemical_Handbook_Included 1

#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Handbook : public Object, public AddressBook
        {
        public:
            explicit Handbook();
            virtual ~Handbook() noexcept;

            template <typename T> inline
            bool has(const T &obj) const noexcept { return 0 != search(obj); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Handbook);
        };

    }
}


#endif

