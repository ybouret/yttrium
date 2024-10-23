
#include "y/lingo/pattern/posix.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/input.hpp"
using namespace Yttrium;
using namespace Lingo;



static inline void testPosix(const char * const name)
{
    std::cerr << "Testing " << name << std::endl;
    const AutoPtr<const Pattern> motif = posix::named(name);
    {
        const String outName = String(name) + ".dat";
        motif->toBinary(outName);
        {
            InputFile fp(outName);
            const AutoPtr<const Pattern> reloaded = Pattern::Read(fp);
            Y_CHECK(*reloaded==*motif);
        }
    }

    {
        const String dotName = String(name) + ".dot";
        GraphViz::Vizible::DotToPng(dotName,*motif);
    }

}

#define Y_Lingo_Posix(NAME) testPosix(#NAME);

Y_UTEST(posix)
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

    //GraphViz::Vizible::DotToPng("regexp.dot",*p);


}
Y_UDONE()
