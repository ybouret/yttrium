

#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/clusters.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/container/cxx/series.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

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
                       XWritable               &c) noexcept :
                win(),
                law(l),
                Cok(c)
                {
                }

                Broken(const Broken &b) noexcept :
                win(b.win),
                law(b.law),
                Cok(b.Cok)
                {

                }

                friend std::ostream & operator<<(std::ostream &os, const Broken &self)
                {
                    os << real_t(self.win) << " @" << (self.law); // << " -> " << self.Cok;
                    return os;
                }

                ~Broken() noexcept
                {
                }

                static int Compare(const Broken &lhs, const Broken &rhs) noexcept
                {
                    return Comparison::Increasing(lhs.win, rhs.win);
                }


                xreal_t                  win;
                const Conservation::Law &law;
                XWritable               &Cok;




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

            void process(const Cluster  &cls,
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
        jail( rows ),
        Cnew( rows, cols),
        Cinj( used ? cls.species.size : 0 ),
        xadd()
        {
        }

        Injector:: ~Injector() noexcept
        {
        }

        void Injector:: process(const Cluster  &cl,
                                XWritable      &Cin,
                                XMLog          &xml)
        {
            Y_XML_SECTION(xml,"Inject");

            // clean all injected
            for(size_t i=cols;i>0;--i) Cinj[i].free();


            const bool hasLaws = cl.laws.isValid();

            Y_XML_SECTION_OPT(xml, "Cluster", " laws='" << (hasLaws?cl.laws->size:0) << "'");
            if(hasLaws)
            {
                    for(const Conservation::Laws::Group *g=cl.laws->groups.head;g;g=g->next)
                        process(*g,Cin,xml);
            }

            std::cerr << "Cinj=" << Cinj << std::endl;
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
            {
                Y_XML_SECTION(xml, "Initialize");
                for(Conservation::LNode *ln=grp.head;ln;ln=ln->next)
                {
                    const Conservation::Law &law = **ln;
                    Broken                   broken(law,Cnew[jail.size()+1]);
                    if(law.broken(broken.win, broken.Cok, SubLevel, Cin, TopLevel, xadd))
                    {
                        Y_XMLOG(xml," (+) " << broken);
                        jail << broken;
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // iterative recursion
            //
            //__________________________________________________________________
            while(jail.size()>0)
            {
                Y_XML_SECTION_OPT(xml,"Reduction"," broken='" << jail.size() << "'");
                HeapSort::Call(jail,Broken::Compare);
                for(size_t i=1;i<jail.size();++i)
                {
                    Y_XMLOG(xml," (+) " << jail[i]);
                }

                // fixed most broken
                {
                    const Broken & best = jail.tail();
                    Y_XMLOG(xml," (*) " << best);
                    const Conservation::Law &law = best.law;
                    const XReadable         &Cok = best.Cok;
                    for(const Actor *a=law->head;a;a=a->next)
                    {
                        const size_t   isub = a->sp.indx[SubLevel];
                        const size_t   itop = a->sp.indx[TopLevel];
                        const xreal_t  Cnew = Cok[ isub ];
                        xreal_t       &Cold = Cin[ itop ];
                        assert(Cnew>=Cold);
                        Cinj[isub] << (Cnew-Cold);
                    }
                }

                // remove it
                jail.popTail();
                
                // update


                break;
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
    for(const Cluster *cl=clusters->head;cl;cl=cl->next)
    {
        injector.process(*cl, C0, xml);
    }

}
Y_UDONE()
