

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
                gain(0),
                claw(l),
                Csub(c)
                {
                }

                Broken(const Broken &b) noexcept :
                gain(b.gain),
                claw(b.claw),
                Csub(b.Csub)
                {

                }

                friend std::ostream & operator<<(std::ostream &os, const Broken &self)
                {
                    const xreal_t zero;
                    os << real_t(self.gain) << " @" << (self.claw); // << " -> " << self.Cok;
                    if(self.gain>zero) { os << "->"; self.claw.displayCompact(os,self.Csub,SubLevel); }
                    return os;
                }

                ~Broken() noexcept
                {
                }

                static int Compare(const Broken &lhs, const Broken &rhs) noexcept
                {
                    return Comparison::Increasing(lhs.gain, rhs.gain);
                }

                bool still(const XReadable &Ctop, XAdd &xadd)
                {
                    return claw.broken(gain, Csub, SubLevel, Ctop, TopLevel, xadd);
                }

                xreal_t                  gain;
                const Conservation::Law &claw;
                XWritable               &Csub;




            private:
                Y_DISABLE_ASSIGN(Broken);
            };

            const bool        used;
            const size_t      rows; //!< max group size
            const size_t      cols; //!< max species in sub-level
            CxxSeries<Broken> jail;
            XMatrix           Cnew;
            CxxArray<XAdd>    Cinj;
            XAdd              xadd;
            const xreal_t     zero;

            void process(const Clusters  &cls,
                         XWritable       &Ctop,
                         XWritable       &dTop,
                         XMLog           &xml)
            {
                dTop.ld(zero);
                for(const Cluster *cl = cls->head;cl;cl=cl->next)
                {
                    process(*cl,Ctop,dTop,xml);
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Injector);
            void process(const Cluster  &cl,
                         XWritable      &Ctop,
                         XWritable      &dTop,
                         XMLog          &xml);

            void process(const Conservation::Laws::Group &grp,
                         XWritable                       &Ctop,
                         XMLog                           &xml);
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
        xadd(),
        zero()
        {
        }

        Injector:: ~Injector() noexcept
        {
        }

        void Injector:: process(const Cluster  &cl,
                                XWritable      &Ctop,
                                XWritable      &dTop,
                                XMLog          &xml)
        {
            Y_XML_SECTION(xml,"Inject");

            //------------------------------------------------------------------
            //
            // clean all injected
            //
            //------------------------------------------------------------------
            for(size_t i=cols;i>0;--i) Cinj[i].free();


            const bool hasLaws = cl.laws.isValid();

            Y_XML_SECTION_OPT(xml, "Cluster", " laws='" << (hasLaws?cl.laws->size:0) << "'");
            if(hasLaws)
            {
                // process all laws per group
                for(const Conservation::Laws::Group *g=cl.laws->groups.head;g;g=g->next)
                    process(*g,Ctop,xml);
                for(const SNode *sn=cl.species.head;sn;sn=sn->next)
                {
                    const Species &s = **sn;
                    const size_t   i = s.indx[TopLevel];
                    const size_t   j = s.indx[SubLevel];
                    cl.uuid.lj(std::cerr,s.name) << " = " << Cinj[j] << std::endl;
                    dTop[i] = Cinj[j].sum();
                }

            }

        }

        void Injector:: process(const Conservation::Laws::Group &grp,
                                XWritable                       &Ctop,
                                XMLog                           &xml)
        {
            Y_XML_SECTION_OPT(xml, "Group", " size='" << grp.size << "'");


            //__________________________________________________________________
            //
            //
            // initialize with all broken laws
            //
            //__________________________________________________________________
            jail.free();
            {
                Y_XML_SECTION(xml, "Initialize");
                for(Conservation::LNode *ln=grp.head;ln;ln=ln->next)
                {
                    const Conservation::Law &claw = **ln;
                    Broken                   broken(claw,Cnew[jail.size()+1]);
                    if(broken.still(Ctop,xadd))
                    {
                        Y_XMLOG(xml," (+) " << broken);
                        jail << broken;
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            // iterative reduction
            //
            //__________________________________________________________________
            while(jail.size()>0)
            {
                {
                    Y_XML_SECTION_OPT(xml,"Reduction"," broken='" << jail.size() << "'");

                    //__________________________________________________________
                    //
                    // order by increasing gain
                    //__________________________________________________________
                    HeapSort::Call(jail,Broken::Compare);
                    for(size_t i=1;i<jail.size();++i) { Y_XMLOG(xml," (+) " << jail[i]); }

                    //__________________________________________________________
                    //
                    // fixed most broken
                    //__________________________________________________________
                    {
                        const Broken & best = jail.tail();
                        Y_XMLOG(xml," (*) " << best);
                        const Conservation::Law &claw = best.claw;
                        const XReadable         &Csub = best.Csub;
                        for(const Actor *a=claw->head;a;a=a->next)
                        {
                            const size_t   isub = a->sp.indx[SubLevel];
                            const size_t   itop = a->sp.indx[TopLevel];
                            const xreal_t  Cnew = Csub[ isub ];
                            xreal_t       &Cold = Ctop[ itop ];
                            assert(Cnew>=Cold);
                            Cinj[isub] << (Cnew-Cold); // store  difference
                            Cold = Cnew;               // update concentration
                        }
                    }

                    //__________________________________________________________
                    //
                    // remove it
                    //__________________________________________________________
                    jail.popTail();
                    if(jail.size()<=0) break;
                }


                //__________________________________________________________________
                //
                //
                // update remaining broken
                //
                //__________________________________________________________________
                {
                    Y_XML_SECTION_OPT(xml,"Upgrading"," broken='" << jail.size() << "'");
                    for(size_t i=jail.size();i>0;--i)
                    {
                        if( jail[i].still(Ctop,xadd) )
                        {
                            Y_XMLOG(xml, " (+) " << jail[i]);
                        }
                        else
                        {
                            Y_XMLOG(xml, " (-) " << jail[i]);
                            jail.remove(i);
                        }
                    }
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
    Vector<xreal_t> dC(m,0);

    Species::Conc(C0,ran,0.3,0.5);

    //std::cerr << "C0=" << C0 << std::endl;
    lib(std::cerr,"[",C0,"]");

    Injector injector(clusters);
    injector.process(clusters,C0,dC,xml);

    lib(std::cerr << "C1=","\t[",C0,"]");
    lib(std::cerr << "dC=","\t[",dC,"]");


}
Y_UDONE()
