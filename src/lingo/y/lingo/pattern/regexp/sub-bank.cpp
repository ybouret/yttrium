
#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Pattern * RXC:: subBank()
        {
            assert(curr[-1]==LBRACK);
            const char * const start = curr;
            AutoPtr<Logic>     motif = 0;

            if(curr>=last) throw Specific::Exception(CallSign,"unfinished bank");
            switch(curr[0])
            {
                case ':':
                    return getPosix();

                default:
                    break;
            }



            throw Exception("Not Implemented");
            return 0;
        }

        Pattern * RXC:: getPosix()
        {
            assert(0!=curr);
            assert(curr<last);
            assert(curr[0]  == ':');
            assert(curr[-1] == LBRACK);
            const char * const start = &curr[-1];

            {
                // find next ':'
                while(++curr<last)
                {
                    if(':' == *curr)
                        goto FOUND_NEXT_COLON;

                }
                throw Specific::Exception(CallSign,"missing next ':' in posix bank'");

            FOUND_NEXT_COLON:
                // check following is RBRACK
                if(++curr>=last)      throw Specific::Exception(CallSign,"unfinished posix bank after '%s", start);
                if(RBRACK!=*(curr++)) throw Specific::Exception(CallSign,"invalid posix bank starting at '%s", start);
            }

            const String name(start+2,(curr-start)-4);
            return posix::named(name);
        }

    }

}

