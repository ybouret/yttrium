#include "y/stream/libc/input.hpp"
#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/string.hpp"
#include "y/ptr/auto.hpp"
#include "y/stream/gzip/input.hpp"
#include "y/stream/bzip2/input.hpp"

#include <cstring>

using namespace Yttrium;

namespace Yttrium
{

    class InputStreamProxy : public Object, public Counted, public Identifiable
    {
    protected:
        explicit InputStreamProxy() noexcept {}

    public:
        virtual ~InputStreamProxy() noexcept {}

        virtual InputStream * open(const String &) const = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputStreamProxy);
    };

    template <typename ISTREAM, const size_t N>
    class MagicInputStream : public InputStreamProxy
    {
    public:
        static const char * const CallSign;
        static const uint8_t Bytes[N];

        explicit MagicInputStream() noexcept {}
        virtual ~MagicInputStream() noexcept {}

        virtual const char *  callSign() const noexcept { return CallSign; }
        virtual InputStream * open(const String &fileName) const
        {
            uint8_t probe[N] = { 0 };
            memset(probe,0,sizeof(probe));
            if( !InputFile::Probe(fileName,probe,sizeof(probe)) )
            {
                std::cerr << "couldn't probe" << std::endl;
                return 0;
            }

            if( 0 != memcmp(Bytes,probe,N) )
            {
                std::cerr << "probe mismatch" << std::endl;
                return 0;
            }

            std::cerr << "Try Open" << std::endl;
            try
            {
                return new ISTREAM( fileName );
            }
            catch(...)
            {

            }
            std::cerr << "failure" << std::endl;
            return 0;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MagicInputStream);
    };

    typedef MagicInputStream<GZip::InputFile,2> MagicGZipFile;
    template<> const uint8_t      MagicGZipFile:: Bytes[] = { 0x1f, 0x8b };
    template<> const char * const MagicGZipFile:: CallSign = "GZip::InputStream";

    typedef MagicInputStream<Bzip2::InputFile,2> MagicBzip2File;
    template<> const uint8_t      MagicBzip2File:: Bytes[] = { 'B', 'Z' };
    template<> const char * const MagicBzip2File:: CallSign = "Bzip2::InputStream";


}

Y_UTEST(io_input_data)
{
    if(argc>1)
    {

        MagicGZipFile        mgz;
        MagicBzip2File       mbz2;

        AutoPtr<InputStream> fp = mgz.open(argv[1]);  if(fp.isValid()) std::cerr << "success!" << std::endl;
        fp = mbz2.open(argv[1]); if(fp.isValid()) std::cerr << "success!" << std::endl;

    }
}
Y_UDONE()

