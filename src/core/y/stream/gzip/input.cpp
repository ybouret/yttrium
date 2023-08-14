
#include "y/stream/gzip/input.hpp"
#include "y/string.hpp"
#include "y/system/exception.hpp"
#include "y/stream/gzip/file.hpp"
#include "y/stream/libc/buffer/cached.hpp"
#include "y/type/utils.hpp"

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
            fp( CheckFileName(fileName), "r'"),
            io(),
            nb( unsigned(Min<size_t>(io.bytes, IntegerFor<unsigned>::Maximum) ) )
            {
            }

            inline virtual ~Code() noexcept
            {
            }

            
            File               fp;
            Libc::CachedBuffer io;
            const unsigned     nb;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
            static const char * CheckFileName(const char *fileName)
            {
                if(0==fileName) throw Specific::Exception(InputFile::CallSign,"NULL fileName");
                return fileName;
            }

            bool load()
            {
                assert(io.size<=0);
                io.ready();
                const unsigned nr = fp.read(io.entry,nb);
                if(nr>0)
                {
                    for(unsigned i=0;i<nr;++i)
                    {
                        io.pushTail( new IO::Char(io.entry[i]) );
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        InputFile:: InputFile(const char *fileName) :
        code( new Code(fileName) )
        {
        }

        InputFile:: ~InputFile() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }


    }

}

