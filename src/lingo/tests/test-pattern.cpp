

#include "y/lingo/pattern/all.hpp"
#include "y/lingo/pattern/char-db.hpp"
#include "y/stream/libc/input.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Lingo;


namespace
{
    static inline void process(Pattern::List     &plist,
                               Pattern    *       pattern,
                               const char * const fileName)
    {
        const AutoPtr<const Pattern> p = pattern;
        p->toBinary(fileName);
        plist.pushTail( p->clone() );
        std::cerr << std::setw(32) << fileName << " -> '" << p->regularExpression() << "'" << std::endl;

        {
            InputFile fp(fileName);
            const AutoPtr<const Pattern> reloaded = Pattern::Read(fp);
            Y_CHECK( *reloaded == *p );
        }

    }

}


Y_UTEST(pattern)
{
    Pattern::List plist;

    process( plist, new Single('a'),                          "single.dat");
    process( plist, new Range('a','z'),                       "range.dat");
    process( plist, new Exclude('k'),                         "exclude.dat");
    process( plist, Optional::Create( new Single('1') ),      "optional.dat");
    process( plist, Repeated::Create( new Range('a','z'),0 ), "rep0.dat");
    process( plist, Repeated::Create( new Range('0','9'),1 ), "rep1.dat");
    process( plist, Repeated::Create( new Range('A','Z'),5 ), "rep5.dat");
    process( plist, Counting::Create( new Range('A','Z'),1,3 ), "counting.dat");

    {
        AutoPtr<Logic> p = new And();
        process( plist, p.yield(), "and.dat");
    }



    // optional
    // repeated
    // counting
    
    Y_SIZEOF(Single);
    Y_SIZEOF(Range);
    Y_SIZEOF(Exclude);
    Y_SIZEOF(Optional);
    Y_SIZEOF(Repeated);
    

}
Y_UDONE()
