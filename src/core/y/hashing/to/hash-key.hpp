
//! \file

#ifndef Y_HASHING_TO_HASH_KEY_INCLUDED
#define Y_HASHING_TO_HASH_KEY_INCLUDED 1


#include "y/hashing/to/key.hpp"

namespace Yttrium
{

    namespace Hashing
    {

        //______________________________________________________________________
        //
        //
        //! key hasher prototype
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class ToHashKey : public ToKey<size_t,FUNCTION>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit ToHashKey() noexcept : ToKey<size_t,FUNCTION>() {} //!< setup
            inline virtual ~ToHashKey() noexcept {}                            //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ToHashKey);
        };
    }

}

#endif
