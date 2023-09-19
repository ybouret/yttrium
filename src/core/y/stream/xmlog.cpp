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

    std::ostream & XMLog:: Indent(std::ostream &os, const unsigned n)
    {
        for(unsigned i=n<<1;i>0;--i) os << ' ';
        return os;
    }

    std::ostream & XMLog:: indent(std::ostream &os) const
    {
        return Indent(os,depth);
    }


    std::ostream & XMLog:: operator()(void) {
        return indent(std::cerr);
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
