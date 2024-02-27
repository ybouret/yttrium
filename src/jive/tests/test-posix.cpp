

#include "y/jive/pattern/posix.hpp"

#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;

static inline
void testPosix( Jive::Pattern *p, const char *id)
{
    const AutoPtr<Jive::Pattern> keep(p);
    const String root     = id;
    const String dataFile = root + ".dat";

    
    {
        Libc::OutputFile fp(dataFile);
        p->serialize(fp);
    }

    {
        InputFile fp(dataFile);
        const AutoPtr<Jive::Pattern> reloaded = Jive::Pattern::ReadFrom(fp);
        Y_CHECK(*reloaded == *p );
    }

    const String dotFile = root + ".dot";
    GraphViz::Vizible::DotToPng(dotFile,*p);

}

#define Y_JIVE_POSIX(NAME) do {            \
std::cerr << "--> " << #NAME << std::endl; \
testPosix( Jive::posix:: NAME(), #NAME );  \
} while(false)

Y_UTEST(posix)
{
    Y_JIVE_POSIX(lower);
    Y_JIVE_POSIX(upper);
    Y_JIVE_POSIX(alpha);
    Y_JIVE_POSIX(digit);
    Y_JIVE_POSIX(alnum);
    Y_JIVE_POSIX(word);
    Y_JIVE_POSIX(xdigit);
    Y_JIVE_POSIX(blank);
    Y_JIVE_POSIX(space);
    Y_JIVE_POSIX(punct);
    Y_JIVE_POSIX(word);
    Y_JIVE_POSIX(endl);
    Y_JIVE_POSIX(dot);
    Y_JIVE_POSIX(core);
    Y_JIVE_POSIX(vowel);
    Y_JIVE_POSIX(consonant);

}
Y_UDONE()

