

#include "y/lingo/pattern/all.hpp"
#include "y/lingo/pattern/char-db.hpp"
#include "y/stream/libc/input.hpp"
#include "y/vfs/vfs.hpp"
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
            CharDB fc;
            p->query(fc);
            const AutoPtr<const Pattern> F = fc.compile();
            std::cerr << "firstChars=" << F->regularExpression() << std::endl;
        }

        {
            InputFile fp(fileName);
            const AutoPtr<const Pattern> reloaded = Pattern::Read(fp);
            Y_CHECK( *reloaded == *p );
        }

        {
            const String dotName = VFS::ChangedExtension("dot",fileName);
            std::cerr << "dotName=" << dotName << std::endl;
            GraphViz::Vizible::DotToPng(dotName,*p);
        }

    }

}


Y_UTEST(pattern)
{
    Pattern::List plist;

    process( plist, new Any1(),                                 "any1.dat");
    process( plist, new Void(),                                 "void.dat");
    process( plist, new Single('a'),                            "single.dat");
    process( plist, new Range('a','z'),                         "range.dat");
    process( plist, new Exclude('k'),                           "exclude.dat");
    process( plist, Optional::Create( new Single('1') ),        "optional.dat");
    process( plist, Repeated::Create( new Range('a','z'),0 ),   "rep0.dat");
    process( plist, Repeated::Create( new Range('0','9'),1 ),   "rep1.dat");
    process( plist, Repeated::Create( new Range('A','Z'),5 ),   "rep5.dat");
    process( plist, Counting::Create( new Range('A','Z'),1,3 ), "counting.dat");

    {
        AutoPtr<Logic> p = new And();
        p->add('_');
        p->add('a','z');
        p->add('A','Z');
        process( plist, p.yield(), "and.dat");
    }

    {
        AutoPtr<Logic> p = new Or();
        p->add('_');
        p->add('a','z');
        p->add('A','Z');
        process( plist, p.yield(), "or.dat");
    }



    // optional
    // repeated
    // counting
    
    Y_SIZEOF(Single);
    Y_SIZEOF(Range);
    Y_SIZEOF(Exclude);
    std::cerr << std::endl;

    Y_SIZEOF(Optional);
    Y_SIZEOF(Repeated);
    Y_SIZEOF(Counting);
    std::cerr << std::endl;

    Y_SIZEOF(And);
    


}
Y_UDONE()
