#include "y/ink/format.hpp"
#include <cctype>


namespace Yttrium
{
    namespace Ink
    {

        Format:: ~Format() noexcept
        {
        }
        

        bool Format:: matches(const String &ext) const noexcept
        {
            return matches( ext.c_str(), ext.size() );
        }

        bool Format:: matches(const char * const ext) const noexcept
        {
            return matches( ext, Core::Length(ext) );
        }

        const String & Format:: key() const noexcept
        {
            return name;
        }

        bool Format:: CaseInsensitiveMatch(const char * const lhs, const size_t lsz,
                                           const char * const rhs, const size_t rsz) noexcept
        {
            assert( Good(lhs,lsz) );
            assert( Good(rhs,rsz) );

            if(lsz!=rsz) return false;

            for(size_t i=0;i<lsz;++i)
            {
                const int l = tolower(lhs[i]);
                const int r = tolower(rhs[i]);
                if(l!=r) return false;
            }
            
            return true;
        }

    }

}
