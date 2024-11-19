#include "y/stream/libc/input.hpp"
#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/string.hpp"
#include "y/ptr/auto.hpp"
#include "y/stream/gzip/input.hpp"
#include "y/stream/bzip2/input.hpp"
#include <cstring>
#include "y/type/proxy.hpp"

using namespace Yttrium;

namespace Yttrium
{

    class InputStreamFormat : public Object,  public Identifiable
    {
    public:
        typedef CxxListOf<InputStreamFormat> List;

    protected:
        explicit InputStreamFormat() noexcept : next(0), prev(0) {}

    public:
        virtual ~InputStreamFormat() noexcept {}

        virtual InputStream * open(const String &) const = 0;

        InputStreamFormat *next;
        InputStreamFormat *prev;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(InputStreamFormat);
    };

    class FallbackInputStream : public InputStreamFormat
    {
    public:

        explicit FallbackInputStream() noexcept {}
        virtual ~FallbackInputStream() noexcept {}

        virtual const char * callSign() const noexcept { return "InputFile"; }
        virtual InputStream * open(const String &fileName) const
        {
            return new InputFile(fileName);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(FallbackInputStream);
    };

    template <typename ISTREAM, const size_t N>
    class MagicInputStream : public InputStreamFormat
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
    template<> const uint8_t      MagicGZipFile:: Bytes[]  = { 0x1f, 0x8b };
    template<> const char * const MagicGZipFile:: CallSign = "GZip::InputStream";

    typedef MagicInputStream<Bzip2::InputFile,2> MagicBzip2File;
    template<> const uint8_t      MagicBzip2File:: Bytes[]  = { 'B', 'Z' };
    template<> const char * const MagicBzip2File:: CallSign = "Bzip2::InputStream";


    class MagicInputFactory : public Proxy<const InputStreamFormat::List>
    {
    public:
        explicit MagicInputFactory() : crew() {

        }

        virtual ~MagicInputFactory() noexcept   {}

        template <typename MAGIC_FILE> inline
        void add()
        {
            crew.pushHead( new MAGIC_FILE() );
        }

        InputStream * create(const String &fileName) const noexcept
        {
            for(const InputStreamFormat *fmt=crew.head;fmt;fmt=fmt->next)
            {
                InputStream * const fp = fmt->open(fileName);
                if(0!=fp) return fp;
            }

            return fallback.open(fileName);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MagicInputFactory);
        InputStreamFormat::List crew;
        FallbackInputStream     fallback;

        virtual ConstInterface & surrogate() const noexcept { return crew; }
    };




}

Y_UTEST(io_input_data)
{
    MagicInputFactory mif;
    mif.add<MagicGZipFile>();
    mif.add<MagicBzip2File>();

    if(argc>1)
    {





    }
}
Y_UDONE()

