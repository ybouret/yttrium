

#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/container/cxx/series.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {
        class Injector
        {
        public:
            explicit Injector(const Clusters &cls);
            virtual ~Injector() noexcept;

            class Broken
            {
            public:
                Broken(const Conservation::Law &l,
                       const xreal_t            s,
                       XWritable               &d) :
                law(l),
                score(s),
                delta(d)
                {
                }

                Broken(const Broken &other) noexcept :
                law(other.law),
                score(other.score),
                delta(other.delta)
                {

                }

                ~Broken() noexcept {}

                friend std::ostream & operator<<(std::ostream &os, const Broken &self)
                {
                    os << real_t(self.score) << " @" << self.law << " -> " << self.delta;
                    return os;
                }


                const Conservation::Law &law;
                xreal_t                  score;
                XWritable               &delta;

            private:
                Y_DISABLE_ASSIGN(Broken);
            };


            const bool        used;
            const size_t      rows;
            const size_t      cols;
            CxxSeries<Broken> jail;
            XMatrix           Cnew;
            CxxArray<XAdd>    Cinj;
            XAdd              xadd;

            void process(const Clusters &cls,
                         XWritable      &Cin,
                         XMLog          &xml);

            void process(const Conservation::Laws::Group &grp,
                         XWritable                       &Cin,
                         XMLog                           &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Injector);

        };
    }
}

namespace Yttrium
{
    namespace Chemical
    {
        Injector:: Injector(const Clusters &cls) :
        used(cls.maxLPG>0),
        rows( used ? cls.maxLPG : 0),
        cols( used ? cls.maxSPC : 0),
        jail(rows),
        Cnew( rows, cols),
        Cinj( cols ),
        xadd()
        {
        }

        Injector:: ~Injector() noexcept
        {
        }

        void Injector:: process(const Clusters &cls,
                                XWritable      &Cin,
                                XMLog          &xml)
        {
            Y_XML_SECTION_OPT(xml,"Inject"," clusters='" << cls->size << "'");
            for(size_t i=cols;i>0;--i) Cinj[i].free();
            for(const Cluster *cl=cls->head;cl;cl=cl->next)
            {
                const bool hasLaws = cl->laws.isValid();

                Y_XML_SECTION_OPT(xml, "Cluster", " laws='" << (hasLaws?cl->laws->size:0) << "'");
                if(!hasLaws) return;

                for(const Conservation::Laws::Group *g=cl->laws->groups.head;g;g=g->next)
                    process(*g,Cin,xml);

            }

        }

        void Injector:: process(const Conservation::Laws::Group &grp,
                                XWritable                       &Cin,
                                XMLog                           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Group", " size='" << grp.size << "'");

            //__________________________________________________________________
            //
            //
            // initialize
            //
            //__________________________________________________________________
            const xreal_t zero;
            size_t        nbad = 0;
            jail.free();
            for(Conservation::LNode *ln=grp.head;ln;ln=ln->next)
            {
                const Conservation::Law &law = **ln;
                const xreal_t            xs  = law.excess(Cin,TopLevel,xadd);
                if(xs<zero)
                {
                    XWritable &Cxs = Cnew[++nbad];
                    Cxs.ld(zero);
                    const xreal_t scale = -xs;
                    //const xreal_t score = law.inject(scale, Cxs, SubLevel, xadd);
                    //const Broken &broken = jail.grow(law, score, Cxs);
                    //Y_XMLOG(xml, real_t(xs) << " => " << broken);
                }
            }
        }

    }

}


Y_UTEST(plexus)
{
    Weasel::Compiler &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);
    (void) clusters.K(0);

    clusters.showK(std::cerr);

    {
        OutputFile fp("clusters.dot");
        GraphViz::Vizible::Enter(fp, "G");
        size_t cid=0;
        for(const Cluster *cl=clusters->head;cl;cl=cl->next,++cid)
        {
            cl->viz(fp,cid);
        }
        GraphViz::Vizible::Leave(fp);
    }

    GraphViz::Vizible::RenderPNG("clusters.dot", true);

    Y_SIZEOF(Cluster);
    Y_SIZEOF(Clusters);

    const size_t    m = lib->size();
    Vector<xreal_t> C0(m,0);

    Species::Conc(C0,ran,0.3,0.5);

    std::cerr << "C0=" << C0 << std::endl;


    Injector injector(clusters);
    injector.process(clusters, C0, xml);


}
Y_UDONE()
