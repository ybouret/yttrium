
#include "y/stream/libc/writable/output-grasp.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"

#include <cstdio>
#include <cerrno>
#include <cstring>

namespace Yttrium
{
    namespace Libc
    {
        OutputGrasp:: ~OutputGrasp() noexcept
        {

        }


        OutputGrasp:: OutputGrasp(const StdErr_ &) noexcept :
        isErr(true),
        isOut(false),
        isReg(false)
        {

        }

        OutputGrasp:: OutputGrasp(const StdOut_ &) noexcept :
        isErr(false),
        isOut(true),
        isReg(false)
        {

        }

        OutputGrasp:: OutputGrasp(const char *fileName):
        isErr(false),
        isOut(false),
        isReg(true)
        {
            if( 0 == fileName) throw Specific::Exception("Libc::OutputFile","NULL file name");

            if( 0 == strcmp(fileName,Y_STDERR) )
            {
                Coerce(isErr) = true;
                Coerce(isReg) = false;
                return;
            }

            if( 0 == strcmp(fileName,Y_STDOUT) )
            {
                Coerce(isOut) = true;
                Coerce(isReg) = false;
                return;
            }

        }

        void * OutputGrasp:: openErr()
        {
            Y_GIANT_LOCK();
            FILE *fp = stderr;
            if(!fp) throw Libc::Exception(EIO,"closed stderr!");
            return fp;
        }

        void * OutputGrasp:: openOut()
        {
            Y_GIANT_LOCK();
            FILE *fp = stdout;
            if(!fp) throw Libc::Exception(EIO,"closed stdout!");
            return fp;
        }


        void * OutputGrasp:: openFile(const char *fileName,
                                      const bool  append)
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

    }

}


