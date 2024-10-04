
#include "y/vfs/vfs.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    void VFS:: createSubDirs(const String &dirPath)
    {
        static const char sep = '/';
        Vector<String> dirs;
        Tokenizer::AppendTo(dirs, dirPath,sep); std::cerr << "dirs=" << dirs << std::endl;
        const size_t ndir = dirs.size();
        String       dirName;
        for(size_t i=1;i<=ndir;++i)
        {
            if(i>1) dirName += sep;
            dirName += dirs[i];
            std::cerr << "mkdir(" << dirName << ")" << std::endl;
            const Entry ep(*this,dirName);
            std::cerr << "    |_" << ep << std::endl;
            switch(ep.type)
            {
                case IsDir: continue;
                case IsReg: throw Specific::Exception("VFS::createSubDirs", "'%s' is a regular file", dirName.c_str());
                case IsUnk:
                    makeDirectory(dirName,true);
            }
        }

    }

    void VFS:: createSubDirs(const char * const dirPath)
    {
        const String _(dirPath);
        createSubDirs(_);
    }

}
