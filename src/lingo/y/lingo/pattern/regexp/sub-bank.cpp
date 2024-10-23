
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


            // dispatch according to first char
            switch(curr[0])
            {
                case ':':
                    return getPosix();

                case '^':
                    motif = new None();
                    ++curr;
                    break;

                case '-':
                    motif = new Or();
                    motif->add('-');
                    ++curr;
                    break;

                default:
                    motif = new Or();
                    break;
            }

            assert(motif.isValid());

            // fill in
            while(curr<last)
            {
                const char c = *(curr++);
                switch(c)
                {
                    case RBRACK:
                        goto DONE;

                    default:
                        std::cerr << "grp " << c << std::endl;
                        motif->add(c);
                        break;
                }
            }

            throw Specific::Exception(CallSign, "unfinished bank after '%s'",start);

        DONE:
            if(0==motif->size) return new Any1();
            return motif.yield();
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

