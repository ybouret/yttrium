#include "y/type/auto-clean.hpp"

namespace Yttrium
{

    AutoClean:: Args:: ~Args() noexcept {}

    AutoClean:: ~AutoClean() noexcept {
        assert(0!=proc);
        proc(args);
    }
}
