

#include "y/field/meta-key/hasher.hpp"

namespace Yttrium
{
    namespace Field
    {

        MetaKeyHasher:: MetaKeyHasher() noexcept : h() {}

        MetaKeyHasher:: ~MetaKeyHasher() noexcept
        {
        }

        size_t  MetaKeyHasher::operator()(const MetaKey &mk) noexcept
        {
            h.set();
            {
                const unsigned dim = mk.size();
                for(unsigned i=0;i<dim;++i)
                {
                    const String &s = mk[i];
                    h.run(s.ro_addr(),s.measure());
                }
            }
            size_t ans = 0;
            h.get(&ans, sizeof(ans));
            return ans;
        }


    }

}
