
#include "y/stream/gzip/input.hpp"
#include "y/string.hpp"
#include "y/system/exception.hpp"
#include "y/stream/gzip/file.hpp"

namespace Yttrium
{
    namespace GZip
    {
        const char * const InputFile:: CallSign = "GZip::InputFile";
        
        const char * InputFile:: callSign() const noexcept
        {
            return CallSign;
        }



        class InputFile:: Code : public Object
        {
        public:
            inline explicit Code(const char *fileName) :
            fp( CheckFileName(fileName), "r"),
            io() { }

            inline virtual ~Code() noexcept { }
            
            inline bool query(char &C)
            {
                if(io.size>0)
                {
                    C = io.pullHead();
                    return true;
                }
                else
                    return fp.getc(C);
            }

            
            inline bool ready()
            {
                char C=0;
                if(fp.getc(C)) io << C;
                return io.size>0;
            }

            File      fp;
            IO::Chars io;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);


            static const char * CheckFileName(const char *fileName)
            {
                if(0==fileName) throw Specific::Exception(InputFile::CallSign,"NULL fileName");
                return fileName;
            }
        };



        InputFile:: InputFile(const char * const fileName) :
        code( new Code(fileName) )
        {
        }


        InputFile:: InputFile(const String &fileName) :
        code( new Code(fileName.c_str()) )
        {
        }


        InputFile:: ~InputFile() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        bool InputFile:: query(char &C)
        {
            assert(0!=code);
            return code->query(C);
        }

        void InputFile:: store(const char C)
        {
            assert(0!=code);
            code->io << C;
        }

        bool InputFile:: ready()
        {
            assert(0!=code);
            return code->ready();
        }
        

    }

}

