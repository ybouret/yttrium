#include "y/container/builder.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    namespace Core
    {
        class Builder:: Code : public Object
        {
        public:
            explicit Code() : Object() {}
            virtual ~Code() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };
    }
}

