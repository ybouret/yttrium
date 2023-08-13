
#include "y/jive/pattern/all.hpp"
#include "y/io/libc/output.hpp"
#include "y/io/libc/input.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

static inline void testPattern(Pattern &p)
{
    static unsigned I = 0;

    std::cerr << "-------- " << FourCC::ToText(p.uuid) << " --------" << std::endl;
    AutoPtr<Pattern> q = p.clone();
    Y_CHECK(q->uuid==p.uuid);
    {
        const String bin = p.toBinary();
        const String cln = q->toBinary();
        Y_CHECK(bin==cln);
    }
    Y_CHECK(p == *q);

    String fileName = FourCC::ToText(p.uuid); fileName += FormatString("%u.dat",++I);
    {
        Libc::OutputFile fp(fileName);
        p.serialize(fp);
    }

    {
        Libc::InputFile fp(fileName);
        AutoPtr<Pattern> r = Pattern::ReadFrom(fp);
        Y_CHECK(p == *r);
    }

    std::cerr << std::endl;

}

Y_UTEST(pattern)
{
    AutoPtr<Pattern>  s = new Single('a');      testPattern(*s);
    AutoPtr<Pattern>  a = new Any1();           testPattern(*a);
    AutoPtr<Pattern>  r = new Range('0','9');   testPattern(*r);
    AutoPtr<Pattern>  O = Optional::From( *r ); testPattern(*O);
    AutoPtr<Compound> A = new And();
    *A << *a << *r << new Exclude('k');         testPattern(*A);
    AutoPtr<Pattern>  R0 = Repeating::ZeroOrMore(*r);      testPattern(*R0);
    AutoPtr<Pattern>  R1 = Repeating::OneOrMore(new Range('a','z')); testPattern(*R1);
    AutoPtr<Pattern>  R4 = Repeating::Make(4, new Range('A','Z')); testPattern(*R4);


}
Y_UDONE()

