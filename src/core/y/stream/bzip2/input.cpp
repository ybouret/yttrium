
#include "y/stream/bzip2/input.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/readable/file.hpp"
#include "y/string.hpp"
#include "y/bzip2/bzlib.h"
#include <cstdio>
#include <cerrno>

namespace Yttrium
{
    namespace Bzip2
    {

        const char * const InputFile:: CallSign = "Bzip2::InputFile";

        class ReadableBZ2
        {
        public:
            explicit ReadableBZ2(FILE *fp) :
            bzerror(BZ_OK),
            bz( BZ2_bzReadOpen(&bzerror,fp, 0, 0, NULL, 0) ),
            done(false)
            {
                if (bzerror != BZ_OK) {
                    const char *msg = BZ2_bzerror(bz, &bzerror);
                    BZ2_bzReadClose(&bzerror,bz);
                    throw Specific::Exception("Bz2_bzReadOpen","'%s'",msg);
                }
            }

            virtual ~ReadableBZ2() noexcept
            {
                BZ2_bzReadClose(&bzerror,bz);
            }

            inline bool getc(char &C)
            {
                if(done) return false;

                bzerror = BZ_OK;
                BZ2_bzRead(&bzerror,bz,&C,1);

                switch(bzerror)
                {
                    case BZ_OK:
                        return true;

                    case BZ_STREAM_END:
                        done = true;
                        return false;

                    default:
                        break;
                }

                const char *msg = BZ2_bzerror(bz, &bzerror);
                throw Specific::Exception("Bz2_bzRead","'%s'",msg);

            }

        protected:
            int     bzerror;
            BZFILE *bz;
            bool    done;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadableBZ2);

        };
        
        class InputFile::Code :
        public Object,
        public Libc::ReadableFile,
        public ReadableBZ2
        {
        public:
            template <typename T> inline Code(const T &args) :
            Object(),
            Libc::ReadableFile(args),
            ReadableBZ2( static_cast<FILE *>(handle) ),
            io()
            {
            }

            inline virtual ~Code() noexcept
            {
            }

            inline bool query(char &C)
            {
                if(io.size>0)
                {
                    C = io.pullHead();
                    return true;
                }
                else
                    return getc(C);
            }

            inline bool ready()
            {
                char C = 0;
                if(getc(C)) io << C;
                return io.size>0;
            }


            IO::Chars io;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        InputFile:: InputFile(const char * fileName) :
        InputStream(),
        code( new Code(fileName) )
        {
        }

        InputFile:: InputFile(const String &fileName) :
        InputStream(),
        code( new Code(fileName) )
        {
        }

        InputFile:: ~InputFile() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }


        const char * InputFile:: callSign() const noexcept { return CallSign; }

        bool InputFile:: query(char &C)
        {
            assert(0!=code);
            return code->query(C);
        }

        void InputFile:: store(char C)
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
