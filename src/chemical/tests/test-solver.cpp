
#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/joint.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(solver)
{
    typedef ArkPtr<KeyType,Joint>   JPtr;
    typedef SuffixSet<KeyType,JPtr> JSet;

    Plexus plexus(true);
    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    const Clusters &cls = plexus.assemble();
    JSet jset;
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        const JPtr jptr = new Joint(*cl);
        Y_CHECK(jset.insert(jptr));
    }
}
Y_UDONE()
