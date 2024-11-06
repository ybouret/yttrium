#include "y/lingo/syntax/xlist.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            XList:: ~XList() noexcept
            {
                std::cerr << "~XList" << std::endl;
            }

            XList:: XList() noexcept
            {
                std::cerr << "XList" << std::endl;
            }

            XNode:: ~XNode() noexcept
            {
                switch(type)
                {
                    case Terminal:
                        if(0!=unit) delete unit;
                        break;

                    case Internal:
                        Destruct( & _list() );
                        break;
                }
                _zero();
            }
        }

    }

}


