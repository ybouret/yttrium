#include "y/jive/syntax/wildcard/repeat.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Syntax
        {

            static
            String *MakeRepeatName(const Rule &rr, const unsigned nmin)
            {
                AutoPtr<String> res = new String( *(rr.name) );
                switch(nmin)
                {
                    case 0: (*res) += '*'; break;
                    case 1: (*res) += '+'; break;
                    default:
                        (*res) += FormatString(">=%u",nmin);
                }
                return res.yield();
            }

            Repeat:: Repeat(const Rule &rr, const unsigned nmin) :
            Wildcard( MakeRepeatName(rr,nmin), UUID, rr),
            atLeast(nmin ){}

            Repeat:: ~Repeat() noexcept
            {
            }


            bool Repeat:: accepts(Y_JIVE_SYNTAX_RULE_API) const
            {
                
                return false;
            }
        }

    }

}

