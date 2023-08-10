
#include "y/io/gzip/input.hpp"
#include "y/string.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace GZip
    {
        InputFile:: ~InputFile() noexcept {}

        const char * const InputFile:: CallSign = "GZip::InputFile";
        
        const char * InputFile:: callSign() const noexcept
        {
            return CallSign;
        }

        static const char * CheckFileName(const char *fileName)
        {
            if(0==fileName) throw Specific::Exception(InputFile::CallSign,"NULL fileName");
            return fileName;
        }


        InputFile:: InputFile(const char *fileName) :
        fp( CheckFileName(fileName), "r" ),
        io()
        {
        }

    }

}

