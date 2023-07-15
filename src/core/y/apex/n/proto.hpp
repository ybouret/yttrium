//! \file

#ifndef Y_Apex_Natural_Proto_Included
#define Y_Apex_Natural_Proto_Included 1

#include "y/object.hpp"
#include "y/apex/number.hpp"
#include "y/apex/m/block.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{
    namespace Apex
    {

        template <typename CORE_TYPE, typename WORD_TYPE>
        class Proto : public Object
        {
        public:
            static const unsigned                        CoreSize = sizeof(CORE_TYPE);
            static const unsigned                        WordSize = sizeof(WORD_TYPE);
            typedef typename UnsignedInt<CoreSize>::Type CoreType;
            typedef typename UnsignedInt<WordSize>::Type WordType;
            typedef Block<WordType>                      Block;

            inline explicit Proto() : data(0)
            {
                Y_STATIC_CHECK(WordSize<CoreSize,InvalidMetrics);
            }

            inline virtual ~Proto() noexcept
            {}

            Block data;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Proto);
        };

    }

}

#endif

