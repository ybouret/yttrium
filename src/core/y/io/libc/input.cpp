
#include "y/io/libc/input.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"

#include <cstdio>
#include <cerrno>
#include <cstring>

namespace Yttrium
{

    namespace Libc
    {
        IsStdIn:: ~IsStdIn() noexcept
        {

        }

        IsStdIn:: IsStdIn() noexcept : flag(true) {}



        IsStdIn:: IsStdIn(const char *fileName) :
        flag(false)
        {
            if( 0 == fileName) throw Specific::Exception("Libc::InputFile","NULL file name");
            if( 0 == strcmp(Y_STDIN,fileName) )
            {
                Coerce(flag) = true;
            }
        }
    }

}

namespace Yttrium
{

    namespace Libc
    {

        InputFile:: ~InputFile() noexcept
        {
        }

        static inline void *openStdIn()
        {
            Y_GIANT_LOCK();
            FILE *fp = stdin;
            if(!fp) throw Libc::Exception(EIO,"closed stdin!");
            return fp;
        }

        InputFile:: InputFile(const StdIn_ &) :
        InputStream(),
        IsStdIn(),
        File(openStdIn(),false),
        buffer()
        {
            assert(true==flag);
        }

        static inline FILE * openCFILE(const char *fileName)
        {
            assert(0!=fileName);
            assert(0!=strcmp(fileName,Y_STDIN));

            std::cerr << "openCFILE(" << fileName << ")" << std::endl;
            Y_GIANT_LOCK();
            FILE *fp = fopen(fileName,"rb");
            if(!fp) throw Libc::Exception(errno,"fopen(%s)",fileName);
            return fp;

        }

        InputFile:: InputFile(const char *fileName) :
        InputStream(),
        IsStdIn(fileName),
        File( flag ? openStdIn() : openCFILE(fileName), !flag ),
        buffer()
        {

        }



        bool InputFile:: query(char &C)
        {

            if( buffer.size <= 0 )
            {
                FILE  *fp = static_cast<FILE*>(handle);
                if(feof(fp)) return false;

                size_t nr = 0;
                {
                    Y_GIANT_LOCK();;
                    nr = fread(buffer.entry,1,buffer.bytes,fp);
                    if(ferror(fp)) throw Libc::Exception(errno,"fread");
                }
                if(nr<=0)
                {
                    return false;
                }
                else
                {
                    C = buffer.entry[0];
                    for(size_t i=1;i<nr;++i)
                    {
                        assert(buffer.stock.size>0);
                        IO::Char *ch = buffer.stock.query();
                        **ch = buffer.entry[i];
                        buffer.pushTail(ch);
                    }
                    return true;
                }

            }
            else
            {
                IO::Char *ch = buffer.popHead();
                C = **ch;
                buffer.stock.store(ch);
                return true;
            }
        }


    }

}

