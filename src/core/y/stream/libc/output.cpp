

#include "y/stream/libc/output.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"
#include "y/string.hpp"

#include <cstdio>
#include <cerrno>
#include <cstring>

namespace Yttrium
{

    namespace Libc
    {

        const char * const OutputFile::CallSign = "Libc::OutputFile";

        const char * OutputFile:: callSign() const noexcept { return CallSign; }

        OutputFile:: ~OutputFile() noexcept
        {
            try { emit(); } catch(...) {}
        }
        

        OutputFile:: OutputFile(const StdErr_ &_) :
        OutputStream(),
        WritableFile(_),
        buffer()
        {
        }


        OutputFile:: OutputFile(const StdOut_ &_) :
        OutputStream(),
        WritableFile(_),
        buffer()
        {
        }



        OutputFile:: OutputFile(const char *fileName, const bool append) :
        OutputStream(),
        WritableFile(fileName,append),
        buffer()
        {
        }

        OutputFile:: OutputFile(const String &fileName, const bool append) :
        OutputStream(),
        WritableFile(fileName,append),
        buffer()
        {
        }


        void OutputFile:: write(const char c)
        {
            assert(buffer.curr>=buffer.entry);
            if(buffer.curr>=buffer.last)
            {
                emit();
            }
            assert(buffer.curr<buffer.last);
            *(buffer.curr)++ = c;
        }

        void OutputFile:: flush()
        {
            emit();
        }

        void OutputFile:: emit()
        {
            const size_t toWrite = buffer.curr - buffer.entry;
            if(toWrite>0)
            {
                
                Y_GIANT_LOCK();
                const size_t nw = fwrite(buffer.entry,1,toWrite, static_cast<FILE*>(handle));
                if(nw<toWrite)
                {
                    const size_t remaining = toWrite-nw;
                    memmove(buffer.entry,buffer.entry+nw,remaining);
                    buffer.curr = buffer.entry + remaining;
                    throw Libc::Exception(errno,"fwrite()");
                }
                else
                {
                    buffer.curr = buffer.entry;
                }
            }
        }

        void OutputFile:: Overwrite(const Core::String<char> & filename)
        {
            const OutputFile here(filename,false);
        }

        void OutputFile:: Overwrite(const char *filename)
        {
            const OutputFile here(filename,false);
        }
    }

}


