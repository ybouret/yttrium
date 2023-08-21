
#include "y/jive/pattern/posix.hpp"
#include "y/jive/pattern/all.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Jive
    {
        Pattern * posix:: lower()
        {
            return new Range('a','z');
        }

        Pattern * posix:: upper()
        {
            return new Range('A','Z');
        }

        Pattern * posix:: alpha()
        {
            AutoPtr<Compound> p = new Or();
            *p << upper();
            *p << lower();
            return   p.yield();
        }

        Pattern * posix:: digit()
        {
            return new Range('0','9');
        }

        Pattern * posix:: alnum()
        {
            AutoPtr<Compound> p = new Or();
            *p << digit();
            *p << upper();
            *p << lower();
            return Pattern::Optimize( p.yield() );
        }

        Pattern * posix:: word()
        {
            AutoPtr<Compound> p = new Or();
            p->add(  '_'  );
            *p << digit();
            *p << upper();
            *p << lower();
            return Pattern::Optimize( p.yield() );
        }

        Pattern * posix:: xdigit()
        {
            AutoPtr<Compound> p = new Or();
            *p << digit();
            p->add('A','F');
            p->add('a','f');
            return Pattern::Optimize( p.yield() );
        }

        Pattern * posix:: blank()
        {
            static const char data[] = " \t";
            return Pattern::Among(data);
        }

        Pattern * posix:: space()
        {
            return Pattern::Optimize( Pattern::Among(" \t\n\r\v\f") );
        }

        Pattern * posix::punct()
        {
            return Pattern::Optimize( Pattern::Among("][!\"#$%&'()*+,./:;<=>?@\\^_`{|}~-") );
        }

        Pattern * posix:: core()
        {
            AutoPtr<Compound> p = new Or();
            p->add( 0x20 );
            p->add( 0x21 );
            p->add( 0x23,0x26 );
            p->add( 0x28,0x5B );
            p->add( 0x5D,0x7F );
            return Pattern::Optimize( p.yield() );
        }

        static const char __vowels__[] = "aeiouyAEIOUY";

        Pattern * posix:: vowel()
        {
            return Pattern::Among(__vowels__);
        }

        Pattern * posix:: consonant()
        {
            AutoPtr<Compound> p = new Or();
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
            return Pattern::Optimize( p.yield() );
        }

        static inline void __fill_endl( Patterns &ops )
        {
            ops.pushTail( new Single('\n') );
            ops.pushTail( new Single('\r') );
            ops.pushTail( Pattern::Exact("\r\n") );
        }

        Pattern * posix:: endl()
        {
            AutoPtr<Compound> p = new Or();
            __fill_endl(p->patterns);
            return  Pattern::Optimize(p.yield());
        }

        Pattern * posix:: dot()
        {
            AutoPtr<Compound> p = new None();
            __fill_endl(p->patterns);
            return  Pattern::Optimize(p.yield());
        }




    }
}
