
#include "y/memory/exclusive/studio.hpp"
#include "y/utest/run.hpp"
#include "y/memory/exclusive/decl.hpp"
#include "y/memory/exclusive/impl.hpp"

using namespace Yttrium;

namespace
{
    class XChar
    {
    public:

        XChar *  next;
        XChar *  prev;
        char     data;

        inline explicit XChar() noexcept : next(0), prev(0), data(0) {}
        inline virtual ~XChar() noexcept {}
        inline explicit XChar(const char c) noexcept : next(0), prev(0), data(c) {}

        char operator*() const noexcept { return data; }

        Y_EXCLUSIVE_DECL();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XChar);
    };


}

Y_Studio(XChar,8,1024);

namespace
{

    Y_EXCLUSIVE_IMPL(XChar);


}

#include "y/data/list/cxx.hpp"

Y_UTEST(memory_studio)
{
    Studio<XChar> &S = Studio<XChar>::Single::Instance();
    {
        CxxListOf<XChar> chars;

        chars.pushTail(new XChar('h'));
        chars.pushTail(new XChar('e'));
        chars.pushTail(new XChar('l'));
        chars.pushTail(new XChar('l'));
        chars.pushTail(new XChar('o'));
        chars.pushTail(new XChar());
        std::cerr << chars << std::endl;
        S.displayInfo(0);
    }

    S.displayInfo(0);
    S.gc(100);
    S.displayInfo(0);




}
Y_UDONE()
