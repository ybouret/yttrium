

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

#if 0
        static inline void *openErr()
        {
            Y_GIANT_LOCK();
            FILE *fp = stderr;
            if(!fp) throw Libc::Exception(EIO,"closed stderr!");
            return fp;
        }

        static inline void *openOut()
        {
            Y_GIANT_LOCK();
            FILE *fp = stdout;
            if(!fp) throw Libc::Exception(EIO,"closed stdout!");
            return fp;
        }


        void * OutputGrasp:: openFile(const char *fileName,
                                      const bool append)
        {
            assert(0!=fileName);

            if(isErr)
            {
                assert(!isOut);
                assert(!isReg);
                assert(0==strcmp(fileName,Y_STDERR));
                return openErr();
            }

            if(isOut)
            {
                assert(!isErr);
                assert(!isReg);
                assert(0==strcmp(fileName,Y_STDOUT));
                return openOut();
            }

            assert(isReg);
            assert(!isErr);
            assert(!isOut);

            const char *mode = append ? "ab" : "wb";
            Y_GIANT_LOCK();
            FILE *fp = fopen(fileName,mode);
            if(!fp)
            {
                throw Libc::Exception(errno,"fopen(%s)",fileName);
            }
            return fp;
        }
#endif
        
        OutputFile:: OutputFile(const StdErr_ &_) :
        OutputStream(),
        OutputGrasp(_),
        Libc::File( openErr(), false),
        buffer()
        {
        }


        OutputFile:: OutputFile(const StdOut_ &_) :
        OutputStream(),
        OutputGrasp(_),
        Libc::File( openOut(), false),
        buffer()
        {
        }



        OutputFile:: OutputFile(const char *fileName, const bool append) :
        OutputStream(),
        OutputGrasp(fileName),
        Libc::File( openFile(fileName,append), isReg),
        buffer()
        {
        }

        OutputFile:: OutputFile(const String &fileName, const bool append) :
        OutputStream(),
        OutputGrasp(fileName()),
        Libc::File( openFile(fileName(),append), isReg),
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


    }

}


