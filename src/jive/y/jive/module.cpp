

#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Module:: ~Module() noexcept
        {
        }

        Module:: Module(const Context &ctx, const Input &inp) noexcept :
        Context(ctx),
        input(inp)
        {
        }


        Jive::Char * Module:: get()
        {
            char C = 0;
            if(input->query(C))
            {
                Jive::Char *chr = new Char(*this,C);
                newChar();
                return chr;
            }
            else
            {
                return 0;
            }
        }


    }

}

#include "y/io/libc/input.hpp"


namespace Yttrium
{
    namespace Jive
    {

        Module * Module:: OpenStdIn()
        {
            const Input   inp = new Libc::InputFile(StdIn);
            const Context ctx = new String(Y_STDIN);
            return new Module(ctx,inp);
        }

        Module * Module:: OpenFile(const char   *fileName)
        {
            const Input   inp = new Libc::InputFile(fileName);
            const Context ctx = new String(fileName);
            return new Module(ctx,inp);
        }

        Module * Module:: OpenFile(const String &fileName)
        {
            const Input   inp = new Libc::InputFile(fileName);
            const Context ctx = new String(fileName);
            return new Module(ctx,inp);
        }

    }

}

#include "y/io/data/input.hpp"


namespace Yttrium
{
    namespace Jive
    {
        Module * OpenData_(const Context &ctx, const void *addr, const size_t size)
        {
            const Input   inp = new InputDataStream(addr,size);
            return new Module(ctx,inp);
        }

    }

}


