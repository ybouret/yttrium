#include "y/chemical/reactive/equilibria/independence.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        Independence:: Independence(const size_t capacity) :
        bank(capacity),
        repo(bank)
        {
            
        }

        Independence:: ~Independence() noexcept
        {
        }
        


    }

}
