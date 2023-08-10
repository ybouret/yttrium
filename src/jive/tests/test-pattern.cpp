
#include "y/jive/pattern/all.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Jive;

Y_UTEST(pattern)
{
    AutoPtr<Pattern>  s = new Single('a');
    AutoPtr<Pattern>  a = new Any1();
    AutoPtr<Pattern>  r = new Range('0','9');
    AutoPtr<Compound> A = new And();
    AutoPtr<Optional> O = new Optional( *r );
    *A << *a << *r << new Exclude('k');

    {
        AutoPtr<Pattern> s1 = s->clone(); std::cerr << char(s1->as<Single>()->code) << std::endl;
        AutoPtr<Pattern> r1 = r->clone(); std::cerr << char(r1->as<Range>()->lower) << "," <<  char(r1->as<Range>()->upper) << std::endl;
        AutoPtr<Pattern> a1 = a->clone(); std::cerr << FourCC::ToText( a1->as<Any1>()->UUID ) << std::endl;
        AutoPtr<Pattern> A1 = A->clone();
        const String     Abin = A->toBinary();
        const String     A1bin = A1->toBinary();
        Y_CHECK(Abin == A1bin);
    }


}
Y_UDONE()

