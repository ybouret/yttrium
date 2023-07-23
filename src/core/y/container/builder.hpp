
#ifndef Y_Container_Builder_Included
#define Y_Container_Builder_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Core
    {

        class Builder
        {
        protected:
            class Code;

            explicit Builder();
        public:
            virtual ~Builder() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Builder);
            Code *code;
        };
    }
}

#endif

