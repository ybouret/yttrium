

#include "y/stream/bzip2/output.hpp"
#include "y/stream/libc/writable/file.hpp"
#include "y/bzip2/bzlib.h"
#include "y/system/exception.hpp"
#include "y/string.hpp"

namespace Yttrium
{
    namespace Bzip2
    {

        class WritableBZ2
        {
        public:
            explicit WritableBZ2(FILE *fp, const int level) :
            bzerror(0),
            bz( BZ2_bzWriteOpen( &bzerror, fp, level, 0,0 ) )
            {
                if(BZ_OK != bzerror)
                {
                    const char *msg = BZ2_bzerror(bz, &bzerror);
                    BZ2_bzWriteClose(&bzerror,bz,1,0,0);
                    throw Specific::Exception("BZ2_bzWriteOpen","'%s'",msg);
                }
            }

            virtual ~WritableBZ2() noexcept
            {
                BZ2_bzWriteClose(&bzerror,bz,0,0,0);
            }

            void write(const char C)
            {
                bzerror = BZ_OK;
                BZ2_bzWrite(&bzerror,bz,(void*)&C,1);

                if(BZ_OK!=bzerror)
                {
                    const char *msg = BZ2_bzerror(bz, &bzerror);
                    BZ2_bzWriteClose(&bzerror,bz,1,0,0);
                    throw Specific::Exception("BZ2_bzWrite","'%s'",msg);
                }

            }

        protected:
            int     bzerror;
            BZFILE *bz;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(WritableBZ2);
        };

        class OutputFile:: Code :
        public Object,
        public Libc::WritableFile,
        public WritableBZ2
        {
        public:

            explicit Code(const char *filename, const int level ):
            Object(),
            Libc::WritableFile(filename,false),
            WritableBZ2( static_cast<FILE *>(handle), level )
            {

            }

            virtual ~Code() noexcept
            {

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        const char * const OutputFile:: CallSign = "Bzip2::OutputFile";

        OutputFile:: OutputFile(const char *fileName, const int level) :
        code( new Code(fileName,level) )
        {
        }

        OutputFile:: OutputFile(const String &fileName, const int level) :
        code( new Code(fileName(),level) )
        {
        }
        

        const char * OutputFile:: callSign() const noexcept {
            return CallSign;
        }

        OutputFile:: ~OutputFile() noexcept
        {
            assert(0!=code);
            delete code;
            code = 0;
        }

        void OutputFile:: write(const char C)
        {
            assert(0!=code);
            code->write(C);
        }

        void OutputFile:: flush()
        {
            // do nothing..
        }





    }
}
