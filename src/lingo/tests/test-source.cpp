#include "y/lingo/source.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Lingo;

namespace
{
    static const char msg[] = "Hello, World!";
}

Y_UTEST(source)
{
    AutoPtr<Lingo::Module> m;
    if(argc>1)
    {
        m = Lingo::Module::OpenFile(argv[1]);
    }
    else
    {
        m = Lingo::Module::OpenData("message", msg);
    }
    Y_ASSERT(m.isValid());
    Lingo::Source source( m.yield() );
    Y_ASSERT(m.isEmpty());

    Lingo::Token token;
    while( source.ready() ) token.pushTail( source.get() );

    std::cerr << token.toPrintable() << std::endl;

    for(const Lingo::Char *ch=token.head;ch;ch=ch->next)
    {
        std::cerr << ch->line << ':' << ch->column << ':' << ch->printable() << std::endl;
    }

}
Y_UDONE()
