
#include "y/jive/pattern/all.hpp"
#include "y/io/libc/output.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

static inline void testPattern(Pattern &p)
{
    AutoPtr<Pattern> q = p.clone();
    Y_CHECK(p == *q);

    String fileName = FourCC::ToText(p.uuid); fileName += ".dat";
    {
        Libc::OutputFile fp(fileName);
        p.serialize(fp);
    }
    
}

Y_UTEST(pattern)
{
    AutoPtr<Pattern>  s = new Single('a');     testPattern(*s);
    AutoPtr<Pattern>  a = new Any1();          testPattern(*a);
    AutoPtr<Pattern>  r = new Range('0','9');  testPattern(*r);
    AutoPtr<Optional> O = new Optional( *r );  testPattern(*O);
    AutoPtr<Compound> A = new And();
    *A << *a << *r << new Exclude('k');
    AutoPtr<Guest>    R = new Repeating(0,*r);




}
Y_UDONE()

