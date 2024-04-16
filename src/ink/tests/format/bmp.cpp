
#include "y/ink/format.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Ink
    {
        class FormatBMP : public Format
        {
        public:
            explicit FormatBMP();
            virtual ~FormatBMP() noexcept;
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatBMP);
            bool matches(const char * const ext, const size_t len) const noexcept;
        };

        FormatBMP::  FormatBMP() : Format("BMP") {}
        FormatBMP:: ~FormatBMP() noexcept {}

        bool FormatBMP:: matches(const char * const ext, const size_t len) const noexcept
        {
            assert(Good(ext,len));
            static const char mine[] = "bmp";
            return CaseInsensitiveMatch(ext, len, mine, sizeof(mine)/sizeof(mine[0])-1);
        }

    }
}

Y_UTEST(format_bmp)
{
    Ink::FormatOptions opts;

    Ink::Format::Handle bmp = new Ink::FormatBMP();


}
Y_UDONE()

