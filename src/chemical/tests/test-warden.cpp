#include "y/chemical/weasel.hpp"
#include "y/chemical/plexus/warden.hpp"
#include "y/chemical/plexus/equalizer.hpp"

#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(warden)
{
    Random::Rand              ran;
    Chemical::Weasel &        weasel = Chemical::Weasel::Instance();
    Chemical::Library         lib;
    Chemical::LuaEquilibria   eqs;

    for(int i=1;i<argc;++i)
    {
        weasel( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }
    
    Vector<Chemical::xreal_t> C0(lib->size(),0),Injected(C0), C1(C0);

    for(size_t i=C0.size();i>0;--i)
    {
        C0[i] = Chemical::Species::Concentration(ran);
        if( ran.choice() ) C0[i] = - C0[i];
        C1[i] = C0[i];
    }



    bool  verbose = true;
    XMLog xml(verbose);
    Chemical::Constants K;
    Chemical::Clusters  cls(eqs,K,xml);

    lib(std::cerr << "C0=",C0) << std::endl;

    cls.graphViz("system");

    Banks        banks;
    Fence        fence(banks);
    XAdd         xadd;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Y_XML_SECTION_OPT(xml, "Cluster::Shaping"," cntl='" << cl->controllers.size << "'");
        for(const Controller *cntl=cl->controllers.head;cntl;cntl=cntl->next)
        {
            fence.shapeFull(cntl->components, C0);
            Y_XMLOG(xml," (*) " << fence);
            const unsigned res = fence.studyFull(xml);
            if( (res&Fence::IMPROVE) )
            {
                std::cerr << "Should improve..." << std::endl;
                for(size_t i=C0.size();i>0;--i)
                {
                    C1[i] = C0[i];
                }
                const xreal_t gain = cntl->primary.moveControl(C1, TopLevel, fence.cursor, fence.zeroed.head, C0, TopLevel, xadd);
                lib(std::cerr << "C1=",C1) << std::endl;
                const xreal_t b0 =cntl->primary.balance(C0, TopLevel, xadd);
                const xreal_t b1 =cntl->primary.balance(C1, TopLevel, xadd);

                std::cerr << "|C0| = " << real_t(b0) << std::endl;
                std::cerr << "|C1| = " << real_t(b1) << std::endl;

                std::cerr << "gain = " << real_t(gain) << " / " << real_t(b0-b1) << std::endl;

            }

        }
    }

    Equalizer equalizer(cls);
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        equalizer.tune(C0,*cl,xml);
    }

    return 0;
    Chemical::Conservation::Warden warden(cls);

    warden(C0, Injected, cls.groups,xml);




}
Y_UDONE()
