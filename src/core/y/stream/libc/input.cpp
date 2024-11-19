
#include "y/stream/libc/input.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/string.hpp"

#include <cstdio>
#include <cerrno>
#include <cstring>



namespace Yttrium
{


    const char * const InputFile::CallSign = ":InputFile";

    const char * InputFile:: callSign() const noexcept { return CallSign; }

    InputFile:: ~InputFile() noexcept
    {
    }



    InputFile:: InputFile(const StdIn_ &_) :
    ReadableFile(_),
    buffer()
    {
    }

    InputFile:: InputFile(const char *fileName) :
    ReadableFile(fileName),
    buffer()
    {
    }

    InputFile:: InputFile(const String &fileName) :
    ReadableFile(fileName),
    buffer()
    {
    }



    bool InputFile:: query(char &C)
    {

        if( buffer.size <= 0 )
        {
            FILE  *fp = static_cast<FILE*>(handle);
            if(feof(fp)) return false;

            buffer.ready();
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
                if(nr>1)
                    buffer.bring(1,nr-1);
                return true;
            }

        }
        else
        {
            C = buffer.pluck();
            return true;
        }
    }

    void InputFile:: store(const char c)
    {
        buffer.unget(c);
    }

    bool InputFile:: ready()
    {
        if(buffer.size>0) return true;
        assert(buffer.size<=0);
        char C = 0;
        if(!query(C)) return false;
        buffer.unget(C);
        return true;
    }

    void InputFile:: gc() noexcept
    {
        buffer.prune();
    }

    bool InputFile:: probe(void * const addr, const size_t size)
    {
        assert(Good(addr,size));
        char * p = static_cast<char *>(addr);
        for(size_t i=size;i>0;--i)
        {
            if( !query( *(p++) ) )  return false;
        }
        return true;
    }



}

