#include "y/stream/xmlog.hpp"

namespace Yttrium
{

    XMLog:: XMLog(bool &globalVerbose) noexcept :
    verbose(globalVerbose),
    depth(0)
    {
    }
    
    XMLog:: ~XMLog() noexcept
    {
    }


    std::ostream & XMLog :: operator*() noexcept
    {
        return std::cerr;
    }

    std::ostream & XMLog:: operator()(void) {
        for(unsigned i=depth<<1;i>0;--i) std::cerr << ' ';
        return std::cerr;
    }


    void XMLog::Section::enter(const bool full)
    {
        if(xml.verbose)
        {
            xml() << LANGLE << sub;
            if(full) *xml <<  RANGLE << std::endl;
        }
        Coerce(xml.depth)++;
    }

    XMLog:: Section:: ~Section() noexcept
    {
        Coerce(xml.depth)--;
        try
        {
            if(xml.verbose) xml() << LANGLE << sub << SLASH << RANGLE << std::endl;
        }
        catch(...)
        {
        }
    }

}
