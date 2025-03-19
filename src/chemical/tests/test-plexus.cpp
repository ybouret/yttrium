
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/aftermath.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/heavy/list/solo.hpp"

#include "y/sequence/vector.hpp"

#include "y/utest/run.hpp"

#include "y/random/mt19937.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::SoloHeavyList<Outcome> OutList;
        typedef OutList::NodeType             OutNode;


        class Reactor
        {
        public:

            explicit Reactor(const Cluster &persistentCluster) :
            cluster(persistentCluster),
            outList(),
            Ceq(cluster->equilibria->size,cluster->species->size)
            {
                outList.proxy->reserve(Ceq.rows);
            }

            virtual ~Reactor() noexcept
            {
            }

            //! solve topLevel
            void operator()(XMLog &xml, XWritable &C0, const XReadable &K0);


            const Cluster & cluster;
            Aftermath       aftermath;
            OutList         outList;
            XMatrix         Ceq;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };

        void Reactor:: operator()(XMLog &           xml,
                                  XWritable &       C0,
                                  const XReadable & K0)
        {
            Y_XML_SECTION(xml, "Reactor");

            {
                outList.free();
                bool emergency = false;
                for(const ENode *en=cluster->equilibria->head;en;en=en->next)
                {
                    const Components &eq  = **en;
                    const xreal_t     eK  = eq(K0,TopLevel);
                    XWritable        &cc  = cluster.gather(Ceq[outList.size+1],C0);
                    const Outcome     out = aftermath(eq,eK,cc,SubLevel,C0,TopLevel);
                    switch(out.st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "(-) [" << out.situation() << "] " << eq.name);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml, "(!) [" << out.situation() << "] " << eq.name);
                            emergency = true;
                            outList << out;
                            continue;

                        case Running:
                            if(emergency) continue;
                            Y_XMLOG(xml, "(+) [" << out.situation() << "] " << eq.name);
                            continue;
                    }
                }
            }

        }

    }

}

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{

    Random::MT19937 ran;
    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";

    Library    lib;
    Equilibria eqs;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        weasel(lib,eqs,Lingo::Module::Open(data,data) );
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool verbose = true;

    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        std::cerr << *cl << std::endl;
    }

    const size_t m = lib->size();
    XVector      C0(m,0);
    XVector      C(m,0);

    {
        Library::Concentrations(C0,ran);
        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        Aftermath am;
        for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
        {
            const Equilibrium &eq = **it;
            const xreal_t      eK = eq(cls.K,TopLevel);
            std::cerr << "solving " << eq.name << std::endl;
            lib.Concentrations(C0,ran);
            C.ld(C0);
            am(eq,eK,C,TopLevel,C0,TopLevel);
        }
    }

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Library::Concentrations(C0,ran);
        Reactor reactor(*cl);
        reactor(xml,C0,cls.K);
    }




    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);

}
Y_UDONE()

