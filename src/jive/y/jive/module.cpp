

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

#include "y/stream/libc/input.hpp"


namespace Yttrium
{
    namespace Jive
    {

        Module * Module:: OpenStdIn()
        {
            const Input   inp = new Libc::InputFile(StdIn);
            const Context ctx(Y_STDIN,AsTag);
            return new Module(ctx,inp);
        }

        Module * Module:: OpenFile(const char   *fileName)
        {
            const Input   inp = new Libc::InputFile(fileName);
            const Context ctx(fileName,AsTag);
            return new Module(ctx,inp);
        }

        Module * Module:: OpenFile(const String &fileName)
        {
            const Input   inp = new Libc::InputFile(fileName);
            const Context ctx(fileName,AsTag);
            return new Module(ctx,inp);
        }

    }

}

#include "y/stream/data/input.hpp"
#include "y/text/ops.hpp"


namespace Yttrium
{
    namespace Jive
    {
        Module * Module:: OpenData_(const Context &ctx, const void *addr, const size_t size)
        {
            const Input   inp = new InputDataStream(addr,size);
            return new Module(ctx,inp);
        }

        Module * Module:: OpenData_(const Context &ctx, const char *text)
        {
            std::cerr << "data: '" << text << "'" << std::endl;
            
            const Input   inp = new InputDataStream(text,StringLength(text));
            return new Module(ctx,inp);
        }
    }

}

#include "y/stream/char/input.hpp"


namespace Yttrium
{
    namespace Jive
    {
        Module * Module:: OpenData_(const Context &ctx, const IO::Chars &line)
        {
            const Input inp = new InputCharStream(line);
            return new Module(ctx,inp);
        }

    }

}


