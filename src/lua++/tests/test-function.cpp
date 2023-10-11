
#include "y/lua++/function.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

static const char code[] =
"function f(x, y)\n"
"return (x^2 * math.sin(y))/(1 - x)\n"
"end\n"
"function F(x)\n"
"return math.cos(3.14*x)\n"
"end\n"
;

Y_UTEST(function)
{
    Lua::VM vm = new Lua::State();
    vm->dostring(code);

    {
        Lua::Function<float>  Ff(vm,"F");
        Lua::Function<double> Fd(vm,"F");

        for(double x=0;x<=1.0;x+=0.1)
        {
            std::cerr << "F(" << x << ")=" << Ff(x) << ", " << Fd(x) << std::endl;
        }

    }


    {
        Lua::Function<float>  f(vm,"f");

        for(double x=0;x<=0.9;x+=0.1)
        {
            std::cerr << "x=" << x << ": ";
            for(double y=0;y<=3;y+=0.5)
            {
                std::cerr << " y=" << y << ":@" << f(x,y);
            }

            std::cerr << std::endl;

        }

    }



}
Y_UDONE()
