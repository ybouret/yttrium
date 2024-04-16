
#include "y/ink/image/format.hpp"
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

        FormatBMP::  FormatBMP() : Format("BMP","(bmp)&") {}
        FormatBMP:: ~FormatBMP() noexcept {}

        

    }
}

Y_UTEST(format_bmp)
{
    Ink::FormatOptions opts;

    Ink::Format::Handle bmp = new Ink::FormatBMP();

    {
        Y_CHECK(bmp->matches("hello.bmp"));
        Y_CHECK(bmp->matches("Hello.BmP"));
    }

}
Y_UDONE()

