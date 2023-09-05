
#include "y/stream/bzip2/input.hpp"
#include "y/system/exception.hpp"

#include <cstdio>
#include <cerrno>

namespace Yttrium
{
    namespace Bzip2
    {

        
        class InputFile::Code : public Object
        {
        public:
            explicit Code(const char *fileName)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };
    }
}
