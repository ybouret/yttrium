
#include "y/jive/pattern/all.hpp"
#include "y/io/libc/output.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

static inline void testPattern(Pattern &p)
{
    std::cerr << "-------- " << FourCC::ToText(p.uuid) << " --------" << std::endl;
    AutoPtr<Pattern> q = p.clone();
    Y_CHECK(q->uuid==p.uuid);
    {
        const String bin = p.toBinary();
        const String cln = q->toBinary();
        Y_CHECK(bin==cln);
    }
    Y_CHECK(p == *q);

    String fileName = FourCC::ToText(p.uuid); fileName += ".dat";
    {
        Libc::OutputFile fp(fileName);
        p.serialize(fp);
    }
    std::cerr << std::endl;

}

Y_UTEST(pattern)
{
    AutoPtr<Pattern>  s = new Single('a');     testPattern(*s);
    AutoPtr<Pattern>  a = new Any1();          testPattern(*a);
    AutoPtr<Pattern>  r = new Range('0','9');  testPattern(*r);
    AutoPtr<Optional> O = new Optional( *r );  testPattern(*O);
    AutoPtr<Compound> A = new And();
    *A << *a << *r << new Exclude('k');        testPattern(*A);
    AutoPtr<Guest>    R = new Repeating(0,*r); testPattern(*R);




}
Y_UDONE()

