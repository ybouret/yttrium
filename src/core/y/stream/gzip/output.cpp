

#include "y/stream/gzip/output.hpp"
#include "y/string.hpp"
#include "y/system/exception.hpp"
#include "y/stream/gzip/file.hpp"
#include "y/type/utils.hpp"
#include <cstring>

namespace Yttrium
{
    namespace GZip
    {
        const char * const OutputFile:: CallSign = "GZip::InputFile";

        const char * OutputFile:: callSign() const noexcept
        {
            return CallSign;
        }

        namespace
        {
            class WriteMode
            {
            public:
                inline explicit WriteMode(const int level, const bool append) noexcept :
                mode()
                {
                    memset(mode,0,sizeof(mode));
                    mode[0] = 'w';
                    mode[1] = 'b';
                    mode[2] = '0' + level;
                    if(append)
                        mode[3] = 'a';
                }

                inline virtual ~WriteMode() noexcept
                {
                    memset(mode,0,sizeof(mode));

                }

                char mode[8];
            private:
                Y_DISABLE_COPY_AND_ASSIGN(WriteMode);
            };
        }


        static const char * CheckFileName(const char *fileName)
        {
            if(0==fileName)
                throw Specific::Exception(OutputFile::CallSign,"NULL fileName");
            
            return fileName;
        }

        class OutputFile :: Code :
        public Object,
        public WriteMode,
        public File
        {
        public:

            inline explicit Code(const char *fileName,
                                 const int   level,
                                 const bool  append) :
            Object(),
            WriteMode(level,append),
            File( CheckFileName(fileName), mode )
            {

            }

            virtual ~Code() noexcept
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        OutputFile:: ~OutputFile() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        OutputFile:: OutputFile(const char *fileName,
                                const int   level,
                                const bool  append) :
        code( new Code(fileName,level,append) )
        {}


        OutputFile:: OutputFile(const String &fileName,
                                const int   level,
                                const bool  append) :
        code( new Code(fileName(),level,append) )
        {}

        void OutputFile:: write(const char c)
        {
            assert(0!=code);
            code->putc(c);
        }

        void OutputFile:: flush() noexcept
        {

        }

    }

}



