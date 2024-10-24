
#include "y/lingo/pattern/posix.hpp"
#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {
        Pattern * posix:: lower()
        {
            return new Lump('a','z');
        }

        Pattern * posix:: upper()
        {
            return new Lump('A','Z');
        }

        Pattern * posix:: alpha()
        {
            AutoPtr<Logic> p = new Or();
            *p << upper();
            *p << lower();
            return p.yield();
        }

        Pattern * posix:: digit()
        {
            return new Lump('0','9');
        }

        Pattern * posix:: alnum()
        {
            AutoPtr<Logic> p = new Or();
            *p << digit();
            *p << upper();
            *p << lower();
            return Pattern::Optimize(p.yield());
        }

        Pattern * posix:: word()
        {
            AutoPtr<Logic> p = new Or();
            p->add(  '_'  );
            *p << digit();
            *p << upper();
            *p << lower();
            return Pattern::Optimize(p.yield());
        }

        Pattern * posix:: xdigit()
        {
            AutoPtr<Logic> p = new Or();
            *p << digit();
            p->add('A','F');
            p->add('a','f');
            return Pattern::Optimize(p.yield());
        }

        Pattern * posix:: blank()
        {
            static const char data[] = " \t";
            return Pattern::Among(data);
        }

        Pattern * posix:: space()
        {
            return Pattern::Optimize( Pattern::Among(" \t\n\r\v\f"));
        }

        Pattern * posix::punct()
        {
            return Pattern::Optimize( Pattern::Among("][!\"#$%&'()*+,./:;<=>?@\\^_`{|}~-"));
        }

        Pattern * posix:: core()
        {
            AutoPtr<Logic> p = new Or();
            p->add( 0x20 );
            p->add( 0x21 );
            p->add( 0x23,0x26 );
            p->add( 0x28,0x5B );
            p->add( 0x5D,0x7F );
            return Pattern::Optimize( p.yield());
        }

        static const char __vowels__[] = "aeiouyAEIOUY";

        Pattern * posix:: vowel()
        {
            return Pattern::Among(__vowels__);
        }

        Pattern * posix:: consonant()
        {
            AutoPtr<Logic> p = new Or();
            for(int i='a';i<='z';++i)
            {
                if(strchr(__vowels__,i)) continue;
                p->add(i);
            }

            for(int i='A';i<='Z';++i)
            {
                if(strchr(__vowels__,i)) continue;
                p->add(i);
            }
            return Pattern::Optimize( p.yield());
        }

        static inline void __fill_endl( Patterns &ops )
        {
            ops.pushTail( new Byte('\n') );
            ops.pushTail( new Byte('\r') );
            ops.pushTail( Pattern::Exact("\r\n") );
        }

        Pattern * posix:: endl()
        {
            AutoPtr<Logic> p = new Or();
            __fill_endl(*p);
            return  Pattern::Optimize(p.yield());
        }

        Pattern * posix:: dot()
        {
            AutoPtr<Logic> p = new None();
            __fill_endl(*p);
            return  Pattern::Optimize(p.yield() );
        }

#define Y_Lingo_Posix(NAME) if(id == #NAME) return NAME()

        Pattern * posix:: named(const String &id)
        {
            Y_Lingo_Posix(lower);
            Y_Lingo_Posix(upper);
            Y_Lingo_Posix(alpha);
            Y_Lingo_Posix(digit);
            Y_Lingo_Posix(alnum);
            Y_Lingo_Posix(word);
            Y_Lingo_Posix(xdigit);
            Y_Lingo_Posix(blank);
            Y_Lingo_Posix(space);
            Y_Lingo_Posix(punct);
            Y_Lingo_Posix(core);
            Y_Lingo_Posix(vowel);
            Y_Lingo_Posix(consonant);
            Y_Lingo_Posix(endl);
            Y_Lingo_Posix(dot);
            throw Specific::Exception("posix expression", "no name '%s'", id.c_str());
        }

        Pattern * posix:: named(const char * const id)
        {
            const String _(id);
            return named(_);
        }

    }
}
