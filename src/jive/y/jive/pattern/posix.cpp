
#include "y/jive/pattern/posix.hpp"
#include "y/jive/pattern/all.hpp"

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
            return Pattern::Among(" \t\n\r\v\f");
        }

        Pattern * posix::punct()
        {
            return Pattern::Among("][!\"#$%&'()*+,./:;<=>?@\\^_`{|}~-");
        }

#if 0
        static inline void __fill_endl( patterns &ops )
        {
            ops.push_back( new single('\n') );
            ops.push_back( new single('\r') );
            ops.push_back( logical::equal("\r\n") );
        }

        pattern * posix:: endl()
        {
            auto_ptr<op_or> p = new op_or();
            __fill_endl(*p);
            return  pattern::optimize(p.yield());
        }

        pattern * posix:: dot()
        {
            auto_ptr<op_none> p = new op_none();
            __fill_endl(*p);
            return  pattern::optimize(p.yield());
        }

        pattern * posix:: core()
        {
            auto_ptr<op_or> p = new op_or();
            p->add( 0x20 );
            p->add( 0x21 );
            p->add( 0x23,0x26 );
            p->add( 0x28,0x5B );
            p->add( 0x5D,0x7F );
            return pattern::optimize( p.yield() );
        }

        static const char __vowels__[] = "aeiouyAEIOUY";

        pattern * posix:: vowel()
        {
            return logical::among(__vowels__);
        }

        pattern * posix:: consonant()
        {
            return logical::avoid(__vowels__);
        }
#endif

    }
}
