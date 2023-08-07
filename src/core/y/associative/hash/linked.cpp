#include "y/associative/hash/linked.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Core
    {

        void HashLinked::UnexpectedInsertFailure(const char *which)
        {
            assert(0!=which);
            throw Specific::Exception(which,"unexpected insert failure!!");
        }
    }

}
