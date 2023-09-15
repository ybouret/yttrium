#include "y/stream/xmlog.hpp"

namespace Yttrium
{

    XMLog:: ~XMLog() noexcept
    {
        leave(mark);
    }


    std::ostream & XMLog:: indent() const
    {
        for(unsigned i=deep;i>0;--i)
            std::cerr << "  ";
        return std::cerr;
    }

    std::ostream & XMLog:: operator()(void)
    {
        return indent();
    }

    void XMLog:: Markup:: incr()
    {
        Coerce(xml_.deep)++;
        xml_.enter(uid_);
    }

    XMLog:: Markup:: ~Markup() noexcept
    {
        xml_.leave(uid_);
        Coerce(xml_.deep)--;
    }
}
