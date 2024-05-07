#include "y/ink/image/format.hpp"
#include "y/vfs/vfs.hpp"
#include <cctype>


namespace Yttrium
{
    namespace Ink
    {

        Format:: ~Format() noexcept
        {
        }
        

        bool Format:: matches(const String &path)
        {
            const char * const fileName = VFS::BaseName(path);if(!fileName) return false;
            const char *       ext      = VFS::Extension(path);
            if(!ext++) return false;
            return 0 != extension.exactly(fileName,ext);
        }


        std::ostream & operator<<(std::ostream &os, const Format &fmt)
        {
            return os << '"' << fmt.key() << '"';
        }

        String Format:: LowerCaseExt(const String &path)
        {
            String res;
            const char * ext = VFS::Extension(path);
            if(!ext++) return res;

            while( '\0' != *ext )
            {
                res += tolower(*ext);
                ++ext;
            }

            return res;
        }




    }

}
