

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
        std::cerr << "<" << fileName << ">" << std::endl;
        const AutoPtr<const Pattern> p = pattern;
        p->toBinary(fileName);
        plist.pushTail( p->clone() );
        std::cerr << std::setw(32) << fileName << " -> '" << p->regularExpression() << "'" << std::endl;

        {
            CharDB fc;
            p->query(fc);
            const AutoPtr<const Pattern> F = fc.compile();
            std::cerr << "\tfirstChars='" << F->regularExpression() << "' / '" << fc << "'" << std::endl;
        }

        {
            InputFile fp(fileName);
            const AutoPtr<const Pattern> reloaded = Pattern::Read(fp);
            Y_ASSERT( *reloaded == *p );
        }

        {
            const String dotName = VFS::ChangedExtension("dot",fileName);
            std::cerr << "\tdotName=" << dotName << std::endl;
            GraphViz::Vizible::DotToPng(dotName,*p);
        }
        std::cerr << "<" << fileName << "/>" << std::endl << std::endl;
    }

}


Y_UTEST(pattern)
{
    Pattern::List plist;

    process( plist, new Any1(),                                 "any1.dat");
    process( plist, new Void(),                                 "void.dat");
    process( plist, new Byte('a'),                              "byte.dat");
    process( plist, new Lump('a','z'),                          "lump.dat");
    process( plist, new Excl('k'),                              "excl.dat");

    process( plist, Optional::Create( new Byte('1') ),          "optional.dat");
    process( plist, MoreThan::Create( new Lump('a','z'),0 ),    "rep0.dat");
    process( plist, MoreThan::Create( new Lump('0','9'),1 ),    "rep1.dat");
    process( plist, MoreThan::Create( new Lump('A','Z'),5 ),    "rep5.dat");
    process( plist, Counting::Create( new Lump('A','Z'),1,3 ),  "counting.dat");

    {
        AutoPtr<Logic> p = new And();
        p->add('_');
        p->add('a','z');
        p->add('A','Z');
        process( plist, p.yield(), "and.dat");
    }

    return 0;

    {
        AutoPtr<Logic> p = new Or();
        p->add('_');
        p->add('a','z');
        p->add('A','Z');
        process( plist, p.yield(), "or.dat");
    }

    {
        AutoPtr<Logic> p = new None();
        p->add('_');
        p->add('a','z');
        p->add('A','Z');
        process( plist, p.yield(), "none.dat");
    }

    
    Y_SIZEOF(Any1);
    Y_SIZEOF(Void);
    Y_SIZEOF(Byte);
    Y_SIZEOF(Lump);
    Y_SIZEOF(Excl);
    std::cerr << std::endl;

    Y_SIZEOF(Optional);
    Y_SIZEOF(MoreThan);
    Y_SIZEOF(Counting);
    std::cerr << std::endl;

    Y_SIZEOF(And);
    Y_SIZEOF(Or);
    Y_SIZEOF(None);

#if 0
    const int lower2upper = int('A')-int('a');
    for(char c='a';c<='z';++c)
    {
        std::cerr << c << " -> " << char(c+lower2upper) << std::endl;
    }
#endif

}
Y_UDONE()
