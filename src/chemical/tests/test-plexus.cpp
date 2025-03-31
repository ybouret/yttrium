
#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/conservation/wardens.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/chemical/plexus/equalizer/extents.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        class Gain
        {
        public:
            Gain(const xreal_t      _g,
                 const Components & _E,
                 const XReadable  & _C) noexcept :
            g(_g), E(_E), C(_C)
            {
            }

            ~Gain() noexcept {}

            Gain(const Gain &_) noexcept :
            g(_.g), E(_.E), C(_.C)
            {
            }

            Y_OSTREAM_PROTO(Gain);

            const xreal_t     g;
            const Components &E;
            const XReadable  &C;

        private:
            Y_DISABLE_ASSIGN(Gain);
        };

        std::ostream & operator<<(std::ostream &os, const Gain &gain)
        {
            os << std::setw(Restartable::Width) << gain.g.str() << " @" << gain.E.name << "=";
            gain.E.displayCompact(os,gain.C,SubLevel);
            return os;
        }

        typedef Small::SoloHeavyList<Gain> GList;
        typedef GList::NodeType            GNode;


        class Equalizer
        {
        public:
            explicit Equalizer(const Cluster &cls);
            virtual ~Equalizer() noexcept;
            typedef CxxArray<Extents,MemoryModel> Table;


            const Cluster &cluster;
            EqzBanks       banks;
            const size_t   nrows;
            const size_t   ncols;
            GList          gains;
            Table          table;
            XMatrix        c_eqz;
            XAdd           xadd;

            void operator()(XMLog &xml, XWritable &C0);

            void analyze(XMLog &xml, const Extents &, const XReadable &C0, const AddressBook * wanders);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };


        Equalizer:: Equalizer(const Cluster &cls) :
        cluster(cls),
        banks(),
        nrows(cluster.definite->size),
        ncols(cluster->species->size),
        gains(nrows),
        table(nrows,CopyOf,banks),
        c_eqz(nrows,nrows>0?ncols:nrows),
        xadd()
        {

        }

        Equalizer:: ~Equalizer() noexcept
        {
        }

        void Equalizer:: operator()(XMLog &xml, XWritable &C0)
        {
            Y_XML_SECTION(xml, "Equalizer");

            Y_XML_COMMENT(xml,"definite");
            gains.free();
            for(const ENode *en=cluster.definite->head;en;en=en->next)
            {
                const Equilibrium &eq   = **en;
                Y_XML_SECTION(xml,*eq.name);
                Extents           &exts = eq(table,AuxLevel);
                const Resultant    res  = exts(eq,C0,TopLevel, & cluster.wandering );
                Y_XMLOG(xml, "reactants :" << exts.reac);
                Y_XMLOG(xml, "products  :" << exts.prod);
                Y_XMLOG(xml, ResultantText(res) );

                switch(res)
                {
                    case Correct: continue;
                    case BadBoth: continue;
                    case BadReac: break;
                    case BadProd: break;
                }

                XWritable &cc = c_eqz[gains.size+1];
                cluster.gather(cc,C0);
                const Gain G(exts.generate(xadd, cc, eq, C0, TopLevel, &cluster.wandering),eq,cc);
                gains << G;
                std::cerr << G << std::endl;
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

    bool           verbose = true;
    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    cls.graphViz("cs");

    const size_t m = lib->size();
    XVector      C0(m,0); // concentration
    XVector      I0(m,0); // injection
    XVector      C(m,0);

    Library::Concentrations(C0,ran,0.1,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Equalizer eqz(*cl);
        eqz(xml,C0);
    }


    return 0;

    Conservation::Wardens wardens(cls);
    wardens(xml,C0,I0);

    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
    lib.show(std::cerr << "I0=", "\t[", I0, "]", xreal_t::ToString ) << std::endl;

    return 0;

    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactors cs(cls);
    cs(xml,C0);




    
    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);

    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Conservation::Canon);
    Y_SIZEOF(Conservation::Canons);

    Y_SIZEOF(Reactor);


}
Y_UDONE()

